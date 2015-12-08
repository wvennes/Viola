//---------------------------------------------------------------------------
// LabyrinthGenerator.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_GRAVEYARDGENERATOR_
#define _INCLUDED_GRAVEYARDGENERATOR_

//#include "Engine/Math/Vec2.hpp"
#include <string>
#include "BaseMapGenerator.hpp"

class Map;


/////////////////////////////////////////////////////////////////////////////
class GraveyardGenerator : public BaseMapGenerator
{
public:
	static BaseMapGenerator* CreateGenerator( const std::string& name );
	~GraveyardGenerator();

	Map*								GenerateMap();
	void								FinishGeneration( Map& map );
	void								Startup();
	void								Shutdown();
	void								PlacePlayer( Map& map, Player& player );
	void								ProcessOneStep( Map& map );

protected:
	GraveyardGenerator();
	GraveyardGenerator( const std::string& name );

	CellCoords							GetLocationForRoom( Map& map );
	void								PlaceDoor( const CellCoords& location, Map& map );
	void								PlaceKeyRoom( const CellCoords& location, Map& map );
	void								PlaceItemRoom( const CellCoords& location, Map& map );
	void								PlaceGraves( Map& map );

protected:
	static const unsigned int			MAX_NUM_ROOMS;
	int									m_roomsPlaced;
	static MapGeneratorRegistration		s_graveyardGeneratorRegistration;

};

#endif