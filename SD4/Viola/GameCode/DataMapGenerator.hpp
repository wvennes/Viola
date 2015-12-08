//---------------------------------------------------------------------------
// LabyrinthGenerator.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_DATAMAPGENERATOR_
#define _INCLUDED_DATAMAPGENERATOR_

//#include "Engine/Math/Vec2.hpp"
#include <string>
#include "BaseMapGenerator.hpp"

class Map;
class NPC;


/////////////////////////////////////////////////////////////////////////////
class DataMapGenerator : public BaseMapGenerator
{
public:
	static BaseMapGenerator* CreateGenerator( const std::string& name );
	~DataMapGenerator() {};

	Map*								GenerateMap();
	void								FinishGeneration( Map& map );
	void								Startup();
	void								Shutdown();
	void								PlacePlayer( Map& map, Player& player );
	void								LoadFromFile( TiXmlNode* node, Map& map, Player& player );
	void								ProcessOneStep( Map& map );

protected:
	DataMapGenerator() : BaseMapGenerator( "none" ) {}
	DataMapGenerator( std::string name ) : BaseMapGenerator( name ) {}

	void								LoadTerrainData( TiXmlNode* root, Map& map );
	void								LoadVisibilityData( TiXmlNode* root, Map& map );
	void								LoadFeatures( TiXmlNode* root, Map& map );
	void								LoadAndPlacePlayer( TiXmlNode* root, Player& player, Map& map );
	void								LoadNPCs( TiXmlNode* root, std::vector< NPC* >& npcs );
	void								LoadItems( TiXmlNode* root, std::map< unsigned int, Item* >& items );

protected:
	static MapGeneratorRegistration		s_dataMapGeneratorRegistration;

};

#endif