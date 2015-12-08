//---------------------------------------------------------------------------
// BaseAIBehavior.cpp
//---------------------------------------------------------------------------

#include "BaseAIBehavior.hpp"
#define STATIC

STATIC AIBehaviorRegistrationMap* AIBehaviorRegistration::s_behaviorRegistrationMap = new AIBehaviorRegistrationMap();


//---------------------------------------------------------------------------
STATIC void BaseAIBehavior::DestroyAIBehaviorRegistrationMap()
{
	delete AIBehaviorRegistration::GetBehaviorRegistrationMap();
}


//---------------------------------------------------------------------------
STATIC BaseAIBehavior* BaseAIBehavior::CreateBehaviorFromXMLNode( const char* name, TiXmlElement* element )
{
	AIBehaviorRegistrationMap* behaviorRegistrationMap = AIBehaviorRegistration::GetBehaviorRegistrationMap();

	if ( behaviorRegistrationMap )
	{
		return behaviorRegistrationMap->find( name )->second->CreateBehavior( element );
	}

	return nullptr;
}


//---------------------------------------------------------------------------
void BaseAIBehavior::SetOwningAgent( Agent* agent )
{
	m_agent = agent;
}