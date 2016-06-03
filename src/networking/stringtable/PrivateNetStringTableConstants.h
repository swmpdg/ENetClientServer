#ifndef NETWORKING_STRINGTABLE_PRIVATE_NETSTRINGTABLECONSTANTS_H
#define NETWORKING_STRINGTABLE_PRIVATE_NETSTRINGTABLECONSTANTS_H

#include "networking/shared/stringtable/NetStringTableConstants.h"

#undef OVERFLOW

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

enum class SerializeResult
{
	WROTEDATA = 0,
	WROTENOTHING,
	OVERFLOW
};
}

#endif //NETWORKING_STRINGTABLE_PRIVATE_NETSTRINGTABLECONSTANTS_H