//---------------------------------------------------------------------------
// LabyrinthGenerator.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_LABYRINTHGENERATOR_
#define _INCLUDED_LABYRINTHGENERATOR_

//#include "Engine/Math/Vec2.hpp"
#include <string>
#include "BaseMapGenerator.hpp"

class Map;


/////////////////////////////////////////////////////////////////////////////
enum LabyrinthDirection
{
	LABYRINTH_NORTH = 0,
	LABYRINTH_SOUTH = 1,
	LABYRINTH_EAST = 2,
	LABYRINTH_WEST = 3,
	NUM_LABYRINTH_DIRECTIONS = 4
};


/////////////////////////////////////////////////////////////////////////////
class LabyrinthGenerator : public BaseMapGenerator
{
public:
	static BaseMapGenerator* CreateGenerator( const std::string& name );
	~LabyrinthGenerator();

	Map*								GenerateMap();
	void								FinishGeneration( Map& map );
	void								Startup();
	void								Shutdown();
	void								PlacePlayer( Map& map, Player& player );
	void								ProcessOneStep( Map& map );

private:
	std::vector< CellCoords >			m_visitedCellStack;

private:
	void								PlaceSeedRoom( Map& map );
	bool								HaveAllNeighborsBeenVisited( Map& map );

protected:
	LabyrinthGenerator();
	LabyrinthGenerator( const std::string& name );

protected:
	static MapGeneratorRegistration		s_labyrinthGeneratorRegistration;

};

#endif