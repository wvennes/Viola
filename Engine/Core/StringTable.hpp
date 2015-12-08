//---------------------------------------------------------------------------
// StringTable.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_STRINGTABLE_
#define _INCLUDED_STRINGTABLE_

#include <string>
#include <map>

int				StringID( const std::string& stringValue );
const char*		StringValue( const int stringID );

#endif