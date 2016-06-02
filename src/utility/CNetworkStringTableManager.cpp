#include <algorithm>
#include <cstdio>

#include "NetworkConstants.h"

#include "CNetworkBuffer.h"

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

void CNetworkStringTableManager::Clear()
{
	for( auto pTable : m_Tables )
	{
		delete pTable;
	}

	m_Tables.clear();
}

bool CNetworkStringTableManager::Serialize( CNetworkBuffer& buffer, const float flTime )
{
	bool bWroteSomething = false;

	for( auto pTable : m_Tables )
	{
		if( buffer.HasOverflowed() )
			break;

		if( pTable->Serialize( buffer, flTime, 0 ) )
		{
			bWroteSomething = true;
		}
	}

	return bWroteSomething;
}

bool CNetworkStringTableManager::UnserializeTable( const CNetworkStringTable::TableID_t ID, CNetworkBuffer& buffer )
{
	bool bSuccess = false;

	if( auto pTable = GetTableByIndex( ID ) )
	{
		bSuccess = pTable->Unserialize( buffer );
	}
	else
	{
		printf( "CNetworkStringTableManager::Unserialize: Invalid table index %u!\n", ID );
	}

	return bSuccess;
}