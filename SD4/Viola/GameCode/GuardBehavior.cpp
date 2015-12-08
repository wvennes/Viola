//---------------------------------------------------------------------------
// GuardBehavior.cpp
//---------------------------------------------------------------------------

#include "GuardBehavior.hpp"
#include "Engine/Core/TinyXMLHelper.hpp"
#include "Agent.hpp"
#include "PathFinder.hpp"
#define STATIC
#define UNUSED( x ) ( void )( x )

STATIC AIBehaviorRegistration GuardBehavior::s_guardBehaviorRegistration( "guard", &GuardBehavior::CreateBehaviorFromXMLNode );

//---------------------------------------------------------------------------
GuardBehavior::GuardBehavior( const char* name )
	: BaseAIBehavior( name )
	, m_hasLocationBeenSet( false )
	, m_isGuardGoingHome( false )
	, m_maxTravelLength( 0.f )
	, m_guardLocation( Vec2i( 0, 0 ) )
{

}


//---------------------------------------------------------------------------
BaseAIBehavior* GuardBehavior::CreateBehaviorFromXMLNode( TiXmlElement* element )
{
	std::string name = GetAttributeAsString( element, "name" );
	GuardBehavior* behavior = new GuardBehavior( name.c_str() );
	behavior->m_maxTravelLength = GetAttributeAsFloat( element, "travelDistance" );
	return behavior;
}


//---------------------------------------------------------------------------
BaseAIBehavior* GuardBehavior::Clone()
{
	return new GuardBehavior( *this );
}


//---------------------------------------------------------------------------
bool GuardBehavior::Think( Map& map )
{
	if ( m_isGuardGoingHome && m_currentPath.size() > 0 )
	{
		m_agent->MoveAgent( &map, m_currentPath.back() - m_agent->GetPosition() );
		m_currentPath.pop_back();
	}
	else if ( m_isGuardGoingHome && m_currentPath.empty() )
	{
		PathFinder p;
		p.FindPath( m_agent->GetPosition(), m_guardLocation, map, m_agent->GetMovementPropertes(), m_currentPath );

// 		m_agent->MoveAgent( &map, m_currentPath.back() - m_agent->GetPosition() );
// 		m_currentPath.pop_back();

//		if ( m_currentPath.empty() && m_agent->GetPosition() == m_guardLocation ) m_isGuardGoingHome = false;
	}
	if ( m_currentPath.empty() )
	{
		m_isGuardGoingHome = false;
	}

	return true;
}


//---------------------------------------------------------------------------
float GuardBehavior::Evaluate( Map& map )
{
	if ( m_isGuardGoingHome )
	{
		return 100.f;
	}

	if ( !m_hasLocationBeenSet )
	{
		m_guardLocation = m_agent->GetPosition();
		m_hasLocationBeenSet = true;
	}

	Vec2f home( ( float ) m_guardLocation.x, ( float ) m_guardLocation.y );
	Vec2f currentLocation( ( float ) m_agent->GetPosition().x, ( float ) m_agent->GetPosition().y );
	Vec2f displacement = currentLocation - home;
	float distance = displacement.CalculateDistanceSquared();

	if ( distance > ( m_maxTravelLength * m_maxTravelLength ) )
	{
		m_isGuardGoingHome = true;
		return 100.f;
	}
	else if ( m_guardLocation != m_agent->GetPosition() )
	{
		m_isGuardGoingHome = true;
		return 100.f;
	}

	UNUSED( map );
	return 0.f;
}