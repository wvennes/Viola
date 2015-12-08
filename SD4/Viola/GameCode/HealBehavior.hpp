//---------------------------------------------------------------------------
// HealBehavior.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_HEALBEHAVIOR_
#define _INCLUDED_HEALBEHAVIOR_

#include "BaseAIBehavior.hpp"

/////////////////////////////////////////////////////////////////////////////
class HealBehavior : public BaseAIBehavior
{
public:
	// TO DO: create from XML Node instead of string
	static BaseAIBehavior*			CreateBehaviorFromXMLNode( TiXmlElement* element );
	float							Evaluate( Map& map );
	bool							Think( Map& map );
	BaseAIBehavior*					Clone();
	~HealBehavior() {}

protected:
	HealBehavior( const char* name ); // TO DO: Create from XML node

protected:
	static AIBehaviorRegistration	s_healBehaviorRegistration;

	int								m_minHeal;
	int								m_maxHeal;
	int								m_healThreshold;
	Vec2i							m_targetLocation;

};

#endif