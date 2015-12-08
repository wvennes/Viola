//---------------------------------------------------------------------------
// LabyrinthGenerator.cpp
//---------------------------------------------------------------------------

#include "CellTypes.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "LabyrinthGenerator.hpp"
#define STATIC

STATIC MapGeneratorRegistration LabyrinthGenerator::s_labyrinthGeneratorRegistration( "Labyrinth", CreateGenerator );


//---------------------------------------------------------------------------
STATIC BaseMapGenerator* LabyrinthGenerator::CreateGenerator( const std::string& name )
{
	return new LabyrinthGenerator( name );
}


//---------------------------------------------------------------------------
LabyrinthGenerator::LabyrinthGenerator()
	: BaseMapGenerator( "none" )
{

}


//---------------------------------------------------------------------------
LabyrinthGenerator::LabyrinthGenerator( const std::string& name )
	: BaseMapGenerator( name )
{

}


//---------------------------------------------------------------------------
LabyrinthGenerator::~LabyrinthGenerator()
{
// 	m_visitedCellStack.clear();
// 	tempCellStack.swap( m_visitedCellStack 
//	delete m_visitedCellStack.data();
}


//---------------------------------------------------------------------------
Map* LabyrinthGenerator::GenerateMap()
{
	Map* map = new Map();
	CellCoords currentCoords;

	for ( int x = 0; x < MAP_WIDTH_CELLS; ++ x )
	{
		for ( int y = 0; y < MAP_HEIGHT_CELLS; ++ y )
		{
			currentCoords = CellCoords( x, y );
			Cell* cell = map->GetCell( currentCoords );
			if ( cell == nullptr ) continue;
			cell->SetCellProperties( CellTypes::WALL );
		}
	}

	PlaceSeedRoom( *map );

	return map;
}


//---------------------------------------------------------------------------
void LabyrinthGenerator::Startup()
{

}


//---------------------------------------------------------------------------
void LabyrinthGenerator::Shutdown()
{

}


//---------------------------------------------------------------------------
void LabyrinthGenerator::PlacePlayer( Map& map, Player& player )
{
	CellCoords coords = map.GetRandomValidLocationInMap( player.GetMovementPropertes() );
	Cell* cell = map.GetCell( coords );
	if ( cell == nullptr ) return;
	player.SetPosition( coords );
	cell->SetOccupyingAgent( &player );
	player.AddToMap( &map );
}


//---------------------------------------------------------------------------
void LabyrinthGenerator::PlaceSeedRoom( Map& map )
{
	CellCoords randomLocation( rand() % MAP_WIDTH_CELLS, rand() % MAP_HEIGHT_CELLS );
	Cell* currentCell = map.GetCell( randomLocation );
	if ( currentCell == nullptr ) return;
	currentCell->m_hasBeenVisited = true;
	currentCell->SetCellProperties( CellTypes::FLOOR );
	m_visitedCellStack.push_back( randomLocation );
}


//---------------------------------------------------------------------------
void LabyrinthGenerator::ProcessOneStep( Map& map )
{
	if ( m_visitedCellStack.empty() ) return;

	bool hasPlacedPath = false;
	Vec2i nextCellOffset;
	Vec2i nextWallOffset;
	CellCoords nextCell;
	CellCoords nextWall;

	while ( !hasPlacedPath && !m_visitedCellStack.empty() )
	{
		LabyrinthDirection randomDirection = ( LabyrinthDirection ) ( rand() % 4 );
		
		switch ( randomDirection )
		{
		case LABYRINTH_NORTH:
			{
				nextCellOffset = Vec2i( 0, 2 );
				nextWallOffset = Vec2i( 0, 1 );
				break;
			}
		case LABYRINTH_SOUTH:
			{
				nextCellOffset = Vec2i( 0, -2 );
				nextWallOffset = Vec2i( 0, -1 );
				break;
			}
		case LABYRINTH_EAST:
			{
				nextCellOffset = Vec2i( 2, 0 );
				nextWallOffset = Vec2i( 1, 0 );
				break;
			}
		case LABYRINTH_WEST:
			{
				nextCellOffset = Vec2i( -2, 0 );
				nextWallOffset = Vec2i( -1, 0 );
				break;
			}
		}

		nextCell = m_visitedCellStack.back() + nextCellOffset;
		nextWall = m_visitedCellStack.back() + nextWallOffset;

		Cell* cell = map.GetCell( nextCell );
		Cell* wallCell = map.GetCell( nextWall );

		if ( cell != nullptr && wallCell != nullptr && !cell->m_hasBeenVisited )
		{
			cell->SetCellProperties( CellTypes::FLOOR );
			cell->m_hasBeenVisited = true;
			wallCell->SetCellProperties( CellTypes::FLOOR );
			m_visitedCellStack.push_back( nextCell );
			hasPlacedPath = true;
		}
		else if ( HaveAllNeighborsBeenVisited( map ) )
		{
			m_visitedCellStack.pop_back();
		}

		if ( m_visitedCellStack.empty() )
		{
			std::vector< Vec2i >().swap( m_visitedCellStack );
		}
	}
}


//---------------------------------------------------------------------------
bool LabyrinthGenerator::HaveAllNeighborsBeenVisited( Map& map )
{
	Vec2i north( 0, 2 );
	Vec2i south( 0, -2 );
	Vec2i east( 2, 0 );
	Vec2i west( -2, 0 );
	Vec2i currentCell = m_visitedCellStack.back();

	Cell* cellToNorth = map.GetCell( currentCell + north );
	Cell* cellToSouth = map.GetCell( currentCell + south );
	Cell* cellToEast = map.GetCell( currentCell + east );
	Cell* cellToWest = map.GetCell( currentCell + west );

	if ( cellToNorth != nullptr && !cellToNorth->m_hasBeenVisited ) return false;
	if ( cellToSouth != nullptr && !cellToSouth->m_hasBeenVisited ) return false;
	if ( cellToEast != nullptr && !cellToEast->m_hasBeenVisited ) return false;
	if ( cellToWest != nullptr && !cellToWest->m_hasBeenVisited ) return false;

	return true;
}


//---------------------------------------------------------------------------
void LabyrinthGenerator::FinishGeneration( Map& map )
{
	while ( !m_visitedCellStack.empty() )
	{
		ProcessOneStep( map );
	}

	PlaceNPCAtRandomLocation( "goblin", map );
	PlaceNPCAtRandomLocation( "goblin", map );
	PlaceNPCAtRandomLocation( "goblin", map );
	PlaceNPCAtRandomLocation( "goblin", map );
	PlaceNPCAtRandomLocation( "goblin", map );
	PlaceNPCAtRandomLocation( "goblin", map );
	PlaceNPCAtRandomLocation( "goblin", map );
	PlaceNPCAtRandomLocation( "goblin", map );
	PlaceNPCAtRandomLocation( "goblin", map );
	PlaceNPCAtRandomLocation( "goblin", map );
}