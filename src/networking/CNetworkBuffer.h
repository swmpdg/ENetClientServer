#ifndef CNETWORKBUFFER_H
#define CNETWORKBUFFER_H

#include <cstdint>

//TODO: bounds checking

// Pad a number so it lies on an N byte boundary.
// So PadNumber(0,4) is 0 and PadNumber(1,4) is 4
template<typename T>
inline T PadNumber( const T iNum, const T iBoundary )
{
	return ( ( iNum + ( iBoundary - 1 ) ) / iBoundary ) * iBoundary;
}

template<typename T>
inline T BitByte( const T iBits )
{
	return PadNumber( iBits, static_cast<T>( 8 ) ) >> 3;
}

template<typename T>
inline T ByteBit( const T iBytes )
{
	return iBytes << 3;
}

/*
* A network buffer that points to a buffer containing arbitrary data
*/
class CNetworkBuffer final
{
public:
	CNetworkBuffer();
	CNetworkBuffer( const char* pszDebugName, uint8_t* pData, const size_t uiNumBytes, const size_t uiCurrentBit = 0 );

	static void InitMasks();

	const char* GetDebugName() const { return m_pszDebugName; }

	bool HasOverflowed() const { return m_fOverflowed; }

	const uint8_t* GetData() const { return m_pData; }

	uint8_t* GetData() { return m_pData; }

	const uint8_t* GetCurrentData() const { return m_pData + GetBytesInBuffer(); }
	uint8_t* GetCurrentData() { return m_pData + GetBytesInBuffer(); }

	size_t GetMaxBits() const { return m_uiMaxBits; }
	size_t GetMaxBytes() const { return BitByte( m_uiMaxBits ); }

	size_t GetBitsInBuffer() const { return m_uiCurrentBit; }
	size_t GetBytesInBuffer() const { return BitByte( m_uiCurrentBit ); }
	size_t GetUnpaddedBytesInBuffer() const { return BitByte( m_uiCurrentBit ); }

	size_t GetBitsLeft() const { return m_uiMaxBits - m_uiCurrentBit; }
	size_t GetBytesLeft() const { return BitByte( GetBitsLeft() ); }

	/*
	* This buffer is reset to an empty state
	*/
	void ResetToEmpty();

	/*
	* This buffer's read/write pointer is reset to the start
	* Resets the overflow flag
	*/
	void ResetToStart();

	void SetBuffer( const char* pszDebugName, uint8_t* pData, const size_t uiNumBytes, const size_t uiCurrentBit = 0 );

	/*
	* Checks whether the buffer would be overflowed if the given number of bits were to be written
	*/
	bool CheckOverflow( const size_t uiBits ) const;

	/*
	* If overflow occurs after writing uiBits bits, sets overflow flag
	*/
	bool Overflow( const size_t uiBits );

	/**
	*	Clears the overflow flag if it was set.
	*/
	void ClearOverflow();

	/*
	* Write methods
	*/

	void WriteOneBit( const int iValue );

	void WriteUnsignedBitLong( unsigned int iValue, const size_t uiNumBits );
	void WriteSignedBitLong( int iValue, const size_t uiNumBits );

	inline void WriteBitLong( const unsigned int iValue, const size_t uiNumBits, const bool fSigned )
	{
		if( fSigned )
			WriteSignedBitLong( ( int ) iValue, uiNumBits );
		else
			WriteUnsignedBitLong( iValue, uiNumBits );
	}

	void WriteBitFloat( const float flValue );
	bool WriteBits( const void* pData, const size_t iNumBits );

	void WriteChar( const int iValue );
	void WriteByte( const int iValue );
	void WriteShort( const int iValue );
	void WriteWord( const int iValue );
	void WriteLong( const int iValue );
	void WriteFloat( const float flValue );

	bool WriteBytes( const void* pData, const size_t uiNumBytes );
	bool WriteString( const char* pszString );

	bool PadToByte();

	bool ExternalBitsWritten( const size_t uiBits );

	bool ExternalBytesWritten( const size_t uiBytes );

	bool BackUpBits( const size_t uiBits );

	bool BackUpBytes( const size_t uiBytes );

	/*
	* Read methods
	*/
	int ReadOneBit();

	unsigned int ReadUnsignedBitLong( const size_t uiNumBits );
	int ReadSignedBitLong( const size_t uiNumBits );

	unsigned int ReadBitLong( const size_t uiNumBits, const bool fSigned )
	{
		if( fSigned )
			return ( unsigned int ) ReadSignedBitLong( uiNumBits );
		else
			return ReadUnsignedBitLong( uiNumBits );
	}

	float ReadBitFloat();
	bool ReadBits( void* pBuffer, const size_t uiNumBits );

	char ReadChar();
	uint8_t ReadByte();
	short ReadShort();
	unsigned short ReadWord();
	int ReadLong();
	float ReadFloat();

	bool ReadBytes( void* pBuffer, const size_t uiNumBytes );
	bool ReadString( char* pszBuffer, const size_t uiBufferSize, const bool fIsLine = false );

	bool ReadAndDiscardBits( const size_t uiNumBits );
	bool ReadAndDiscardBytes( const size_t uiNumBytes );

private:
	const char* m_pszDebugName;	//Debug name. Points to a static const string
	bool m_fOverflowed;			//Whether a read or write operation overflowed
	uint8_t* m_pData;			//Pointer to destination buffer
	size_t m_uiMaxBits;			//Size of the destination buffer, in bits
	size_t m_uiCurrentBit;		//Next bit we're reading from or writing to

private:
	CNetworkBuffer( const CNetworkBuffer& ) = delete;
	CNetworkBuffer& operator=( const CNetworkBuffer& ) = delete;
};

#endif //CNETWORKBUFFER_H