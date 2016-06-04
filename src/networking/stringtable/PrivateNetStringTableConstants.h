#ifndef NETWORKING_STRINGTABLE_PRIVATENETSTRINGTABLECONSTANTS_H
#define NETWORKING_STRINGTABLE_PRIVATENETSTRINGTABLECONSTANTS_H

#include "networking/shared/stringtable/NetStringTableConstants.h"

/**
*	@ingroup NetStringTable
*
*	@{
*/

namespace NetStringTable
{
/**
*	Converts a table ID to an index.
*/
inline size_t TableIDToIndex( const TableID_t ID )
{
	if( ID == INVALID_TABLE_ID )
		return -1;

	return ID - 1;
}

/**
*	Converts an index to a table ID.
*/
inline TableID_t IndexToTableID( const size_t uiIndex )
{
	if( uiIndex == -1 )
		return INVALID_TABLE_ID;

	return uiIndex + 1;
}

/**
*	Data serialization results.
*/
enum class SerializeResult
{
	/**
	*	Data was written.
	*/
	WROTEDATA = 0,

	/**
	*	No data was written.
	*/
	WROTENOTHING,

	/**
	*	Buffer overflow occurred during writing.
	*/
	OVERFLOWED
};
}

/** @} */

#endif //NETWORKING_STRINGTABLE_PRIVATENETSTRINGTABLECONSTANTS_H