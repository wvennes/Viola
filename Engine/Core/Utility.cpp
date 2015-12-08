//---------------------------------------------------------------------------
// Utility.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/Utility.hpp"


//---------------------------------------------------------------------------
void ToLower( std::string& value )
{
	std::transform( value.begin(), value.end(), value.begin(), ::tolower );
}