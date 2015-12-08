//---------------------------------------------------------------------------
// FactionRelationship.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_FACTIONRELATIONSHIP_
#define _INCLUDED_FACTIONRELATIONSHIP_

/////////////////////////////////////////////////////////////////////////////
class FactionRelationship
{

public:
	FactionRelationship();
	FactionRelationship( int id, int value );
	FactionRelationship( const FactionRelationship& otherFactionRelationship );
	~FactionRelationship();

	int		GetFactionID() const { return m_factionID; }
	int		GetValue() const { return m_value; }
	void	SetValue( int value ) { m_value = value; }
	void	AdjustRelationshipValue( int deltaValue ) { m_value += deltaValue; }

private:
	int		m_factionID;
	int		m_value;

};

#endif