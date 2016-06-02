#ifndef UTILITY_CAUTOSTRING_H
#define UTILITY_CAUTOSTRING_H

#include <cstring>
#include <functional>

#include "StringUtils.h"

/**
*	String class that automatically manages memory
*	Intended for use in containers
*/
class CAutoString final
{
public:
	/**
	*	Default constructor.
	*/
	CAutoString()
		: m_pszString( nullptr )
	{
	}

	/**
	*	Constructs a string containing the given string.
	*	@param pszString String to set.
	*/
	explicit CAutoString( const char* pszString )
		: m_pszString( nullptr )
	{
		Set( pszString );
	}

	/**
	*	Copy constructor.
	*/
	CAutoString( const CAutoString& other )
		: m_pszString( nullptr )
	{
		Set( other.Get() );
	}

	/**
	*	Move constructor.
	*/
	CAutoString( CAutoString&& other )
		: m_pszString( other.m_pszString )
	{
		other.m_pszString = nullptr;
	}

	/**
	*	Destructor.
	*/
	~CAutoString()
	{
		delete[] m_pszString;
	}

	/**
	*	@return Whether the string is null.
	*/
	bool IsEmpty() const { return m_pszString == nullptr; }

	/**
	*	@return The string. Always returns a valid pointer.
	*/
	const char* Get() const
	{
		return m_pszString ? m_pszString : "";
	}

	/**
	*	@see Get() const
	*/
	operator const char*() const { return Get(); }

	/**
	*	Sets the string.
	*	@param pszString String to set. Can be null.
	*/
	void Set( const char* pszString );

	/**
	*	Compares the string with the given string.
	*/
	int Compare( const char* pszString ) const;

	/**
	*	Operator less than.
	*/
	bool operator<( const char* pszString ) const { return Compare( pszString ) < 0; }

	/**
	*	@copydoc operator<( const char* pszString ) const
	*/
	bool operator<( const CAutoString& other ) const { return Compare( other.m_pszString ) < 0; }

	/**
	*	Operator equals.
	*/
	bool operator==( const char* pszString ) const { return Compare( pszString ) == 0; }

	/**
	*	@copydoc operator==( const char* pszString ) const
	*/
	bool operator==( const CAutoString& other ) const { return Compare( other.m_pszString ) == 0; }

	/**
	*	Operator not equal.
	*/
	bool operator!=( const char* pszString ) const { return Compare( pszString ) != 0; }

	/**
	*	@copydoc operator!=( const CAutoString& other ) const
	*/
	bool operator!=( const CAutoString& other ) const { return Compare( other.m_pszString ) != 0; }

	/**
	*	Assignment operator.
	*/
	CAutoString& operator=( const char* pszString ) { Set( pszString ); return *this; }

	/**
	*	Assignment operator.
	*/
	CAutoString& operator=( const CAutoString& other )
	{
		if( this != &other )
		{
			Set( other.m_pszString );
		}

		return *this;
	}

	/**
	*	Move assignment operator.
	*/
	CAutoString& operator=( CAutoString&& other )
	{
		if( this != &other )
		{
			Set( nullptr );

			m_pszString = other.m_pszString;
			other.m_pszString = nullptr;
		}

		return *this;
	}

private:
	char* m_pszString;
};

inline void CAutoString::Set( const char* pszString )
{
	if( pszString != m_pszString )
	{
		delete[] m_pszString;

		if( pszString )
		{
			const size_t uiLength = strlen( pszString );
			m_pszString = new char[ uiLength + 1 ];

			strcpy( m_pszString, pszString );
		}
		else
			m_pszString = nullptr;
	}
}

inline int CAutoString::Compare( const char* pszString ) const
{
	if( !pszString )
		return m_pszString ? 1 : 0;

	if( !m_pszString )
		return -1;

	return strcmp( m_pszString, pszString );
}

namespace std
{
template<>
struct hash<CAutoString> : public std::unary_function<CAutoString, size_t>
{
	std::size_t operator()( const CAutoString& szStr ) const
	{
		return StringHash( szStr.Get() );
	}
};
}

#endif //UTILITY_CAUTOSTRING_H