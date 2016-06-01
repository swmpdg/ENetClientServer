#include <cassert>
#include <cstdint>

#include "CNetBufInputStream.h"

bool CNetBufInputStream::Next( const void** data, int* size )
{
	if( m_Buffer.GetBytesLeft() <= 0 )
		return false;

	assert( m_Buffer.GetBytesLeft() <= INT_MAX );

	if( m_Buffer.GetBytesLeft() > INT_MAX )
		return false;

	*data = m_Buffer.GetCurrentData();

	*size = m_Buffer.GetBytesLeft();

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