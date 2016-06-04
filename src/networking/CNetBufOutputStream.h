#ifndef UTILITY_CNETBUFOUTPUTSTREAM_H
#define UTILITY_CNETBUFOUTPUTSTREAM_H

#include <google/protobuf/io/zero_copy_stream.h>

#include "CNetworkBuffer.h"

/**
*	@ingroup Networking
*
*	@{
*/

/**
*	Zero copy output stream designed to interface with CNetworkBuffer.
*	@see google::protobuf::io::ZeroCopyOutputStream
*	@see CNetworkBuffer
*/
class CNetBufOutputStream final : public google::protobuf::io::ZeroCopyOutputStream
{
public:
	static const size_t DEFAULT_CHUNK_SIZE = 1024;

public:
	/**
	*	Constructor.
	*	@param buffer Buffer that will be written to.
	*	@param uiChunkSize Size of the chunks to give to the output stream. Larger chunks may allow for fewer calls.
	*/
	CNetBufOutputStream( CNetworkBuffer& buffer, const size_t uiChunkSize = DEFAULT_CHUNK_SIZE )
		: m_Buffer( buffer )
		, m_uiChunkSize( uiChunkSize > 0 ? uiChunkSize : DEFAULT_CHUNK_SIZE )
	{
		m_Buffer.PadToByte();
	}

	bool Next( void** data, int* size ) override final;

	void BackUp( int count ) override final;

	google::protobuf::int64 ByteCount() const override final;

private:
	CNetworkBuffer& m_Buffer;
	const size_t m_uiChunkSize;
	google::protobuf::int64 m_ByteCount = 0;

private:
	CNetBufOutputStream( const CNetBufOutputStream& ) = delete;
	CNetBufOutputStream& operator=( const CNetBufOutputStream& ) = delete;
};

/** @} */

#endif //UTILITY_CNETBUFOUTPUTSTREAM_H