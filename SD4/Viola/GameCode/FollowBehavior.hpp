//---------------------------------------------------------------------------
// FollowBehavior.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_FOLLOWBEHAVIOR_
#define _INCLUDED_FOLLOWBEHAVIOR_

#include "BaseAIBehavior.hpp"

/////////////////////////////////////////////////////////////////////////////
class FollowBehavior : public BaseAIBehavior
{
public:
	// TO DO: create from XML Node instead of string
	static BaseAIBehavior*			CreateBehaviorFromXMLNode( TiXmlElement* element );
	float							Evaluate( Map& map );
	bool							Think( Map& map );
	BaseAIBehavior*					Clone();
	~FollowBehavior() {}

protected:
	FollowBehavior( const char* name ); // TO DO: Create from XML node

protected:
	static AIBehaviorRegistration	s_followBehaviorRegistration;

	float							m_distanceThreshold;
	Vec2i							m_followTarget;
	std::string						m_leaderName;

};

#endif