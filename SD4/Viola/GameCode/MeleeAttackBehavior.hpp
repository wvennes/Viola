//---------------------------------------------------------------------------
// MeleeAttackBehavior.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_MELEEATTACKBEHAVIOR_
#define _INCLUDED_MELEEATTACKBEHAVIOR_

#include "BaseAIBehavior.hpp"

/////////////////////////////////////////////////////////////////////////////
class MeleeAttackBehavior : public BaseAIBehavior
{
public:
	// TO DO: create from XML Node instead of string
	static BaseAIBehavior*			CreateBehaviorFromXMLNode( TiXmlElement* element );
	float							Evaluate( Map& map );
	bool							Think( Map& map );
	BaseAIBehavior*					Clone();
	~MeleeAttackBehavior() {}

protected:
	MeleeAttackBehavior( const char* name ); // TO DO: Create from XML node

protected:
	static AIBehaviorRegistration	s_meleeAttackBehaviorRegistration;
	Vec2i							m_targetLocation;
	float							m_attackStrength;

};

#endif