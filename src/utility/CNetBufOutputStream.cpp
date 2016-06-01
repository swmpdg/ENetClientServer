#include <cassert>
#include <cstdint>

#include "CNetBufOutputStream.h"

bool CNetBufOutputStream::Next( void** data, int* size )
{
	const size_t uiBytesToReturn = m_Buffer.GetBytesLeft() >= m_uiChunkSize ? m_uiChunkSize : m_Buffer.GetBytesLeft();

	if( uiBytesToReturn == 0 )
		return false;

	//Avoid overflowing int.
	assert( uiBytesToReturn <= INT_MAX );

	if( uiBytesToReturn > INT_MAX )
		return false;

	auto pData = m_Buffer.GetCurrentData();

	m_Buffer.ExternalBytesWritten( uiBytesToReturn );

	*data = pData;
	*size = static_cast<int>( uiBytesToReturn );

	m_ByteCount += *size;

	return true;
}

void CNetBufOutputStream::BackUp( int count )
{
	m_Buffer.BackUpBytes( count );

	m_ByteCount -= count;
}

google::protobuf::int64 CNetBufOutputStream::ByteCount() const
{
	return m_ByteCount;
}