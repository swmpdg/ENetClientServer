#ifndef UTILITY_CNETBUFINPUTSTREAM_H
#define UTILITY_CNETBUFINPUTSTREAM_H

#include <google/protobuf/io/zero_copy_stream.h>

#include "CNetworkBuffer.h"

class CNetBufInputStream final : public google::protobuf::io::ZeroCopyInputStream
{
public:
	static const size_t DEFAULT_CHUNK_SIZE = 1024;

public:
	CNetBufInputStream( CNetworkBuffer& buffer, const size_t uiChunkSize = DEFAULT_CHUNK_SIZE )
		: m_Buffer( buffer )
		, m_uiChunkSize( uiChunkSize > 0 ? uiChunkSize : DEFAULT_CHUNK_SIZE )
	{
		m_Buffer.PadToByte();
	}

	bool Next( const void** data, int* size ) override final;

	void BackUp( int count ) override final;

	bool Skip( int count ) override final;

	google::protobuf::int64 ByteCount() const override final { return m_ByteCount; }

private:
	CNetworkBuffer& m_Buffer;
	const size_t m_uiChunkSize;
	google::protobuf::int64 m_ByteCount = 0;

private:
	CNetBufInputStream( const CNetBufInputStream& ) = delete;
	CNetBufInputStream& operator=( const CNetBufInputStream& ) = delete;
};

#endif //UTILITY_CNETBUFINPUTSTREAM_H