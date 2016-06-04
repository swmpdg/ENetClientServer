#ifndef NETWORKING_STRINGTABLE_NETSTRINGTABLECONSTANTS_H
#define NETWORKING_STRINGTABLE_NETSTRINGTABLECONSTANTS_H

class INetworkStringTable;

/**
*	@defgroup NetStringTable Network String Table
*
*	@{
*/

namespace NetStringTable
{
/**
*	Invalid string index.
*/
const size_t INVALID_STRING_INDEX = -1;

/**
*	Type that represents a single table.
*/
typedef size_t TableID_t;

/**
*	Invalid table ID.
*/
const TableID_t INVALID_TABLE_ID = 0;

/**
*	Callback for client side table additions.
*	@param pObject User data.
*	@param pTable Table that the string was added to.
*	@param uiIndex Index of the string that was added.
*/
using StringAddedCallback = void ( * )( void* pObject, INetworkStringTable* pTable, const size_t uiIndex );
}

/**
*	Short alias for the NetStringTable namespace.
*/
namespace NST = NetStringTable;

/** @} */

#endif //NETWORKING_STRINGTABLE_NETSTRINGTABLECONSTANTS_H