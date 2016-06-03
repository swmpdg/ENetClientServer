#include <algorithm>
#include <cstdio>

#include "networking/NetworkConstants.h"

#include "networking/CNetworkBuffer.h"

#include "PrivateNetStringTableConstants.h"

#include "CNetworkStringTable.h"

#include "CNetworkStringTableManager.h"

const CNetworkStringTable* CNetworkStringTableManager::GetTableByIndex( const size_t uiIndex ) const
{
	if( m_Tables.size() <= uiIndex )
		return nullptr;

	return m_Tables[ uiIndex ];
}

CNetworkStringTable* CNetworkStringTableManager::GetTableByIndex( const size_t uiIndex )
{
	return const_cast<CNetworkStringTable*>( const_cast<const CNetworkStringTableManager* const>( this )->GetTableByIndex( uiIndex ) );
}

const CNetworkStringTable* CNetworkStringTableManager::GetTableByName( const char* const pszName ) const
{
	if( !pszName || !( *pszName ) )
		return nullptr;

	auto it = std::find_if( m_Tables.begin(), m_Tables.end(),
		[ = ]( CNetworkStringTable* pTable )
		{
			return strcmp( pszName, pTable->GetName() ) == 0;
		}
	);

	return it != m_Tables.end() ? *it : nullptr;
}

CNetworkStringTable* CNetworkStringTableManager::GetTableByName( const char* const pszName )
{
	return const_cast<CNetworkStringTable*>( const_cast<const CNetworkStringTableManager* const>( this )->GetTableByName( pszName ) );
}

CNetworkStringTable* CNetworkStringTableManager::CreateTable( const char* const pszName, const size_t uiMaxEntries )
{
	if( !pszName || !( *pszName ) )
		return nullptr;

	if( uiMaxEntries == 0 )
	{
		printf( "CNetworkStringTableManager::CreateTable: Maximum number of entries must be greater than zero and a power of 2!\n" );
		return nullptr;
	}

	if( ( uiMaxEntries & ( uiMaxEntries - 1 ) ) != 0 )
	{
		printf( "CNetworkStringTableManager::CreateTable: Maximum number of entries must be greater than zero and a power of 2!\n" );
		return nullptr;
	}

	if( !TableCreationAllowed() )
	{
		printf( "CNetworkStringTableManager::CreateTable: Table creation is not allowed!\n" );
		return nullptr;
	}

	if( GetTableByName( pszName ) )
	{
		printf( "CNetworkStringTableManager::CreateTable: A table with name \"%s\" already exists!\n", pszName );
		return nullptr;
	}

	auto pTable = new CNetworkStringTable( pszName, NST::IndexToTableID( m_Tables.size() ), uiMaxEntries );

	m_Tables.push_back( pTable );

	return pTable;
}

void CNetworkStringTableManager::Clear()
{
	for( auto pTable : m_Tables )
	{
		delete pTable;
	}

	m_Tables.clear();
}

NST::SerializeResult CNetworkStringTableManager::Serialize( CNetworkBuffer& buffer, const float flTime )
{
	NST::SerializeResult result = NST::SerializeResult::WROTENOTHING;

	for( auto pTable : m_Tables )
	{
		if( buffer.HasOverflowed() )
			break;

		const auto tblResult = pTable->Serialize( buffer, flTime, 0 );

		if( tblResult == NST::SerializeResult::WROTEDATA )
		{
			result = NST::SerializeResult::WROTEDATA;
		}
		else if( tblResult == NST::SerializeResult::OVERFLOW )
		{
			return NST::SerializeResult::OVERFLOW;
		}
	}

	return result;
}

bool CNetworkStringTableManager::UnserializeTable( const NST::TableID_t ID, CNetworkBuffer& buffer )
{
	bool bSuccess = false;

	if( auto pTable = GetTableByIndex( NST::TableIDToIndex( ID ) ) )
	{
		bSuccess = pTable->Unserialize( buffer );
	}
	else
	{
		printf( "CNetworkStringTableManager::Unserialize: Invalid table index %u!\n", ID );
	}

	return bSuccess;
}