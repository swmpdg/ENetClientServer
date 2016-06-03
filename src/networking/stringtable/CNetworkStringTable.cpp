#include <algorithm>
#include <cassert>
#include <cstring>

#include "networking/CNetworkBuffer.h"

#include "utility/CWorldTime.h"

#include "utility/Util.h"

#include "networking/NetworkConstants.h"

#include "networking/NetworkUtils.h"

#include "messages/sv_cl_messages/NetTable.pb.h"

#include "CNetworkStringTable.h"

#undef GetCurrentTime

CNetworkStringTable::CNetworkStringTable( const char* const pszName, const NST::TableID_t tableID )
	: m_pszName( pszName )
	, m_TableID( tableID )
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

bool CNetworkStringTable::Serialize( CNetworkBuffer& buffer, const float flTime, const size_t uiStart )
{
	const float flCurrentTime = WorldTime.GetCurrentTime();

	bool bWroteSomething = false;

	bool bFullySerialized = true;

	uint8_t msgBuf[ MAX_DATAGRAM ];

	CNetworkBuffer tempBuf( "CNetworkStringTable::Serialize_buffer", msgBuf, sizeof( msgBuf ) );

	for( size_t uiIndex = uiStart; uiIndex < m_TableEntries.size(); ++uiIndex )
	{
		const auto& entry = m_TableEntries[ uiIndex ];

		if( entry.flLastModifiedTime < flTime )
			continue;

		const size_t uiOffset = tempBuf.GetBitsInBuffer();

		tempBuf.WriteOneBit( 1 );
		//TODO: bit count can be determined by maximum entries setting.
		//TODO: index doesn't always have to be sent. If a large number of sequential data is sent, it can be sent as a bit indicating previous + 1.
		tempBuf.WriteUnsignedBitLong( uiIndex, 32 );
		//TODO: code commonly precaches resources that have similar paths. A potential optimization here would allow them to share that.
		//E.g.:
		//weapons/hks1.wav
		//weapons/hks2.wav
		tempBuf.WriteString( entry.pszString );

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
	}

	//Nothing left to write.
	tempBuf.WriteOneBit( 0 );

	if( bWroteSomething )
	{
		sv_cl_messages::NetTable table;

		table.set_command( sv_cl_messages::NetTable_Command_UPDATE );
		table.set_tableid( m_TableID );

		table.set_data( tempBuf.GetData(), tempBuf.GetBytesInBuffer() );

		SerializeToBuffer( SVCLMessage::NETTABLE, table, buffer );
	}

	return bWroteSomething;
}

bool CNetworkStringTable::Unserialize( CNetworkBuffer& buffer )
{
	const float flCurrentTime = WorldTime.GetCurrentTime();

	char szBuffer[ 4096 ];

	//As long as there's another entry
	while( buffer.ReadOneBit() )
	{
		//TODO: optimize bit count
		const size_t uiIndex = buffer.ReadUnsignedBitLong( 32 );

		buffer.ReadString( szBuffer, sizeof( szBuffer ) );

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