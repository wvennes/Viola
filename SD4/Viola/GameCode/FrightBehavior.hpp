//---------------------------------------------------------------------------
// FrightBehavior.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_FRIGHTBEHAVIOR_
#define _INCLUDED_FRIGHTBEHAVIOR_

#include <set>
#include "BaseAIBehavior.hpp"


/////////////////////////////////////////////////////////////////////////////
class FrightBehavior : public BaseAIBehavior
{
public:
	// TO DO: create from XML Node instead of string
	static BaseAIBehavior*			CreateBehaviorFromXMLNode( TiXmlElement* element );
	float							Evaluate( Map& map );
	bool							Think( Map& map );
	BaseAIBehavior*					Clone();
	~FrightBehavior();

protected:
	FrightBehavior( const char* name ); // TO DO: Create from XML node

protected:
	static AIBehaviorRegistration	s_frightBehaviorRegistration;
	Vec2i							m_scaredOfLocation;
	std::set< std::string >			m_frightenedTargets;

};

#endif