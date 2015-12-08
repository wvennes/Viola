//---------------------------------------------------------------------------
// DrainBehavior.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_DRAINBEHAVIOR_
#define _INCLUDED_DRAINBEHAVIOR_

#include "BaseAIBehavior.hpp"

/////////////////////////////////////////////////////////////////////////////
class DrainBehavior : public BaseAIBehavior
{
public:
	// TO DO: create from XML Node instead of string
	static BaseAIBehavior*			CreateBehaviorFromXMLNode( TiXmlElement* element );
	float							Evaluate( Map& map );
	bool							Think( Map& map );
	BaseAIBehavior*					Clone();
	~DrainBehavior() {}

protected:
	DrainBehavior( const char* name ); // TO DO: Create from XML node

protected:
	static AIBehaviorRegistration	s_drainBehaviorRegistration;
	int								m_minDrain;
	int								m_maxDrain;
	Vec2i							m_targetLocation;

};

#endif