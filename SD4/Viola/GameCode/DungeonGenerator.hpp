//---------------------------------------------------------------------------
// DungeonGenerator.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_DUNGEONGENERATOR_
#define _INCLUDED_DUNGEONGENERATOR_

//#include "Engine/Math/Vec2.hpp"
#include <string>
#include "BaseMapGenerator.hpp"

class Map;


/////////////////////////////////////////////////////////////////////////////
enum DungeonDirection
{
	DUNGEON_NORTH,
	DUNGEON_SOUTH,
	DUNGEON_EAST,
	DUNGEON_WEST,
	NUM_DUNGEON_DIRECTIONS
};


/////////////////////////////////////////////////////////////////////////////
class DungeonGenerator : public BaseMapGenerator
{
public:
	static BaseMapGenerator* CreateGenerator( const std::string& name );
	~DungeonGenerator() {};

	Map* GenerateMap();
	void FinishGeneration( Map& map );
	void Startup();
	void Shutdown();
	void PlacePlayer( Map& map, Player& player );
	void PlaceDoor( Map& map, Vec2i& location );
	void ProcessOneStep( Map& map );
	//int CalculateNumberOfWallsAroundPoint( Map& map, Vec2i& coords );

private:
	const static CellCoords ROOM_DIMENSIONS_RANGE;
	const static int		MAX_STEP_ITERATIONS;
	const static int		MAX_FLOOR_OVERLAP;

private:
	void PlaceSeedRoom( Map& map );
	void FillRoom( Map& map, const CellCoords& mins, const CellCoords& maxes );
	int GetNumFloorsInCardinalDirections( Map& map, const CellCoords& location );
	int GetNumFloorsInArea( Map& map, const CellCoords& location, const CellCoords& dimensions );
	int GetNumFloorsForHallway( Map& map, const CellCoords& location, DungeonDirection direction, int length );
	DungeonDirection GetDirectionToCarveIn( Map& map, const CellCoords& location );
	void CreateRoom( Map& map, const CellCoords& location );
	bool CarveRoom( Map& map, const CellCoords& location, DungeonDirection roomDirection, const CellCoords& roomDimensions );
	bool CarveHallway( Map& map, const CellCoords& hallwayLocation, DungeonDirection hallwayDirection, int hallwayLength );

protected:
	DungeonGenerator() : BaseMapGenerator( "none" ) {}
	DungeonGenerator( std::string name ) : BaseMapGenerator( name ) {}

protected:
	static MapGeneratorRegistration		s_dungeonGeneratorRegistration;

};

#endif