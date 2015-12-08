//---------------------------------------------------------------------------
// GraveyardGenerator.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/EngineCommon.hpp"
#include "CellTypes.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "GraveyardGenerator.hpp"
#define STATIC

STATIC const unsigned int GraveyardGenerator::MAX_NUM_ROOMS = 15;
STATIC MapGeneratorRegistration GraveyardGenerator::s_graveyardGeneratorRegistration( "Graveyard", CreateGenerator );
int g_npcCounter = 0;

//---------------------------------------------------------------------------
STATIC BaseMapGenerator* GraveyardGenerator::CreateGenerator( const std::string& name )
{
	return new GraveyardGenerator( name );
}


//---------------------------------------------------------------------------
GraveyardGenerator::GraveyardGenerator()
	: BaseMapGenerator( "none" )
	, m_roomsPlaced( 0 )
{

}


//---------------------------------------------------------------------------
GraveyardGenerator::GraveyardGenerator( const std::string& name )
	: BaseMapGenerator( name )
	, m_roomsPlaced( 0 )
{

}


//---------------------------------------------------------------------------
GraveyardGenerator::~GraveyardGenerator()
{

}


//---------------------------------------------------------------------------
Map* GraveyardGenerator::GenerateMap()
{
	Map* map = new Map();
	m_roomsPlaced = 0;
	PlaceGraves( *map );
	return map;
}


//---------------------------------------------------------------------------
void GraveyardGenerator::Startup()
{

}


//---------------------------------------------------------------------------
void GraveyardGenerator::Shutdown()
{

}


//---------------------------------------------------------------------------
void GraveyardGenerator::PlacePlayer( Map& map, Player& player )
{
	CellCoords coords = map.GetRandomValidLocationInMap( player.GetMovementPropertes() );
	Cell* cell = map.GetCell( coords );
	if ( cell == nullptr ) return;
	player.SetPosition( coords );
	cell->SetOccupyingAgent( &player );
	player.AddToMap( &map );
}


//---------------------------------------------------------------------------
void GraveyardGenerator::ProcessOneStep( Map& map )
{
	CellCoords location = GetLocationForRoom( map );
	if ( location.x < 0 ) return;

	PlaceItemRoom( location, map );
}


//---------------------------------------------------------------------------
void GraveyardGenerator::FinishGeneration( Map& map )
{
	for ( int count = 0; count < 100; ++ count )
	{
		ProcessOneStep( map );
	}
	DebuggerPrintf( "npc counter = %i", g_npcCounter );
}


//---------------------------------------------------------------------------
void GraveyardGenerator::PlaceGraves( Map& map )
{
	CellCoords start( 1, 1 );

	for ( int x = start.x; x < MAP_WIDTH_CELLS; x += 2 )
	{
		for ( int y = start.y; y < MAP_HEIGHT_CELLS; y += 3 )
		{
			Cell* cell = map.GetCell( CellCoords( x, y ) );
			if ( cell == nullptr ) continue;
			cell->SetCellProperties( CellTypes::GRAVE );

			int placeGhost = rand() % 100;
			if ( placeGhost < 5 )
			{
				PlaceNPCAtLocation( CellCoords( x, y ), "spooky ghost", map );
				++ g_npcCounter;
			}
		}
	}
}


//---------------------------------------------------------------------------
void GraveyardGenerator::PlaceDoor( const CellCoords& location, Map& map )
{
	PlaceFeature( map, location, "Cedar Door" );
	Cell* cell = map.GetCell( location );
	Cell* cellToNorth = map.GetCell( location + Vec2i( 0, 1 ) );
	Cell* cellToSouth = map.GetCell( location + Vec2i( 0, -1 ) );

	if ( cell == nullptr || cellToNorth == nullptr || cellToSouth == nullptr ) return;

	// NORTH AND SOUTH CHECK
	if ( cellToNorth->IsType( TYPE_WALL ) && cellToSouth->IsType( TYPE_WALL ) )
	{
		cell->GetOccupyingFeature()->SwapActivatedGlyphs();
	}
}


//---------------------------------------------------------------------------
CellCoords GraveyardGenerator::GetLocationForRoom( Map& map )
{
	CellCoords result( -1, -1 );
	bool canPlaceRoom = false;
	bool hitWall = false;
	int attemptCount = 0;
	CellCoords validLocation;

	while ( !canPlaceRoom && attemptCount < 5000 )
	{
		// get random location
		validLocation = map.GetRandomValidLocationInMap( 0 );

		// will a room fit here?
		for ( int x = validLocation.x - 2; x <= validLocation.x + 2; ++ x )
		{
			for ( int y = validLocation.y - 2; y <= validLocation.y + 2; ++ y )
			{
				Cell* cell = map.GetCell( CellCoords( x, y ) );
				if ( cell == nullptr )
				{
					hitWall = true;
					break;
				}
				else if ( cell->IsType( TYPE_WALL ) && ( cell->GetCellColor() == Colors::WHITE ) )
				{
					hitWall = true;
					break;
				}
			}
		}

		if ( hitWall == false )
		{
			canPlaceRoom = true;
			result = validLocation;
		}

		++ attemptCount;
	}

	return result;
}


//---------------------------------------------------------------------------
void GraveyardGenerator::PlaceItemRoom( const CellCoords& location, Map& map )
{
	if ( m_roomsPlaced > MAX_NUM_ROOMS ) return;

	int roomDirection = rand() % NUM_GENERATOR_DIRECTIONS;
	Vec2i directionVector;
	CellCoords currentLocation;
	Cell* cell = nullptr;

	for ( int x = location.x - 2; x <= location.x + 2; ++ x )
	{
		for ( int y = location.y - 2; y <= location.y + 2; ++ y )
		{
			currentLocation = CellCoords( x, y );
			cell = map.GetCell( currentLocation );
			if ( cell == nullptr ) return;

			// first, clear the cell
			cell->SetCellProperties( CellTypes::FLOOR );

			// place walls around the center
			if ( currentLocation != location && currentLocation.x > location.x - 2 && currentLocation.x < location.x + 2 && currentLocation.y > location.y - 2 && currentLocation.y < location.y + 2 )
			{
				cell->SetCellProperties( CellTypes::WALL );
			}

			// place an item in the center
			if ( currentLocation == location )
			{
				PlaceRandomItemAtLocation( currentLocation, map );
			}
		}
	}

	switch ( roomDirection )
	{
		case MAP_NORTH:
		{
			directionVector = Vec2i( 0, 1 );
			break;
		}
		case MAP_SOUTH:
		{
			directionVector = Vec2i( 0, -1 );
			break;
		}
		case MAP_EAST:
		{
			directionVector = Vec2i( 1, 0 );
			break;
		}
		case MAP_WEST:
		{
			directionVector = Vec2i( -1, 0 );
			break;
		}
	}

	// place door and guard - random chance to spawn a zombie
	currentLocation = location;
	currentLocation += directionVector;
	cell = map.GetCell( currentLocation );
	cell->SetCellProperties( CellTypes::FLOOR );
	PlaceDoor( currentLocation, map );

	currentLocation += directionVector;
	PlaceNPCAtLocation( currentLocation, "guard", map );
	++ g_npcCounter;

	int placeZombie = rand() % 100;
	if ( placeZombie > 50 )
	{
		PlaceNPCAtLocation( location, "zombie", map );
		++ g_npcCounter;
	}

	currentLocation = location - ( directionVector * 2 );
	PlaceFeature( map, currentLocation, "Lamp" );

	++ m_roomsPlaced;
}