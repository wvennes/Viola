//---------------------------------------------------------------------------
// SpawnNPCBehavior.cpp
//---------------------------------------------------------------------------

#include "SpawnNPCBehavior.hpp"
#include "Engine/External/TinyXML/tinyxml.h"
#include "Engine/Core/TinyXMLHelper.hpp"
#include "MessageBar.hpp"
#include "NPCFactory.hpp"
#include "NPC.hpp"
#include "Agent.hpp"
#include "Faction.hpp"
#define STATIC
#define UNUSED( x ) ( void )( x )

STATIC AIBehaviorRegistration SpawnNPCBehavior::s_spawnNPCBehaviorRegistration( "spawnNPC", &SpawnNPCBehavior::CreateBehaviorFromXMLNode );

//---------------------------------------------------------------------------
SpawnNPCBehavior::SpawnNPCBehavior( const char* name )
	: BaseAIBehavior( name )
	, m_spawnThreshold( 0 )
	, m_chanceToSpawnNPC( 0.f )
	, m_npcName( "" )
{

}


//---------------------------------------------------------------------------
BaseAIBehavior* SpawnNPCBehavior::CreateBehaviorFromXMLNode( TiXmlElement* element )
{
	std::string name = GetAttributeAsString( element, "name" );
	SpawnNPCBehavior* behavior = new SpawnNPCBehavior( name.c_str() );

	behavior->m_spawnThreshold = GetAttributeAsInt( element, "spawnThreshold" );
	behavior->m_chanceToSpawnNPC = GetAttributeAsFloat( element, "chanceToSpawn" );
	behavior->m_npcName = GetAttributeAsString( element, "npcName" );

	return behavior;
}


//---------------------------------------------------------------------------
BaseAIBehavior* SpawnNPCBehavior::Clone()
{
	return new SpawnNPCBehavior( *this );
}


//---------------------------------------------------------------------------
float SpawnNPCBehavior::Evaluate( Map& map )
{
	UNUSED( map );
	bool hasSeenEnemy = false;
	const FieldOfViewResult* result = m_agent->GetFieldOfViewResult();
	Agent* currentAgent = nullptr;
	int numberOfSpawnedNPCs = 0;

	EntitySet::iterator iter;
	for ( iter = result->m_entitiesHit.begin(); iter != result->m_entitiesHit.end(); ++ iter )
	{
		currentAgent = dynamic_cast< Agent* >( *iter );
		if ( currentAgent == nullptr ) continue;

		if ( currentAgent->GetName() == m_npcName ) ++ numberOfSpawnedNPCs;

		if ( m_agent->GetFaction()->GetRelationshipValue( currentAgent->GetFaction()->GetFactionNameID() ) < 0 ) hasSeenEnemy = true;
	}

	if ( !hasSeenEnemy ) return 0.f;

	return ( ( float ) m_spawnThreshold * 5.f ) - ( ( float ) numberOfSpawnedNPCs * 5.f );
}


//---------------------------------------------------------------------------
bool SpawnNPCBehavior::Think( Map& map )
{
	float percentage = ( float ) rand() / ( float ) RAND_MAX;
	if ( percentage < m_chanceToSpawnNPC ) return false;

	int count = 0;
	bool canPlaceNPC = false;
	std::vector< Vec2i > cellCoordsAroundAgent;
	map.GetSurroundingNonBlockedCoordinates( m_agent->GetPosition(), m_agent->GetMovementPropertes(), cellCoordsAroundAgent );
	if ( cellCoordsAroundAgent.empty() ) return false;

	int index = 0;
	Cell* cell = nullptr;

	while ( !canPlaceNPC && count < 20 )
	{
		index = rand() % ( unsigned int ) cellCoordsAroundAgent.size();
		cell = map.GetCell( cellCoordsAroundAgent[ index ] );
		if ( cell == nullptr ) return false;

		if ( !cell->IsCellOccupiedByAgent() )
		{
			NPC* newNPC = NPCFactory::GetFactory( m_npcName )->SpawnNPC();
			newNPC->SetPosition( cellCoordsAroundAgent[ index ] );
			newNPC->AddToMap( &map );
			canPlaceNPC = true;

			g_gameMessageLog->LogPrintf( Colors::YELLOW, "The %s summons a %s to fight for him!", m_agent->GetName().c_str(), m_npcName.c_str() );
		}

		++ count;
	}

	return true;
}