//---------------------------------------------------------------------------
// ChaseBehavior.cpp
//---------------------------------------------------------------------------

#include "Engine/External/TinyXML/tinyxml.h"
#include "Engine/Core/TinyXMLHelper.hpp"
#include "PathFinder.hpp"
#include "Faction.hpp"
#include "Agent.hpp"
#include "ChaseBehavior.hpp"
#define STATIC
#define UNUSED(x) (void)(x)

STATIC AIBehaviorRegistration ChaseBehavior::s_chaseBehaviorRegistration( "chase", CreateBehaviorFromXMLNode );


//---------------------------------------------------------------------------
ChaseBehavior::ChaseBehavior( const char* name )
	: BaseAIBehavior( name )
	, m_chaseTarget( Vec2i( -1, -1 ) )
{

}


//---------------------------------------------------------------------------
BaseAIBehavior* ChaseBehavior::CreateBehaviorFromXMLNode( TiXmlElement* element )
{
	std::string name = GetAttributeAsString( element, "name" );
	ChaseBehavior* behavior = new ChaseBehavior( name.c_str() );
	return behavior;
}


//---------------------------------------------------------------------------
float ChaseBehavior::Evaluate( Map& map )
{
	UNUSED( map );
	//m_fieldOfViewResult.m_cellsHit.clear();
	//float score = 0.f;
	if ( m_agent == nullptr ) return 0.f;

	Agent* agentToChase = nullptr;
	Agent* currentAgent = nullptr;
	const FieldOfViewResult* result = m_agent->GetFieldOfViewResult();
	//m_fieldOfViewResult = m_agent->GetFieldOfViewResult( &map, m_fieldOfViewRadius );
	//FieldOfViewResult m_fieldOfViewResult = m_agent->GetFieldOfViewResult( &map, m_fieldOfViewRadius );

	EntitySet::iterator iter;
	for ( iter = result->m_entitiesHit.begin(); iter != result->m_entitiesHit.end(); ++ iter )
	{
		currentAgent = dynamic_cast< Agent* >( *iter );
		if ( currentAgent == nullptr ) continue;
		int currentRelationshipValue = m_agent->GetFaction()->GetRelationshipValue( currentAgent->GetFaction()->GetFactionNameID() );

		if ( m_agent->GetFaction()->GetRelationshipValue( currentAgent->GetFaction()->GetFactionNameID() ) < 0 )
		{
			if ( agentToChase == nullptr )
			{
				agentToChase = currentAgent;
			}
			else
			{
				int lastRelationshipValue = m_agent->GetFaction()->GetRelationshipValue( agentToChase->GetFaction()->GetFactionNameID() );
				if ( currentRelationshipValue < lastRelationshipValue ) agentToChase = currentAgent;
			}
		}

		/*( *iter )->SetIsCellVisible( true );*/
	}

	if ( agentToChase != nullptr )
	{
		m_chaseTarget = agentToChase->GetPosition();
		return 5.f;
	}

	//return score;
	// no player found
	m_chaseTarget = Vec2i( -1, -1 ); // set target to unreachable value for Think()
	return 0.f;
}


//---------------------------------------------------------------------------
bool ChaseBehavior::Think( Map& map )
{
	if ( m_agent == nullptr || m_chaseTarget.x < 0 || m_chaseTarget.y < 0 ) return false;

	std::vector< Vec2i > path;
	PathFinder p;
	Vec2i agentPosition = m_agent->GetPosition();
	p.FindPath( agentPosition, m_chaseTarget, map, m_agent->GetMovementPropertes(), path );

	if ( path.size() > 1 )
	{
		Vec2i nextLocation = *( path.end() - 2 );
		m_agent->MoveAgent( &map, nextLocation - agentPosition );
	}
	
	return true;
}


//---------------------------------------------------------------------------
BaseAIBehavior* ChaseBehavior::Clone()
{
	return new ChaseBehavior( *this );
}