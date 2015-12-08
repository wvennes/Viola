//---------------------------------------------------------------------------
// NPCFactory.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/External/TinyXML/tinyxml.h"
#include "Engine/Core/TinyXMLHelper.hpp"
#include "Engine/Core/StringTable.hpp"
#include "Faction.hpp"
#include "BaseAIBehavior.hpp"
#include "NPC.hpp"
#include "NPCFactory.hpp"
#define STATIC

STATIC NPCFactorySet	NPCFactory::s_factorySet;


//---------------------------------------------------------------------------
NPCFactory::NPCFactory( TiXmlElement* factoryBlueprintNode )
	: m_name( "name" )
	, m_factionID( 0 )
	, m_glyph( ' ' )
	, m_movementProperties( 0 )
	, m_maxHealth( 10.f )
	, m_fieldOfViewRadius( 0.f )
	, m_color( Colors::WHITE )
{
	PopulateFromXML( factoryBlueprintNode );

	s_factorySet.insert( this );
}


//---------------------------------------------------------------------------
NPCFactory::~NPCFactory()
{
// 	for ( unsigned int index = 0; index < m_aiBehaviors.size(); ++ index )
// 	{
// 		if ( m_aiBehaviors.at( index ) != nullptr ) delete m_aiBehaviors.at( index );
// 	}
	DestroyVector( m_aiBehaviors );
	
	m_aiBehaviors.clear();
}


//---------------------------------------------------------------------------
STATIC void NPCFactory::DestroyFactories()
{
	for ( NPCFactorySet::iterator iter = s_factorySet.begin(); iter != s_factorySet.end(); ++ iter )
	{
		delete *iter;
	}

	s_factorySet.clear();
}


//---------------------------------------------------------------------------
void NPCFactory::PopulateFromXML( TiXmlElement* factoryBlueprintNode )
{
	TiXmlElement* aiBehaviors = factoryBlueprintNode->FirstChildElement( "AIBehaviors" );
	TiXmlElement* movementProperties = factoryBlueprintNode->FirstChildElement( "MovementProperties" );
	m_name = GetAttributeAsString( factoryBlueprintNode, "name" );
	m_glyph = GetAttributeAsChar( factoryBlueprintNode, "glyph" );
	m_factionID = StringID( GetAttributeAsString( factoryBlueprintNode, "faction" ) );
	m_color.r = ( unsigned char ) GetAttributeAsInt( factoryBlueprintNode, "r" );
	m_color.g = ( unsigned char ) GetAttributeAsInt( factoryBlueprintNode, "g" );
	m_color.b = ( unsigned char ) GetAttributeAsInt( factoryBlueprintNode, "b" );
	m_maxHealth = GetAttributeAsFloat( factoryBlueprintNode, "maxHealth" );
	m_baseAttackDamage = GetAttributeAsFloat( factoryBlueprintNode, "baseDamage" );
	m_fieldOfViewRadius = GetAttributeAsFloat( factoryBlueprintNode, "fieldOfViewRadius" );

	if ( aiBehaviors != nullptr )
	{
		for ( TiXmlElement* element = aiBehaviors->FirstChildElement( "AIBehavior" ); element != nullptr; element = element->NextSiblingElement( "AIBehavior" ) )
		{
			std::string name = GetAttributeAsString( element, "name" );
			AIBehaviorRegistrationMap* behaviorMap = AIBehaviorRegistration::GetBehaviorRegistrationMap();
			AIBehaviorRegistrationMap::iterator iter = behaviorMap->find( name );
			if ( iter != behaviorMap->end() )
			{
				m_aiBehaviors.push_back( iter->second->CreateBehavior( element ) );
			}
		}
	}

	if ( movementProperties != nullptr )
	{
		for ( TiXmlElement* element = movementProperties->FirstChildElement( "Movement" ); element != nullptr; element = element->NextSiblingElement( "Movement" ) )
		{
			if ( GetAttributeAsString( element, "isBlockedByAir" ) == "true" )
			{
				m_movementProperties |= IS_BLOCKED_BY_AIR;
			}
			if ( GetAttributeAsString( element, "isBlockedByDoors" ) == "true" )
			{
				m_movementProperties |= IS_BLOCKED_BY_DOORS;
			}
			if ( GetAttributeAsString( element, "isBlockedByWalls" ) == "true" )
			{
				m_movementProperties |= IS_BLOCKED_BY_WALLS;
			}
			if ( GetAttributeAsString( element, "isBlockedByWater" ) == "true" )
			{
				m_movementProperties |= IS_BLOCKED_BY_WATER;
			}
			if ( GetAttributeAsString( element, "isBlockedByFire" ) == "true" )
			{
				m_movementProperties |= IS_BLOCKED_BY_FIRE;
			}
			if ( GetAttributeAsString( element, "isSlowedByFire" ) == "true" )
			{
				m_movementProperties |= IS_SLOWED_BY_FIRE;
			}
			if ( GetAttributeAsString( element, "isSlowedByWater" ) == "true" )
			{
				m_movementProperties |= IS_SLOWED_BY_FIRE;
			}
			if ( GetAttributeAsString( element, "isImmuneToFire" ) == "true" )
			{
				m_movementProperties |= IS_IMMUNE_TO_FIRE;
			}
		}
	}
}


//---------------------------------------------------------------------------
STATIC bool NPCFactory::DoesFactoryExist( const std::string& name )
{
	for ( NPCFactorySet::iterator iter = s_factorySet.begin(); iter != s_factorySet.end(); ++ iter )
	{
		NPCFactory* factory = *iter;
		if ( name == factory->GetName() )
		{
			return true;
		}
	}

	return false;
}


//---------------------------------------------------------------------------
STATIC NPCFactory* NPCFactory::GetFactory( const std::string& name )
{
	for ( NPCFactorySet::iterator iter = s_factorySet.begin(); iter != s_factorySet.end(); ++ iter )
	{
		NPCFactory* factory = *iter;
		if ( name == factory->GetName() )
		{
			return factory;
		}
	}

	return nullptr;
}


//---------------------------------------------------------------------------
NPC* NPCFactory::SpawnNPC()
{
	NPC* npc = new NPC( m_name, m_glyph );
	npc->SetColor( m_color );
	npc->SetFaction( Faction::SpawnFaction( m_factionID ) );
	npc->SetMovementProperties( m_movementProperties );
	npc->SetMaxHealth( m_maxHealth );
	npc->SetCurrentHealth( m_maxHealth );
	npc->SetBaseAttackDamage( m_baseAttackDamage );
	npc->SetFieldOfViewRadius( m_fieldOfViewRadius );
	for ( unsigned int index = 0; index < ( unsigned int ) m_aiBehaviors.size(); ++ index )
	{
		npc->AddAIBehavior( m_aiBehaviors.at( index ) );
	}
	return npc;
}