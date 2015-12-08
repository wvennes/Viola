//---------------------------------------------------------------------------
// CellularAutomataMapGenerator.cpp
//---------------------------------------------------------------------------

#include "Map.hpp"
#include "Player.hpp"
#include "NPCFactory.hpp"
#include "NPC.hpp"
#include "CellTypes.hpp"
#include "PathFinder.hpp"
#include "CellularAutomataMapGenerator.hpp"
#define STATIC

// register the generator
STATIC MapGeneratorRegistration CellularAutomataMapGenerator::s_cellularAutomataRegistration( "CellularAutomata", &CellularAutomataMapGenerator::CreateGenerator );


//---------------------------------------------------------------------------
BaseMapGenerator* CellularAutomataMapGenerator::CreateGenerator( const std::string& name )
{
	return new CellularAutomataMapGenerator( name );
}


//---------------------------------------------------------------------------
Map* CellularAutomataMapGenerator::GenerateMap()
{
	Map* map = new Map();
	InitializeRandomData( *map );
	//ProcessOneStep( *map );
	return map;
}


//---------------------------------------------------------------------------
void CellularAutomataMapGenerator::FinishGeneration( Map& map )
{
	int numIterations = 4;
	int numItems = 20;

	for ( int count = 0; count < numIterations; ++ count )
	{
		ProcessOneStep( map );
	}

	for ( int count = 0; count < numItems; ++ count )
	{
		PlaceRandomItem( map );
	}

// 	PathFinder p;
// 	p.FindPath( map.GetRandomValidLocationInMap(), map.GetRandomValidLocationInMap(), map );

	PlaceNPCAtRandomLocation( "orc", map );
	PlaceNPCAtRandomLocation( "orc", map );
	PlaceNPCAtRandomLocation( "orc", map );
	PlaceNPCAtRandomLocation( "orc", map );
	PlaceNPCAtRandomLocation( "ogre", map );
	PlaceNPCAtRandomLocation( "imp", map );
	PlaceNPCAtRandomLocation( "imp", map );
	PlaceNPCAtRandomLocation( "imp", map );
	PlaceNPCAtRandomLocation( "imp", map );

// 	PlaceOrc( map );
// 	PlaceOrc( map );
// 	PlaceOrc( map );
// 	PlaceGoblin( map );
// 	PlaceGoblin( map );
// 	PlaceGoblin( map );
// 	PlaceImp( map );
// 	PlaceImp( map );
// 	PlaceImp( map );
// 	PlaceOrc( map );
}


//---------------------------------------------------------------------------
void CellularAutomataMapGenerator::Startup()
{

}


//---------------------------------------------------------------------------
void CellularAutomataMapGenerator::Shutdown()
{

}


//---------------------------------------------------------------------------
void CellularAutomataMapGenerator::PlacePlayer( Map& map, Player& player )
{
	CellCoords coords = map.GetRandomValidLocationInMap( player.GetMovementPropertes() );
	Cell* cell = map.GetCell( coords );
	if ( cell == nullptr ) return;

	player.SetPosition( coords );
	cell->SetOccupyingAgent( &player );
	player.AddToMap( &map );
}


//---------------------------------------------------------------------------
void CellularAutomataMapGenerator::ProcessOneStep( Map& map )
{
	Map* tempMap = new Map( map );
	int numWalls = 0;
	CellCoords currentCellCoords;
	for ( int x = 0; x < MAP_WIDTH_CELLS; ++ x )
	{
		for ( int y = 0; y < MAP_HEIGHT_CELLS; ++ y )
		{
			currentCellCoords = CellCoords( x, y );
			Cell* cell = map.GetCell( currentCellCoords );
			if ( cell == nullptr ) continue;
			numWalls = CalculateNumberOfWallsAroundPoint( *tempMap, currentCellCoords );

			if ( numWalls >= 15 || numWalls < 5 )
			{
				cell->SetCellProperties( CellTypes::WALL );
			}
			else
			{
				cell->SetCellProperties( CellTypes::FLOOR );
			}
		}
	}

	delete tempMap;
}


//---------------------------------------------------------------------------
int CellularAutomataMapGenerator::CalculateNumberOfWallsAroundPoint( Map& map, Vec2i& coords )
{
	int radius = 2;
	int result = 0;

	for ( int x = coords.x - radius; x <= coords.x + radius; ++ x )
	{
		for ( int y = coords.y - radius; y <= coords.y + radius; ++ y )
		{
			if ( x >= 0 && x < MAP_WIDTH_CELLS && y >= 0 && y < MAP_HEIGHT_CELLS && ( x != 0 && y != 0 ) )
			{
				Cell* cell = map.GetCell( CellCoords( x, y ) );
				if ( cell != nullptr && cell->m_properties.m_type == TYPE_WALL ) ++ result;
			}
		}
	}

	return result;
}