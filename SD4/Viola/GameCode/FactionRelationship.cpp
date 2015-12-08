//---------------------------------------------------------------------------
// FactionRelationship.cpp
//---------------------------------------------------------------------------

#include "FactionRelationship.hpp"


//---------------------------------------------------------------------------
FactionRelationship::FactionRelationship()
	: m_factionID( 0 )
	, m_value( 0 )
{

}


//---------------------------------------------------------------------------
FactionRelationship::FactionRelationship( int id, int value )
	: m_factionID( id )
	, m_value( value )
{

}


//---------------------------------------------------------------------------
FactionRelationship::FactionRelationship( const FactionRelationship& otherFactionRelationship )
	: m_factionID( otherFactionRelationship.GetFactionID() )
	, m_value( otherFactionRelationship.GetValue() )
{

}


//---------------------------------------------------------------------------
FactionRelationship::~FactionRelationship()
{

}