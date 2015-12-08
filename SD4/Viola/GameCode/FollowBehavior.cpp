//---------------------------------------------------------------------------
// FollowBehavior.cpp
//---------------------------------------------------------------------------

#include "Engine/External/TinyXML/tinyxml.h"
#include "Engine/Core/TinyXMLHelper.hpp"
#include "PathFinder.hpp"
#include "Faction.hpp"
#include "Agent.hpp"
#include "FollowBehavior.hpp"
#define STATIC
#define UNUSED(x) (void)(x)

STATIC AIBehaviorRegistration FollowBehavior::s_followBehaviorRegistration( "follow", CreateBehaviorFromXMLNode );


//---------------------------------------------------------------------------
FollowBehavior::FollowBehavior( const char* name )
	: BaseAIBehavior( name )
	, m_distanceThreshold( 0.f )
	, m_followTarget( Vec2i( -1, -1 ) )
	, m_leaderName( "" )
{

}


//---------------------------------------------------------------------------
BaseAIBehavior* FollowBehavior::CreateBehaviorFromXMLNode( TiXmlElement* element )
{
	std::string name = GetAttributeAsString( element, "name" );
	FollowBehavior* behavior = new FollowBehavior( name.c_str() );

	behavior->m_leaderName = GetAttributeAsString( element, "leaderName" );
	behavior->m_distanceThreshold = GetAttributeAsFloat( element, "distanceThreshold" );

	return behavior;
}


//---------------------------------------------------------------------------
float FollowBehavior::Evaluate( Map& map )
{
	UNUSED( map );
	//m_fieldOfViewResult.m_cellsHit.clear();
	//float score = 0.f;
	if ( m_agent == nullptr ) return 0.f;

	Vec2i displacement( 0, 0 );
	float distance = 0.f;
	Agent* currentAgent = nullptr;
	const FieldOfViewResult* result = m_agent->GetFieldOfViewResult();

	EntitySet::iterator iter;
	for ( iter = result->m_entitiesHit.begin(); iter != result->m_entitiesHit.end(); ++ iter )
	{
		currentAgent = dynamic_cast< Agent* >( *iter );
		if ( currentAgent == nullptr ) continue;

		if ( m_leaderName != currentAgent->GetName() ) continue;

		displacement = currentAgent->GetPosition() - m_agent->GetPosition();
		distance = Vec2f( ( float ) displacement.x, ( float ) displacement.y ).CalculateDistance();

		if ( distance > m_distanceThreshold )
		{
			m_followTarget = currentAgent->GetPosition();
		}

		break;

		/*( *iter )->SetIsCellVisible( true );*/
	}

	//return score;
	// no player found
	// m_followTarget = Vec2i( -1, -1 ); // set target to unreachable value for Think()
	return 2.f * distance;
}


//---------------------------------------------------------------------------
bool FollowBehavior::Think( Map& map )
{
	if ( m_agent == nullptr || m_followTarget.x < 0 || m_followTarget.y < 0 ) return false;

	std::vector< Vec2i > path;
	PathFinder p;
	Vec2i agentPosition = m_agent->GetPosition();
	p.FindPath( agentPosition, m_followTarget, map, m_agent->GetMovementPropertes(), path );

	if ( path.size() > 1 )
	{
		Vec2i nextLocation = *( path.end() - 2 );
		m_agent->MoveAgent( &map, nextLocation - agentPosition );
	}

	return true;
}


//---------------------------------------------------------------------------
BaseAIBehavior* FollowBehavior::Clone()
{
	return new FollowBehavior( *this );
}