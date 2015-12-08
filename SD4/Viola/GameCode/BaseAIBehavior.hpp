//---------------------------------------------------------------------------
// BaseAIBehavior.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_BASEAIBEHAVIOR_
#define _INCLUDED_BASEAIBEHAVIOR_

#include <string>
#include <map>
#include "Map.hpp"

class AIBehaviorRegistration;
class BaseAIBehavior;
class Agent;
class TiXmlElement;
class MessageBar;

typedef BaseAIBehavior* ( AICreationFunc ) ( TiXmlElement* element );
typedef std::map< std::string, AIBehaviorRegistration* > AIBehaviorRegistrationMap;

/////////////////////////////////////////////////////////////////////////////
class AIBehaviorRegistration
{
public:
	AIBehaviorRegistration( const char* name, AICreationFunc* creationFunc )
		: m_name( name )
		, m_creationFunc( creationFunc )
	{
		if ( s_behaviorRegistrationMap == nullptr ) s_behaviorRegistrationMap = new AIBehaviorRegistrationMap();

		s_behaviorRegistrationMap->insert( std::pair< std::string, AIBehaviorRegistration* >( name, this ) );
	}

	static AIBehaviorRegistrationMap*	GetBehaviorRegistrationMap() { return s_behaviorRegistrationMap; }
	std::string							GetName() { return m_name; }
	BaseAIBehavior*						CreateBehavior( TiXmlElement* element ) { return m_creationFunc( element ); }

protected:
	std::string							m_name;
	AICreationFunc*						m_creationFunc;
	static AIBehaviorRegistrationMap*	s_behaviorRegistrationMap;

};


////////////////////////////////////////////////////////////////////////////
class BaseAIBehavior
{
public:
	std::string				m_name;

public:
	BaseAIBehavior( const char* name ) : m_name( name ), m_agent( nullptr ) {}

	static void				DestroyAIBehaviorRegistrationMap();

	// TO DO!!! - Pass in a TinyXML node here and parse out the information
	static BaseAIBehavior*	CreateBehaviorFromXMLNode( const char* name, TiXmlElement* xmlElement );
	virtual bool			Think( Map& map ) = 0;
	virtual float			Evaluate( Map& map ) = 0;
	virtual BaseAIBehavior*	Clone() = 0;

	void					SetOwningAgent( Agent* agent );

protected:
	Agent*					m_agent;

};

extern MessageBar* g_gameMessageLog;

#endif