//---------------------------------------------------------------------------
// CellularAutomataMapGenerator.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_CELLULARAUTOMATAMAPGENERATOR_
#define _INCLUDED_CELLULARAUTOMATAMAPGENERATOR_

#include "Engine/Math/Vec2.hpp"
#include "BaseMapGenerator.hpp"

class Map;


/////////////////////////////////////////////////////////////////////////////
class CellularAutomataMapGenerator : public BaseMapGenerator
{
public:
	static BaseMapGenerator* CreateGenerator( const std::string& name );
	~CellularAutomataMapGenerator() {};

	Map* GenerateMap();
	void FinishGeneration( Map& map );
	void Startup();
	void Shutdown();
	void PlacePlayer( Map& map, Player& player );
	void ProcessOneStep( Map& map );
	int CalculateNumberOfWallsAroundPoint( Map& map, Vec2i& coords );

protected:
	CellularAutomataMapGenerator() : BaseMapGenerator( "none" ) {}
	CellularAutomataMapGenerator( std::string name ) : BaseMapGenerator( name ) {}

protected:
	static MapGeneratorRegistration		s_cellularAutomataRegistration;

};

#endif