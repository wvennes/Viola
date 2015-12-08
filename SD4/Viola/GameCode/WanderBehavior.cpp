//---------------------------------------------------------------------------
// WanderBehavior.cpp
//---------------------------------------------------------------------------

#include "WanderBehavior.hpp"
#include "Engine/External/TinyXML/tinyxml.h"
#include "Engine/Core/TinyXMLHelper.hpp"
#include "Agent.hpp"
#define STATIC
#define UNUSED( x ) ( void )( x )

STATIC AIBehaviorRegistration WanderBehavior::s_wanderBehaviorRegistration( "wander", &WanderBehavior::CreateBehaviorFromXMLNode );

//---------------------------------------------------------------------------
WanderBehavior::WanderBehavior( const char* name )
	: BaseAIBehavior( name )
	, m_chanceToRest( 0.f )
{

}


//---------------------------------------------------------------------------
BaseAIBehavior* WanderBehavior::CreateBehaviorFromXMLNode( TiXmlElement* element )
{
	std::string name = GetAttributeAsString( element, "name" );
	WanderBehavior* behavior = new WanderBehavior( name.c_str() );
	behavior->m_chanceToRest = GetAttributeAsFloat( element, "chanceToRest" );
	return behavior;
}


//---------------------------------------------------------------------------
BaseAIBehavior* WanderBehavior::Clone()
{
	return new WanderBehavior( *this );
}


//---------------------------------------------------------------------------
bool WanderBehavior::Think( Map& map )
{
	float percentage = ( float ) rand() / ( float ) RAND_MAX;
	if ( percentage < m_chanceToRest ) return false;

	int direction = rand() % 8;
	Vec2i movement;

	switch ( direction )
	{
		case 0: //NORTH
		{
			movement = Vec2i( 0, 1 );
			break;
		}
		case 1: //SOUTH
		{
			movement = Vec2i( 0, -1 );
			break;
		}
		case 2: //EAST
		{
			movement = Vec2i( 1, 0 );
			break;
		}
		case 3: //WEST
		{
			movement = Vec2i( -1, 0 );
			break;
		}
		case 4: //NORTHEAST
		{
			movement = Vec2i( 1, 1 );
			break;
		}
		case 5: //NORTHWEST
		{
			movement = Vec2i( -1, 1 );
			break;
		}
		case 6: //SOUTHEAST
		{
			movement = Vec2i( 1, -1 );
			break;
		}
		case 7: //SOUTHWEST
		{
			movement = Vec2i( -1, -1 );
			break;
		}
	}

	m_agent->MoveAgent( &map, movement );
	return true;
}


//---------------------------------------------------------------------------
float WanderBehavior::Evaluate( Map& map )
{
	UNUSED( map );
	return 1.f;
}