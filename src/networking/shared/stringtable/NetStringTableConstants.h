#ifndef NETWORKING_STRINGTABLE_NETSTRINGTABLECONSTANTS_H
#define NETWORKING_STRINGTABLE_NETSTRINGTABLECONSTANTS_H

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
}

/**
*	Short alias for the NetStringTable namespace.
*/
namespace NST = NetStringTable;

#endif //NETWORKING_STRINGTABLE_NETSTRINGTABLECONSTANTS_H