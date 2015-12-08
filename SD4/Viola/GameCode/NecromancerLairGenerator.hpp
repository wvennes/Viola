//---------------------------------------------------------------------------
// LabyrinthGenerator.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_NECROMANCERLAIRGENERATOR_
#define _INCLUDED_NECROMANCERLAIRGENERATOR_

//#include "Engine/Math/Vec2.hpp"
#include <string>
#include "BaseMapGenerator.hpp"

class Map;


/////////////////////////////////////////////////////////////////////////////
class NecromancerLairGenerator : public BaseMapGenerator
{
public:
	static BaseMapGenerator* CreateGenerator( const std::string& name );
	~NecromancerLairGenerator();

	Map*								GenerateMap();
	void								FinishGeneration( Map& map );
	void								Startup();
	void								Shutdown();
	void								PlacePlayer( Map& map, Player& player );
	void								ProcessOneStep( Map& map );

protected:
	NecromancerLairGenerator();
	NecromancerLairGenerator( const std::string& name );

	void								PlaceNecromancerRoom( Map& map );
	void								PlaceAllLavaRivers( Map& map );
	void								PlaceLavaRiver( Map& map );
	void								PlacePathToNecromancerRoom( Map& map, const Vec2i& leftHandCorner, const Vec2i& dimensions );
	void								PlaceCatacombs( Map& map );
	void								PlaceEntrance( Map& map, Player& player );
	void								PlaceEntranceHallway( Map& map, const Vec2i& leftHandCorner, const Vec2i& dimensions );

protected:
	static MapGeneratorRegistration		s_necromancerLairGeneratorRegistration;

};

#endif