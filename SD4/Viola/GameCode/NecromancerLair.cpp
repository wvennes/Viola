//---------------------------------------------------------------------------
// NecromancerLairGenerator.cpp
//---------------------------------------------------------------------------

#include <vector>
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/GameMath.hpp"
#include "CellTypes.hpp"
#include "Map.hpp"
#include "PathFinder.hpp"
#include "Player.hpp"
#include "NecromancerLairGenerator.hpp"
#define UNUSED(x) (void)(x)
#define STATIC

STATIC MapGeneratorRegistration NecromancerLairGenerator::s_necromancerLairGeneratorRegistration( "NecromancerLair", CreateGenerator );

//---------------------------------------------------------------------------
STATIC BaseMapGenerator* NecromancerLairGenerator::CreateGenerator( const std::string& name )
{
	return new NecromancerLairGenerator( name );
}


//---------------------------------------------------------------------------
NecromancerLairGenerator::NecromancerLairGenerator()
	: BaseMapGenerator( "none" )
{

}


//---------------------------------------------------------------------------
NecromancerLairGenerator::NecromancerLairGenerator( const std::string& name )
	: BaseMapGenerator( name )
{

}


//---------------------------------------------------------------------------
NecromancerLairGenerator::~NecromancerLairGenerator()
{

}


//---------------------------------------------------------------------------
Map* NecromancerLairGenerator::GenerateMap()
{
	Map* map = new Map();
	for ( unsigned int index = 0; index < ( unsigned int ) map->m_cells.size(); ++ index )
	{
		map->m_cells[ index ].SetCellProperties( CellTypes::WALL );
	}
	return map;
}


//---------------------------------------------------------------------------
void NecromancerLairGenerator::Startup()
{

}


//---------------------------------------------------------------------------
void NecromancerLairGenerator::Shutdown()
{

}


//---------------------------------------------------------------------------
void NecromancerLairGenerator::PlacePlayer( Map& map, Player& player )
{
	PlaceEntrance( map, player );
// 	CellCoords coords = map.GetRandomValidLocationInMap( player.GetMovementPropertes() );
// 	Cell* cell = map.GetCell( coords );
// 	if ( cell == nullptr ) return;
// 	player.SetPosition( coords );
// 	cell->SetOccupyingAgent( &player );
// 	player.AddToMap( &map );
}


//---------------------------------------------------------------------------
void NecromancerLairGenerator::ProcessOneStep( Map& map )
{
	UNUSED( map );
}


//---------------------------------------------------------------------------
void NecromancerLairGenerator::FinishGeneration( Map& map )
{
	PlaceNecromancerRoom( map );
	PlaceAllLavaRivers( map );
	PlaceCatacombs( map );
}


//---------------------------------------------------------------------------
void NecromancerLairGenerator::PlaceNecromancerRoom( Map& map )
{
	int roomWidth = GetRandomIntInRange( 5, 8 );
	int roomHeight = GetRandomIntInRange( 8, 12 );

	Cell* currentCell = nullptr;
	Vec2i currentCoords( 0, 0 );
	Vec2i leftHandCorner( MAP_WIDTH_CELLS - roomWidth - 1, GetRandomIntInRange( 0, MAP_HEIGHT_CELLS - roomHeight + 1 ) );

	for ( int x = 0; x < roomWidth; ++ x )
	{
		for ( int y = 0; y < roomHeight; ++ y )
		{
			currentCoords = leftHandCorner + Vec2i( x, y );
			currentCell = map.GetCell( currentCoords );
			if ( currentCell == nullptr ) continue;

			if ( x == 0 && y == 0 )
			{
				PlaceNPCAtLocation( currentCoords, "necromancer", map );
			}

			currentCell->SetCellProperties( CellTypes::FLOOR );
		}
	}

	PlacePathToNecromancerRoom( map, leftHandCorner, Vec2i( roomWidth, roomHeight ) );
}


//---------------------------------------------------------------------------
void NecromancerLairGenerator::PlacePathToNecromancerRoom( Map& map, const Vec2i& leftHandCorner, const Vec2i& dimensions )
{
	Vec2i startCoords( leftHandCorner.x, leftHandCorner.y + ( int ) ( ( float ) dimensions.y * 0.5 ) );
	Vec2i currentCoords = startCoords;
	Cell* currentCell = nullptr;
	float chanceToBecomeWall = 0.f;
	for ( ; currentCoords.x >= 45; -- currentCoords.x )
	{
		for ( int y = -1; y <= 1; ++ y )
		{
			currentCell = map.GetCell( currentCoords + Vec2i( 0, y ) );
			if ( currentCell == nullptr ) continue;
			chanceToBecomeWall = ( float ) rand() / ( float ) RAND_MAX;

			if ( y == -1 || y == 1 )
			{
				if ( chanceToBecomeWall <= 0.33f )
				{
					currentCell->SetCellProperties( CellTypes::FLOOR );					
				}
			}
			else
			{
				if ( map.GetCell( currentCoords + Vec2i( 0, 1 ) )->IsType( TYPE_WALL ) && map.GetCell( currentCoords + Vec2i( 0, -1 ) )->IsType( TYPE_WALL ) )
				{
					currentCell->SetCellProperties( CellTypes::FLOOR );
				}
				else if ( chanceToBecomeWall <= 0.1f )
				{
					currentCell->SetCellProperties( CellTypes::FLOOR );					
				}
			}
		}
	}
}


//---------------------------------------------------------------------------
void NecromancerLairGenerator::PlaceAllLavaRivers( Map& map )
{
	PlaceLavaRiver( map );
	PlaceLavaRiver( map );
	PlaceLavaRiver( map );
}


//---------------------------------------------------------------------------
void NecromancerLairGenerator::PlaceLavaRiver( Map& map )
{
	Vec2i start( GetRandomIntInRange( 10, 54 ), MAP_HEIGHT_CELLS - 1 );
	Vec2i end( GetRandomIntInRange( 10, 54 ), 0 );

	std::vector< Vec2i > path;
	PathFinder p;
	p.FindPath( start, end, map, 0, path );

	for ( unsigned int index = 0; index < ( unsigned int ) path.size(); ++ index )
	{
		Vec2i& currentPosition = path[ index ];
		Cell* cell = map.GetCell( currentPosition );
		if ( cell == nullptr ) continue;

		cell->SetCellProperties( CellTypes::LAVA );
	}
}


//---------------------------------------------------------------------------
void NecromancerLairGenerator::PlaceCatacombs( Map& map )
{
	Vec2i start( 7, 2 );
	Vec2i end( 50, MAP_HEIGHT_CELLS - 3 );
	float chanceToConvert = 0.f;
	float chanceToPlaceFireBat = 0.f;
	float chanceToPlaceSkeleton = 0.f;
	float chanceToPlaceVampireBat = 0.f;

	for ( int x = start.x; x <= end.x; ++ x )
	{
		for ( int y = start.y; y <= end.y; ++ y )
		{
			Cell* cell = map.GetCell( Vec2i( x, y ) );
			if ( cell == nullptr ) continue;
			chanceToConvert = ( float ) rand() / ( float ) RAND_MAX;
			chanceToPlaceFireBat = ( float ) rand() / ( float ) RAND_MAX;
			chanceToPlaceSkeleton = ( float ) rand() / ( float ) RAND_MAX;
			chanceToPlaceVampireBat = ( float ) rand() / ( float ) RAND_MAX;

			if ( cell->IsType( TYPE_FIRE ) )
			{
				if ( chanceToConvert <= 0.33f )
				{
					cell->SetCellProperties( CellTypes::FLOOR );
				}
				else if ( chanceToPlaceFireBat <= 0.5f )
				{
					PlaceNPCAtLocation( Vec2i( x, y ), "fire bat", map );
				}
			}
			else if ( cell->IsType( TYPE_WALL ) )
			{
				if ( chanceToConvert <= 1.f - chanceToConvert )
				{
					cell->SetCellProperties( CellTypes::FLOOR );
					if ( chanceToPlaceVampireBat <= 0.2f )
					{
						PlaceNPCAtLocation( Vec2i( x, y ), "vampire bat", map );
					}
					else if ( chanceToPlaceSkeleton <= 0.2f )
					{
						PlaceNPCAtLocation( Vec2i( x, y ), "skeleton", map );
					}
				}
			}
		}
	}
}


//---------------------------------------------------------------------------
void NecromancerLairGenerator::PlaceEntrance( Map& map, Player& player )
{
	int roomWidth = GetRandomIntInRange( 4, 6 );
	int roomHeight = GetRandomIntInRange( 4, 6 );
	Vec2i leftHandCorner( 2, GetRandomIntInRange( 0, MAP_HEIGHT_CELLS - roomHeight + 1 ) );
	Vec2i playerLocation( 0 + ( int ) ( ( float ) roomWidth * 0.5f ), leftHandCorner.y + ( int ) ( ( float ) roomWidth * 0.5f ) );
	Vec2i currentCoords( 0, 0 );
	Cell* currentCell = nullptr;

	Cell* cell = map.GetCell( playerLocation );
	if ( cell == nullptr ) return;
	player.SetPosition( playerLocation );
	cell->SetOccupyingAgent( &player );
	player.AddToMap( &map );

	PlaceNPCAtLocation( leftHandCorner + Vec2i( 1, 1 ), "skeleton", map );
	PlaceNPCAtLocation( leftHandCorner + Vec2i( roomWidth, roomHeight ) - Vec2i( 1, 1 ), "skeleton", map );
	PlaceNPCAtLocation( playerLocation - Vec2i( 1, 0 ), "cleric", map );

	for ( int x = 0; x < roomWidth; ++ x )
	{
		for ( int y = 0; y < roomHeight; ++ y )
		{
			currentCoords = leftHandCorner + Vec2i( x, y );
			currentCell = map.GetCell( currentCoords );
			if ( currentCell == nullptr ) continue;

			currentCell->SetCellProperties( CellTypes::FLOOR );
		}
	}

	PlaceEntranceHallway( map, leftHandCorner, Vec2i( roomWidth, roomHeight ) );
}


//---------------------------------------------------------------------------
void NecromancerLairGenerator::PlaceEntranceHallway( Map& map, const Vec2i& leftHandCorner, const Vec2i& dimensions )
{
	Vec2i start = leftHandCorner + Vec2i( dimensions.x, ( int ) ( ( float ) dimensions.y * 0.5f ) );

	for ( ; start.x < dimensions.x + 5; ++ start.x )
	{
		for ( int y = -1; y <= 1; ++ y )
		{
			Cell* cell = map.GetCell( start + Vec2i( 0, y ) );
			if ( cell == nullptr ) continue;
			cell->SetCellProperties( CellTypes::FLOOR );
		}
	}
}