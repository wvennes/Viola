//---------------------------------------------------------------------------
// HealBehavior.cpp
//---------------------------------------------------------------------------

#include "Engine/External/TinyXML/tinyxml.h"
#include "Engine/Core/TinyXMLHelper.hpp"
#include "Engine/Math/GameMath.hpp"
#include "CombatSystem.hpp"
#include "Faction.hpp"
#include "Agent.hpp"
#include "MessageBar.hpp"
#include "HealBehavior.hpp"
#define STATIC

STATIC AIBehaviorRegistration HealBehavior::s_healBehaviorRegistration( "heal", CreateBehaviorFromXMLNode );


//---------------------------------------------------------------------------
HealBehavior::HealBehavior( const char* name )
	: BaseAIBehavior( name )
	, m_minHeal( 0 )
	, m_maxHeal( 0 )
	, m_healThreshold( 0 )
	, m_targetLocation( Vec2i( -1, -1 ) )
{

}


//---------------------------------------------------------------------------
BaseAIBehavior* HealBehavior::CreateBehaviorFromXMLNode( TiXmlElement* element )
{
	std::string name = GetAttributeAsString( element, "name" );
	HealBehavior* behavior = new HealBehavior( name.c_str() );
	behavior->m_minHeal = GetAttributeAsInt( element, "minHeal" );
	behavior->m_maxHeal = GetAttributeAsInt( element, "maxHeal" );
	behavior->m_healThreshold = GetAttributeAsInt( element, "healThreshold" );
	//behavior->m_chanceToRest = GetAttributeAsFloat( element, "chanceToRest" );
	return behavior;
}


//---------------------------------------------------------------------------
float HealBehavior::Evaluate( Map& map )
{
	if ( m_agent == nullptr ) return 0.f;

	std::vector< CellCoords > surroundingCoords;
	map.GetSurroundingNonBlockedCoordinates( m_agent->GetPosition(), m_agent->GetMovementPropertes(), surroundingCoords );

	Agent* currentAgent = nullptr;
	Agent* agentToHeal = nullptr;

	for ( unsigned int index = 0; index < ( unsigned int ) surroundingCoords.size(); ++ index )
	{
		Cell* currentCell = map.GetCell( surroundingCoords.at( index ) );

		if ( currentCell == nullptr || !currentCell->IsCellOccupiedByAgent() ) continue;

		currentAgent = currentCell->GetOccupyingAgent();
		int currentRelationshipValue = m_agent->GetFaction()->GetRelationshipValue( currentAgent->GetFaction()->GetFactionNameID() );

		if ( currentRelationshipValue < 0 ) continue; // ally or enemy?
		if ( currentAgent->GetMaxHealth() - currentAgent->GetCurrentHealth() < ( float ) m_healThreshold ) continue; // less than threshold?

		if ( agentToHeal != nullptr && ( agentToHeal->GetCurrentHealth() < currentAgent->GetCurrentHealth() ) ) // less health than last ally?
		{
			agentToHeal = currentAgent;
		}
		else if ( agentToHeal == nullptr ) // first ally?
		{
			agentToHeal = currentAgent;
		}
	}

	// if no agents found, check self
	if ( agentToHeal == nullptr )
	{
		agentToHeal = m_agent;
	}
	

	float missingHealth = agentToHeal->GetMaxHealth() - agentToHeal->GetCurrentHealth();

	if ( missingHealth > ( float ) m_healThreshold )
	{
		m_targetLocation = agentToHeal->GetPosition();
		return 4.f * missingHealth;
	}

	m_targetLocation = Vec2i( -1, -1 );
	return 0.f;

// 	m_targetLocation = Vec2i( -1, -1 );
// 	return 0.f;
}


//---------------------------------------------------------------------------
bool HealBehavior::Think( Map& map )
{
	if ( m_targetLocation.x < 0 || m_targetLocation.y < 0 ) return false;

	Cell* targetCell = map.GetCell( m_targetLocation );
	if ( targetCell == nullptr ) return false;

	int heal = GetRandomIntInRange( m_minHeal, m_maxHeal );

	CombatInformation combatInfo;
	combatInfo.m_attackingEntity = m_agent;
	combatInfo.m_defendingEntity = targetCell->GetOccupyingAgent();
	combatInfo.m_specificDamageToApply = ( float ) ( -1 * heal );
	
	CombatSystem::ApplyAttackAndGetResult( combatInfo );

	if ( targetCell->IsCellVisible() )
	{
		g_gameMessageLog->LogPrintf( Colors::GREEN, "The %s healed the %s for %i health!", combatInfo.m_attackingEntity->GetName().c_str(), combatInfo.m_defendingEntity->GetName().c_str(), ( int ) -combatInfo.m_damageDealt_out );
	}

	return true;
}


//---------------------------------------------------------------------------
BaseAIBehavior* HealBehavior::Clone()
{
	return new HealBehavior( *this );
}