//---------------------------------------------------------------------------
// ChaseBehavior.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_CHASEBEHAVIOR_
#define _INCLUDED_CHASEBEHAVIOR_

#include "BaseAIBehavior.hpp"

/////////////////////////////////////////////////////////////////////////////
class ChaseBehavior : public BaseAIBehavior
{
public:
	// TO DO: create from XML Node instead of string
	static BaseAIBehavior*			CreateBehaviorFromXMLNode( TiXmlElement* element );
	float							Evaluate( Map& map );
	bool							Think( Map& map );
	BaseAIBehavior*					Clone();
	~ChaseBehavior() {}

protected:
	ChaseBehavior( const char* name ); // TO DO: Create from XML node

protected:
	static AIBehaviorRegistration	s_chaseBehaviorRegistration;
	Vec2i							m_chaseTarget;
	FieldOfViewResult				m_fieldOfViewResult;

};

#endif