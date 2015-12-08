//---------------------------------------------------------------------------
// StringTable.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/Utility.hpp"
#include "Engine/Core/StringTable.hpp"
#define STATIC

/////////////////////////////////////////////////////////////////////////////
struct StringTableEntry
{
	int				m_id;
	std::string		m_originalString;
};

typedef std::map< std::string, StringTableEntry >	StringTable;
static StringTable	s_theStringTable;
static int			s_nextStringID = 1;


//---------------------------------------------------------------------------
int StringID( const std::string& stringValue )
{
	std::string value( stringValue );
	ToLower( value );
	StringTable::iterator iter = s_theStringTable.find( value );
	if ( iter != s_theStringTable.end() )
	{
		return iter->second.m_id;
	}
	else
	{
		StringTableEntry newEntry;
		newEntry.m_id = s_nextStringID;
		newEntry.m_originalString = stringValue;
		s_theStringTable.insert( std::make_pair( value, newEntry ) );
		++ s_nextStringID;
		return newEntry.m_id;
	}
}


//---------------------------------------------------------------------------
const char* StringValue( const int stringID )
{
	const char* errorValue = "empty!";
	StringTable::iterator iter;
	for ( iter = s_theStringTable.begin(); iter != s_theStringTable.end(); ++ iter )
	{
		if ( iter->second.m_id == stringID ) return iter->second.m_originalString.c_str();
	}
	return errorValue;
}