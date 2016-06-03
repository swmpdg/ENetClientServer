#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>

#include "networking/CNetworkBuffer.h"

#include "utility/CWorldTime.h"

#include "utility/Util.h"

#include "networking/NetworkConstants.h"

#include "networking/NetworkUtils.h"

#include "messages/sv_cl_messages/NetTable.pb.h"

#include "CNetworkStringTable.h"

#undef GetCurrentTime

CNetworkStringTable::CNetworkStringTable( const char* const pszName, const NST::TableID_t tableID, const size_t uiMaxEntries )
	: m_pszName( pszName )
	, m_TableID( tableID )
	, m_uiMaxEntries( uiMaxEntries )
	, m_uiEntriesBits( static_cast<size_t>( log2( uiMaxEntries ) ) )
{
	assert( pszName && *pszName );
}

size_t CNetworkStringTable::IndexOf( const char* const pszString ) const
{
	if( !pszString )
		return NST::INVALID_STRING_INDEX;

	auto it = std::find_if( m_TableEntries.begin(), m_TableEntries.end(), 
		[ = ]( const TableEntry_t& entry )
		{
			return strcmp( pszString, entry.pszString ) == 0;
		}
	);

	if( it != m_TableEntries.end() )
	{
		return it - m_TableEntries.begin();
	}

	return NST::INVALID_STRING_INDEX;
}

bool CNetworkStringTable::IsValidIndex( const size_t uiIndex ) const
{
	return uiIndex != NST::INVALID_STRING_INDEX && uiIndex < m_TableEntries.size();
}

const char* CNetworkStringTable::GetString( const size_t uiIndex ) const
{
	if( !IsValidIndex( uiIndex ) )
		return nullptr;

	return m_TableEntries[ uiIndex ].pszString;
}

size_t CNetworkStringTable::Add( const char* const pszString )
{
	if( !pszString )
		return NST::INVALID_STRING_INDEX;

	const size_t uiIndex = IndexOf( pszString );

	if( uiIndex != NST::INVALID_STRING_INDEX )
		return uiIndex;

	if( m_TableEntries.size() >= m_uiMaxEntries )
	{
		printf( "CNetworkStringTable::Add: Network String Table %s has reached maximum entries! (%u)\n", m_pszName, m_uiMaxEntries );
		return NST::INVALID_STRING_INDEX;
	}

	TableEntry_t entry;

	entry.pszString = g_StringPool.Allocate( pszString );
	entry.flLastModifiedTime = WorldTime.GetCurrentTime();

	m_TableEntries.push_back( entry );

	m_flLastModifiedTime = WorldTime.GetCurrentTime();

	return m_TableEntries.size() - 1;
}

void CNetworkStringTable::Clear()
{
	m_TableEntries.clear();

	m_flLastModifiedTime = WorldTime.GetCurrentTime();
}

NST::SerializeResult CNetworkStringTable::Serialize( CNetworkBuffer& buffer, const float flTime, const size_t uiStart, const bool bAllowOverflow )
{
	const float flCurrentTime = WorldTime.GetCurrentTime();

	bool bWroteSomething = false;

	bool bFullySerialized = true;

	//Use half a buffer worth at most.
	uint8_t msgBuf[ MAX_DATAGRAM / 2 ];

	CNetworkBuffer tempBuf( "CNetworkStringTable::Serialize_buffer", msgBuf, sizeof( msgBuf ) );

	size_t uiPrevious = NST::INVALID_STRING_INDEX;

	for( size_t uiIndex = uiStart; uiIndex < m_TableEntries.size(); ++uiIndex )
	{
		const auto& entry = m_TableEntries[ uiIndex ];

		if( entry.flLastModifiedTime < flTime )
			continue;

		//If we can only write 1 more bit, we need to save it for the end of list bit.
		if( tempBuf.CheckOverflow( 2 ) )
		{
			bFullySerialized = false;
			break;
		}

		const size_t uiOffset = tempBuf.GetBitsInBuffer();

		tempBuf.WriteOneBit( 1 );

		if( uiPrevious != NST::INVALID_STRING_INDEX && uiPrevious + 1 == uiIndex )
		{
			tempBuf.WriteOneBit( 1 );

			//Code commonly precaches resources that have similar paths. This allows them to share that.
			//E.g.:
			//weapons/hks1.wav
			//weapons/hks2.wav
			const size_t uiCommon = UTIL_FindCommonBaseCount( m_TableEntries[ uiPrevious ].pszString, entry.pszString ) & COMMON_BITS_MASK;

			if( uiCommon > 0 )
			{
				tempBuf.WriteOneBit( 1 );
				tempBuf.WriteUnsignedBitLong( uiCommon, NUM_COMMON_BITS );
				tempBuf.WriteString( entry.pszString + uiCommon );
			}
			else
			{
				tempBuf.WriteOneBit( 0 );
				tempBuf.WriteString( entry.pszString );
			}
		}
		else
		{
			tempBuf.WriteOneBit( 0 );
			tempBuf.WriteUnsignedBitLong( uiIndex, m_uiEntriesBits );
			tempBuf.WriteString( entry.pszString );
		}

		if( tempBuf.HasOverflowed() )
		{
			//Buffer full. Restore to last complete contents and stop.
			tempBuf.BackUpBits( tempBuf.GetBitsInBuffer() - uiOffset );
			tempBuf.ClearOverflow();
			bFullySerialized = false;
			break;
		}

		//Set this after the overflow check so it doesn't return true if the buffer was too full to start with.
		bWroteSomething = true;

		uiPrevious = uiIndex;
	}

	//Nothing left to write.
	tempBuf.WriteOneBit( 0 );

	if( bWroteSomething )
	{
		sv_cl_messages::NetTable table;

		table.set_command( sv_cl_messages::NetTable_Command_UPDATE );
		table.set_tableid( m_TableID );

		table.set_data( tempBuf.GetData(), tempBuf.GetBytesInBuffer() );

		if( !SerializeToBuffer( SVCLMessage::NETTABLE, table, buffer ) )
			return NST::SerializeResult::OVERFLOW;
	}

	if( !bAllowOverflow && !bFullySerialized )
		return NST::SerializeResult::OVERFLOW;

	return bWroteSomething ? NST::SerializeResult::WROTEDATA : NST::SerializeResult::WROTENOTHING;
}

bool CNetworkStringTable::Unserialize( CNetworkBuffer& buffer )
{
	const float flCurrentTime = WorldTime.GetCurrentTime();

	char szBuffer[ 4096 ];

	size_t uiIndex;

	//As long as there's another entry
	while( buffer.ReadOneBit() )
	{
		if( buffer.ReadOneBit() )
		{
			++uiIndex;

			if( buffer.ReadOneBit() )
			{
				const size_t uiCommon = buffer.ReadUnsignedBitLong( NUM_COMMON_BITS );

				strncpy( szBuffer, m_TableEntries[ uiIndex - 1 ].pszString, uiCommon );
				buffer.ReadString( szBuffer + uiCommon, sizeof( szBuffer ) - uiCommon );

				szBuffer[ sizeof( szBuffer ) - 1 ] = '\0';
			}
			else
			{
				buffer.ReadString( szBuffer, sizeof( szBuffer ) );
			}
		}
		else
		{
			uiIndex = buffer.ReadUnsignedBitLong( m_uiEntriesBits );
			buffer.ReadString( szBuffer, sizeof( szBuffer ) );
		}

		if( buffer.HasOverflowed() )
			break;

		if( m_TableEntries.size() <= uiIndex )
		{
			//This should always be the next string to add.
			assert( uiIndex == m_TableEntries.size() );

			TableEntry_t entry;

			entry.pszString = g_StringPool.Allocate( szBuffer );
			entry.flLastModifiedTime = flCurrentTime;

			m_TableEntries.push_back( entry );
		}
		else
		{
			//Modifying an existing entry. The string itself cannot be modified.
			assert( strcmp( szBuffer, m_TableEntries[ uiIndex ].pszString ) == 0 );
		}
	}

	return !buffer.HasOverflowed();
}