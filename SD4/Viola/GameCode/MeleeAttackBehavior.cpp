//---------------------------------------------------------------------------
// MeleeAttackBehavior.cpp
//---------------------------------------------------------------------------

#include "Engine/External/TinyXML/tinyxml.h"
#include "Engine/Core/TinyXMLHelper.hpp"
#include "CombatSystem.hpp"
#include "Faction.hpp"
#include "Agent.hpp"
#include "MessageBar.hpp"
#include "MeleeAttackBehavior.hpp"
#define STATIC

STATIC AIBehaviorRegistration MeleeAttackBehavior::s_meleeAttackBehaviorRegistration( "meleeAttack", CreateBehaviorFromXMLNode );


//---------------------------------------------------------------------------
MeleeAttackBehavior::MeleeAttackBehavior( const char* name )
	: BaseAIBehavior( name )
	, m_targetLocation( Vec2i( -1, -1 ) )
	, m_attackStrength( 0.f )
{

}


//---------------------------------------------------------------------------
BaseAIBehavior* MeleeAttackBehavior::CreateBehaviorFromXMLNode( TiXmlElement* element )
{
	std::string name = GetAttributeAsString( element, "name" );
	MeleeAttackBehavior* behavior = new MeleeAttackBehavior( name.c_str() );
	behavior->m_attackStrength = GetAttributeAsFloat( element, "damage" );
	//behavior->m_chanceToRest = GetAttributeAsFloat( element, "chanceToRest" );
	return behavior;
}


//---------------------------------------------------------------------------
float MeleeAttackBehavior::Evaluate( Map& map )
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
		return 30.f;
	}

	m_targetLocation = Vec2i( -1, -1 );
	return 0.f;
}


//---------------------------------------------------------------------------
bool MeleeAttackBehavior::Think( Map& map )
{
	if ( m_targetLocation.x < 0 || m_targetLocation.y < 0 ) return false;

	Cell* targetCell = map.GetCell( m_targetLocation );
	if ( targetCell == nullptr ) return false;
	CombatInformation combatInfo;
	combatInfo.m_attackingEntity = m_agent;
	combatInfo.m_attackType = MELEE;
	combatInfo.m_attackingWeapon = m_agent->GetEquippedWeapon();
	combatInfo.m_defendingEntity = targetCell->GetOccupyingAgent();
	CombatSystem::ApplyAttackAndGetResult( combatInfo );

	map.SendFactionAttackEvent( m_agent, targetCell->GetOccupyingAgent() );

	if ( targetCell->IsCellVisible() )
	{
		g_gameMessageLog->LogPrintf( Colors::RED, "The %s attacked the %s for %i damage!", combatInfo.m_attackingEntity->GetName().c_str(), combatInfo.m_defendingEntity->GetName().c_str(), ( int ) combatInfo.m_damageDealt_out );
	}

	return true;
}


//---------------------------------------------------------------------------
BaseAIBehavior* MeleeAttackBehavior::Clone()
{
	return new MeleeAttackBehavior( *this );
}