//---------------------------------------------------------------------------
// WanderBehavior.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_WANDERBEHAVIOR_
#define _INCLUDED_WANDERBEHAVIOR_

#include "BaseAIBehavior.hpp"

/////////////////////////////////////////////////////////////////////////////
class WanderBehavior : public BaseAIBehavior
{
public:
	// TO DO: create from XML Node instead of string
	static BaseAIBehavior*			CreateBehaviorFromXMLNode( TiXmlElement* element );
	float							Evaluate( Map& map );
	bool							Think( Map& map );
	BaseAIBehavior*					Clone();
	~WanderBehavior() {}

protected:
	WanderBehavior( const char* name ); // TO DO: Create from XML node

protected:
	static AIBehaviorRegistration	s_wanderBehaviorRegistration;
	float							m_chanceToRest;

};

#endif