//---------------------------------------------------------------------------
// FrightBehavior.cpp
//---------------------------------------------------------------------------

#include "FrightBehavior.hpp"
#include "Engine/Core/TinyXMLHelper.hpp"
#include "Agent.hpp"
#include "PathFinder.hpp"
#define STATIC
#define UNUSED( x ) ( void )( x )

STATIC AIBehaviorRegistration FrightBehavior::s_frightBehaviorRegistration( "fright", &FrightBehavior::CreateBehaviorFromXMLNode );

//---------------------------------------------------------------------------
FrightBehavior::FrightBehavior( const char* name )
	: BaseAIBehavior( name )
	, m_scaredOfLocation( Vec2i( -1, -1 ) )
{

}


//---------------------------------------------------------------------------
FrightBehavior::~FrightBehavior()
{
	m_frightenedTargets.clear();
}


//---------------------------------------------------------------------------
STATIC BaseAIBehavior* FrightBehavior::CreateBehaviorFromXMLNode( TiXmlElement* element )
{
	TiXmlElement* scaredOf = element->FirstChildElement( "ScaredOf" );
	std::string name = GetAttributeAsString( element, "name" );

	FrightBehavior* behavior = new FrightBehavior( name.c_str() );

	for ( ; scaredOf != nullptr; scaredOf = element->NextSiblingElement( "ScaredOf" ) )
	{
		behavior->m_frightenedTargets.insert( GetAttributeAsString( scaredOf, "name" ) );
	}

	return behavior;
}


//---------------------------------------------------------------------------
BaseAIBehavior* FrightBehavior::Clone()
{
	return new FrightBehavior( *this );
}


//---------------------------------------------------------------------------
bool FrightBehavior::Think( Map& map )
{
	if ( m_scaredOfLocation.x < -1 ) return false;

	Vec2i currentLocation = m_agent->GetPosition();
	Vec2i displacement = currentLocation - m_scaredOfLocation;

	if ( displacement.x > 1 ) displacement.x = 1;
	if ( displacement.y > 1 ) displacement.y = 1;
	if ( displacement.x < -1 ) displacement.x = -1;
	if ( displacement.y < -1 ) displacement.y = -1;

	m_agent->MoveAgent( &map, displacement );

	return true;
}


//---------------------------------------------------------------------------
float FrightBehavior::Evaluate( Map& map )
{
	UNUSED( map );

	const FieldOfViewResult& result = *m_agent->GetFieldOfViewResult();
	EntitySet::iterator iter;
	for ( iter = result.m_entitiesHit.begin(); iter != result.m_entitiesHit.end(); ++ iter )
	{
		GameEntity* currentAgent = *iter;
		if ( currentAgent == nullptr ) continue;

		if ( m_frightenedTargets.find( currentAgent->GetName() ) != m_frightenedTargets.end() )
		{
			m_scaredOfLocation = currentAgent->GetPosition();
			return 500.f; // RUN AWAY!
		}
	}

	m_scaredOfLocation = Vec2i( -1, -1 );
	return 0.f;
}