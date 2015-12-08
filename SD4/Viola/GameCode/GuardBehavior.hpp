//---------------------------------------------------------------------------
// GuardBehavior.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_GUARDBEHAVIOR_
#define _INCLUDED_GUARDBEHAVIOR_

#include "BaseAIBehavior.hpp"

/////////////////////////////////////////////////////////////////////////////
class GuardBehavior : public BaseAIBehavior
{
public:
	// TO DO: create from XML Node instead of string
	static BaseAIBehavior*			CreateBehaviorFromXMLNode( TiXmlElement* element );
	float							Evaluate( Map& map );
	bool							Think( Map& map );
	BaseAIBehavior*					Clone();
	~GuardBehavior() {}

protected:
	GuardBehavior( const char* name ); // TO DO: Create from XML node

protected:
	static AIBehaviorRegistration	s_guardBehaviorRegistration;
	bool							m_hasLocationBeenSet;
	bool							m_isGuardGoingHome;
	float							m_maxTravelLength;
	Vec2i							m_guardLocation;
	std::vector< Vec2i >			m_currentPath;

};

#endif