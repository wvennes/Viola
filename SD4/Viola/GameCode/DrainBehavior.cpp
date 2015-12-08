//---------------------------------------------------------------------------
// DrainBehavior.cpp
//---------------------------------------------------------------------------

#include "Engine/External/TinyXML/tinyxml.h"
#include "Engine/Core/TinyXMLHelper.hpp"
#include "Engine/Math/GameMath.hpp"
#include "CombatSystem.hpp"
#include "Faction.hpp"
#include "Agent.hpp"
#include "MessageBar.hpp"
#include "DrainBehavior.hpp"
#define STATIC

STATIC AIBehaviorRegistration DrainBehavior::s_drainBehaviorRegistration( "drainLife", CreateBehaviorFromXMLNode );


//---------------------------------------------------------------------------
DrainBehavior::DrainBehavior( const char* name )
	: BaseAIBehavior( name )
	, m_minDrain( 0 )
	, m_maxDrain( 0 )
	, m_targetLocation( Vec2i( -1, -1 ) )
{

}


//---------------------------------------------------------------------------
BaseAIBehavior* DrainBehavior::CreateBehaviorFromXMLNode( TiXmlElement* element )
{
	std::string name = GetAttributeAsString( element, "name" );
	DrainBehavior* behavior = new DrainBehavior( name.c_str() );
	behavior->m_minDrain = GetAttributeAsInt( element, "minDrain" );
	behavior->m_maxDrain = GetAttributeAsInt( element, "maxDrain" );
	//behavior->m_chanceToRest = GetAttributeAsFloat( element, "chanceToRest" );
	return behavior;
}


//---------------------------------------------------------------------------
float DrainBehavior::Evaluate( Map& map )
{
	if ( m_agent == nullptr ) return 0.f;

	std::vector< CellCoords > surroundingCoords;
	map.GetSurroundingNonBlockedCoordinates( m_agent->GetPosition(), m_agent->GetMovementPropertes(), surroundingCoords );

	Agent* currentAgent = nullptr;
	Agent* agentToAttack = nullptr;

	for ( unsigned int index = 0; index < ( unsigned int ) surroundingCoords.size(); ++ index )
	{
		Cell* currentCell = map.GetCell( surroundingCoords.at( index ) );

		if ( currentCell != nullptr && currentCell->IsCellOccupiedByAgent() )
		{
			currentAgent = currentCell->GetOccupyingAgent();
			int currentRelationshipValue = m_agent->GetFaction()->GetRelationshipValue( currentAgent->GetFaction()->GetFactionNameID() );

			if ( m_agent->GetFaction()->GetRelationshipValue( currentAgent->GetFaction()->GetFactionNameID() ) < 0 )
			{
				if ( agentToAttack == nullptr )
				{
					agentToAttack = currentAgent;
				}
				else
				{
					int lastRelationshipValue = m_agent->GetFaction()->GetRelationshipValue( agentToAttack->GetFaction()->GetFactionNameID() );
					if ( currentRelationshipValue < lastRelationshipValue ) agentToAttack = currentAgent;
				}
			}
		}
	}

	if ( agentToAttack != nullptr )
	{
		m_targetLocation = agentToAttack->GetPosition();
		float healthLost = agentToAttack->GetMaxHealth() - agentToAttack->GetCurrentHealth();
		return 5.f * healthLost + 1.f;
	}

	m_targetLocation = Vec2i( -1, -1 );
	return 0.f;
}


//---------------------------------------------------------------------------
bool DrainBehavior::Think( Map& map )
{
	if ( m_targetLocation.x < 0 || m_targetLocation.y < 0 ) return false;

	Cell* targetCell = map.GetCell( m_targetLocation );
	if ( targetCell == nullptr ) return false;

	Agent* currentAgent = targetCell->GetOccupyingAgent();
	int damage = GetRandomIntInRange( m_minDrain, m_maxDrain );

	CombatInformation combatInfo;
	combatInfo.m_attackingEntity = m_agent;
	combatInfo.m_defendingEntity =currentAgent;
	combatInfo.m_specificDamageToApply = ( float ) damage;
	CombatSystem::ApplyAttackAndGetResult( combatInfo );

	map.SendFactionAttackEvent( m_agent, currentAgent );

	float lastCurrentHealth = m_agent->GetCurrentHealth();
	m_agent->ApplyDamage( ( float ) -damage );

	if ( targetCell->IsCellVisible() )
	{
		g_gameMessageLog->LogPrintf( Colors::RED, "The %s touches you!", combatInfo.m_attackingEntity->GetName().c_str() );
		g_gameMessageLog->LogPrintf( Colors::RED, "You feel drained." );
		if ( lastCurrentHealth < m_agent->GetCurrentHealth() )
		{
			g_gameMessageLog->LogPrintf( Colors::RED, "The %s appears stronger.", m_agent->GetName().c_str() );
		}
	}

	return true;
}


//---------------------------------------------------------------------------
BaseAIBehavior* DrainBehavior::Clone()
{
	return new DrainBehavior( *this );
}