#include <cassert>
#include <cstdint>

#include "CNetBufInputStream.h"

bool CNetBufInputStream::Next( const void** data, int* size )
{
	if( m_ByteCount >= m_uiMessageSize )
		return false;

	const size_t uiChunk = m_uiMessageSize - static_cast<size_t>( m_ByteCount );

	assert( uiChunk <= INT_MAX );

	if( uiChunk > INT_MAX )
		return false;

	*data = m_Buffer.GetCurrentData();

	*size = uiChunk;

	m_ByteCount += *size;

	return m_Buffer.ReadAndDiscardBytes( *size );
}

void CNetBufInputStream::BackUp( int count )
{
	m_Buffer.BackUpBytes( count );

	m_ByteCount -= count;
}

bool CNetBufInputStream::Skip( int count )
{
	m_ByteCount += count;

	return m_Buffer.ReadAndDiscardBytes( count );
}