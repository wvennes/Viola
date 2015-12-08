//---------------------------------------------------------------------------
// DungeonGenerator.cpp
//---------------------------------------------------------------------------

#include "Map.hpp"
#include "Player.hpp"
#include "GameCommon.hpp"
#include "CellTypes.hpp"
#include "DungeonGenerator.hpp"
#define STATIC

STATIC MapGeneratorRegistration DungeonGenerator::s_dungeonGeneratorRegistration( "Dungeon", CreateGenerator );

STATIC const CellCoords DungeonGenerator::ROOM_DIMENSIONS_RANGE( 2, 7 );
STATIC const int DungeonGenerator::MAX_STEP_ITERATIONS = 50000;
STATIC const int DungeonGenerator::MAX_FLOOR_OVERLAP = 1;


//---------------------------------------------------------------------------
STATIC BaseMapGenerator* DungeonGenerator::CreateGenerator( const std::string& name )
{
	return new DungeonGenerator( name );
}


//---------------------------------------------------------------------------
Map* DungeonGenerator::GenerateMap()
{
	Map* map = new Map();
	for ( unsigned int index = 0; index < map->m_cells.size(); ++ index )
	{
		map->m_cells.at( index ).m_properties = CellTypes::WALL;
	}
	PlaceSeedRoom( *map );
	//ProcessOneStep( *map );
	return map;
}


//---------------------------------------------------------------------------
void DungeonGenerator::FinishGeneration( Map& map )
{
	int numIterations = 200;

	for ( int count = 0; count < numIterations; ++ count )
	{
		ProcessOneStep( map );
	}

	PlaceNPCAtRandomLocation( "goblin", map );
	PlaceNPCAtRandomLocation( "goblin", map );
	PlaceNPCAtRandomLocation( "goblin", map );
	PlaceNPCAtRandomLocation( "goblin", map );
	PlaceNPCAtRandomLocation( "gargoyle", map );
	PlaceNPCAtRandomLocation( "gargoyle", map );
	PlaceNPCAtRandomLocation( "gargoyle", map );
	PlaceNPCAtRandomLocation( "imp", map );
	PlaceNPCAtRandomLocation( "imp", map );
}


//---------------------------------------------------------------------------
void DungeonGenerator::Startup()
{

}


//---------------------------------------------------------------------------
void DungeonGenerator::Shutdown()
{

}


//---------------------------------------------------------------------------
void DungeonGenerator::PlacePlayer( Map& map, Player& player )
{
	CellCoords coords = map.GetRandomValidLocationInMap( player.GetMovementPropertes() );
	Cell* cell = map.GetCell( coords );
	if ( cell == nullptr ) return;
	player.SetPosition( coords );
	cell->SetOccupyingAgent( &player );
	player.AddToMap( &map );
}


//---------------------------------------------------------------------------
void DungeonGenerator::PlaceDoor( Map& map, Vec2i& location )
{
	PlaceFeature( map, location, "Cedar Door" );
	Cell* cell = map.GetCell( location );
	Cell* cellToNorth = map.GetCell( location + Vec2i( 0, 1 ) );
	Cell* cellToSouth = map.GetCell( location + Vec2i( 0, -1 ) );

	if ( cell == nullptr || cellToNorth == nullptr || cellToSouth == nullptr ) return;

	// NORTH AND SOUTH CHECK
	if ( cellToNorth->IsBlockedByCell( IS_BLOCKED_BY_WALLS ) && cellToSouth->IsBlockedByCell( IS_BLOCKED_BY_WALLS ) )
	{
		cell->GetOccupyingFeature()->SwapActivatedGlyphs();
	}
}


//---------------------------------------------------------------------------
void DungeonGenerator::ProcessOneStep( Map& map )
{
	CellCoords location;
	bool didObjectCarve = false;
	DungeonDirection direction;
	int isRoomOrHallway = rand() % 2;

	for ( int count = 0; count < MAX_STEP_ITERATIONS; ++ count )
	{
		location = CellCoords( rand() % MAP_WIDTH_CELLS, rand() % MAP_HEIGHT_CELLS );

		if ( GetNumFloorsInCardinalDirections( map, location ) == 1 )
		{
			direction = GetDirectionToCarveIn( map, location );

			if ( isRoomOrHallway == 0 )
			{
				Vec2i dimensions( 
					ROOM_DIMENSIONS_RANGE.x + rand() % ( ROOM_DIMENSIONS_RANGE.y - ROOM_DIMENSIONS_RANGE.x + 1 ), 
					ROOM_DIMENSIONS_RANGE.x + rand() % ( ROOM_DIMENSIONS_RANGE.y - ROOM_DIMENSIONS_RANGE.x + 1 ) );

				didObjectCarve = CarveRoom( map, location, direction, dimensions );
			}
			else
			{
				int length = ROOM_DIMENSIONS_RANGE.x + rand() % ( ROOM_DIMENSIONS_RANGE.y - ROOM_DIMENSIONS_RANGE.x + 1 );

				didObjectCarve = CarveHallway( map, location, direction, length );
			}

			if ( didObjectCarve ) break; // break if carved something
		}
	}
//	PlaceSeedRoom( map );
// 	for ( unsigned int index = 0; index < m_rooms.size(); ++ index )
// 	{
// 		Room& currentRoom = m_rooms[ index ];
// 		roomLocation = CellCoords( rand() % MAP_WIDTH_CELLS, rand() % MAP_HEIGHT_CELLS );
// 	}
}


//---------------------------------------------------------------------------
int DungeonGenerator::GetNumFloorsInCardinalDirections( Map& map, const CellCoords& location )
{
	int result = 0;

	Cell* cell = map.GetCell( location );

	if ( cell == nullptr || cell->m_properties.m_type == TYPE_AIR ) return result;

	Cell* cellToNorth = map.GetCell( location + Vec2i( 0, 1 ) );
	Cell* cellToSouth = map.GetCell( location + Vec2i( 0, -1 ) );
	Cell* cellToEast = map.GetCell( location + Vec2i( 1, 0 ) );
	Cell* cellToWest = map.GetCell( location + Vec2i( -1, 0 ) );

	if ( cellToNorth != nullptr && cellToNorth->m_properties.m_type == TYPE_AIR ) ++ result; // to north
	if ( cellToSouth != nullptr && cellToSouth->m_properties.m_type == TYPE_AIR ) ++ result; // to south
	if ( cellToEast != nullptr && cellToEast->m_properties.m_type == TYPE_AIR ) ++ result; // to east
	if ( cellToWest != nullptr && cellToWest->m_properties.m_type == TYPE_AIR ) ++ result; // to west

	return result;
}


//---------------------------------------------------------------------------
DungeonDirection DungeonGenerator::GetDirectionToCarveIn( Map& map, const CellCoords& location )
{
	Cell* cellToNorth = map.GetCell( location + Vec2i( 0, 1 ) );
	Cell* cellToSouth = map.GetCell( location + Vec2i( 0, -1 ) );
	Cell* cellToEast = map.GetCell( location + Vec2i( 1, 0 ) );
	Cell* cellToWest = map.GetCell( location + Vec2i( -1, 0 ) );

	if ( cellToNorth != nullptr && cellToNorth->m_properties.m_type == TYPE_AIR ) return DUNGEON_SOUTH; // to north
	if ( cellToSouth != nullptr && cellToSouth->m_properties.m_type == TYPE_AIR ) return DUNGEON_NORTH; // to south
	if ( cellToEast != nullptr && cellToEast->m_properties.m_type == TYPE_AIR ) return DUNGEON_WEST; // to east
	if ( cellToWest != nullptr && cellToWest->m_properties.m_type == TYPE_AIR ) return DUNGEON_EAST; // to west

	return DUNGEON_NORTH;
}


//---------------------------------------------------------------------------
int DungeonGenerator::GetNumFloorsInArea( Map& map, const CellCoords& location, const CellCoords& dimensions )
{
	int count = 0;
	for ( int x = location.x; x <= location.x + dimensions.x; ++ x )
	{
		for ( int y = location.y; y <= location.y + dimensions.y; ++ y )
		{
			Cell* cell = map.GetCell( CellCoords( x, y ) );
			if ( cell != nullptr && cell->m_properties.m_type == TYPE_AIR ) ++ count;
		}
	}

	return count;
}


//---------------------------------------------------------------------------
int DungeonGenerator::GetNumFloorsForHallway( Map& map, const CellCoords& location, DungeonDirection direction, int length )
{
	int count = 0;
	Vec2i directionVector;
	CellCoords currentLocation = location;

	switch ( direction )
	{
		case DUNGEON_NORTH:
		{
			directionVector = Vec2i( 0, 1 );
			break;
		}
		case DUNGEON_SOUTH:
		{
			directionVector = Vec2i( 0, -1 );
			break;
		}
		case DUNGEON_EAST:
		{
			directionVector = Vec2i( 1, 0 );
			break;
		}
		case DUNGEON_WEST:
		{
			directionVector = Vec2i( -1, 0 );
			break;
		}
	}

	for ( int hallCount = 0; hallCount < length; ++ hallCount )
	{
		currentLocation += directionVector;
		Cell* cell = map.GetCell( currentLocation );
		if ( cell != nullptr && cell->m_properties.m_type == TYPE_AIR ) ++ count;
	}

	return count;
}


//---------------------------------------------------------------------------
void DungeonGenerator::PlaceSeedRoom( Map& map )
{
	CellCoords position( ( int ) ( MAP_WIDTH_CELLS * 0.5 ), ( int ) ( MAP_HEIGHT_CELLS * 0.5 ) );
	CellCoords positionOffsets;

	positionOffsets.x = ROOM_DIMENSIONS_RANGE.x + ( rand() % ( ROOM_DIMENSIONS_RANGE.y - ROOM_DIMENSIONS_RANGE.x + 1 ) );
	positionOffsets.y = ROOM_DIMENSIONS_RANGE.x + ( rand() % ( ROOM_DIMENSIONS_RANGE.y - ROOM_DIMENSIONS_RANGE.x + 1 ) );

	FillRoom( map, position, position + positionOffsets );
}


//---------------------------------------------------------------------------
void DungeonGenerator::FillRoom( Map& map, const CellCoords& mins, const CellCoords& maxes )
{
	for ( int x = mins.x; x <= maxes.x; ++ x )
	{
		for ( int y = mins.y; y <= maxes.y; ++ y )
		{
			Cell* cell = map.GetCell( CellCoords( x, y ) );
			if ( cell != nullptr ) cell->SetCellProperties( CellTypes::ROOM );
		}
	}
}


//---------------------------------------------------------------------------
bool DungeonGenerator::CarveRoom( Map& map, const CellCoords& roomLocation, DungeonDirection roomDirection, const CellCoords& roomDimensions )
{
	bool didRoomCarve = false;
	CellCoords currentLocation = roomLocation;
	Vec2i direction = roomDimensions;

	switch( roomDirection )
	{
	case DUNGEON_NORTH:
		{
			// do nothing
			break;
		}
	case DUNGEON_SOUTH:
		{
			direction = direction * -1;
			break;
		}
	case DUNGEON_EAST:
		{
			// do nothing
			break;
		}
	case DUNGEON_WEST:
		{
			direction = direction * -1;
			break;
		}
	}

	if ( GetNumFloorsInArea( map, currentLocation, direction ) < MAX_FLOOR_OVERLAP )
	{
		FillRoom( map, currentLocation, currentLocation + direction );
		didRoomCarve = true;
	}

	return didRoomCarve;
}


//---------------------------------------------------------------------------
bool DungeonGenerator::CarveHallway( Map& map, const CellCoords& hallwayLocation, DungeonDirection hallwayDirection, int hallwayLength )
{
	bool didHallwayCarve = false;
	CellCoords currentLocation = hallwayLocation;
	Vec2i direction;
	switch( hallwayDirection )
	{
		case DUNGEON_NORTH:
		{
			direction = Vec2i( 0, 1 );
			break;
		}
		case DUNGEON_SOUTH:
		{
			direction = Vec2i( 0, -1 );
			break;
		}
		case DUNGEON_EAST:
		{
			direction = Vec2i( 1, 0 );
			break;
		}
		case DUNGEON_WEST:
		{
			direction = Vec2i( -1, 0 );
			break;
		}
	}

	if ( GetNumFloorsForHallway( map, hallwayLocation, hallwayDirection, hallwayLength ) < MAX_FLOOR_OVERLAP )
	{
		didHallwayCarve = true;
		for ( int count = 0; count < hallwayLength; ++ count )
		{
			Cell* cell = map.GetCell( currentLocation );
			if ( cell == nullptr ) break;

			if ( count == 0 )
			{
				PlaceDoor( map, currentLocation );
			}

			cell->SetCellProperties( CellTypes::HALLWAY );

			currentLocation += direction;
		}
	}

	return didHallwayCarve;
}