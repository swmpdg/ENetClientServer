#include <cstdio>

#include "CHashStringPool.h"

CHashStringPool::CHashStringPool()
	: m_Strings( 256 )
{
}

const char* CHashStringPool::Find( const char* pszString, const char* const pszDefault ) const
{
	Strings_t::const_iterator it = m_Strings.find( CAutoString( pszString ) );

	if( it != m_Strings.end() )
		return it->Get();

	return pszDefault;
}

const char* CHashStringPool::Allocate( const char* pszString, bool* pNewAllocation )
{
	std::pair<Strings_t::iterator, bool> result = m_Strings.emplace( pszString );

	if( pNewAllocation )
		*pNewAllocation = result.second;

	if( result.first != m_Strings.end() )
		return result.first->Get();

	return "";
}

void CHashStringPool::Clear()
{
	m_Strings.clear();
}

void CHashStringPool::DebugPrint() const
{
	size_t uiSizeInUse = 0;

	for( Strings_t::const_iterator it = m_Strings.begin(); it != m_Strings.end(); ++it )
	{
		uiSizeInUse += strlen( it->Get() ) + 1;
	}

	printf( "CHashStringPool: current status:\nNumber of strings allocated: %u\nMemory in use: %u bytes\n", m_Strings.size(), uiSizeInUse );
}