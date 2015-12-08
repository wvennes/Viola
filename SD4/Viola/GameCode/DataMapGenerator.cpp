//---------------------------------------------------------------------------
// DataMapGenerator.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/TinyXMLHelper.hpp"
#include "GameCommon.hpp"
#include "CellTypes.hpp"
#include "Map.hpp"
#include "NPC.hpp"
#include "Item.hpp"
#include "NPCFactory.hpp"
#include "FeatureFactory.hpp"
#include "ItemFactory.hpp"
#include "Feature.hpp"
#include "Player.hpp"
#include "DataMapGenerator.hpp"
#define STATIC

STATIC MapGeneratorRegistration DataMapGenerator::s_dataMapGeneratorRegistration( "Data", CreateGenerator );


//---------------------------------------------------------------------------
STATIC BaseMapGenerator* DataMapGenerator::CreateGenerator( const std::string& name )
{
	return new DataMapGenerator( name );
}


//---------------------------------------------------------------------------
Map* DataMapGenerator::GenerateMap()
{
	return new Map();
}


//---------------------------------------------------------------------------
void DataMapGenerator::Startup()
{

}


//---------------------------------------------------------------------------
void DataMapGenerator::Shutdown()
{

}


//---------------------------------------------------------------------------
void DataMapGenerator::LoadFromFile( TiXmlNode* node, Map& map, Player& player )
{
	std::vector< NPC* > npcs;
	std::map< unsigned int, Item* > items;
	// load terrain data
	LoadTerrainData( node, map );

	// load visibility data
	LoadVisibilityData( node, map );

	// load features
	LoadFeatures( node, map );

	// load npcs
	LoadNPCs( node, npcs );

	// load the player
	LoadAndPlacePlayer( node, player, map );

	// load items
	LoadItems( node, items );

	// give npcs items and place npcs in the world
	for ( unsigned int index = 0; index < ( unsigned int ) npcs.size(); ++ index )
	{
		NPC* npc = npcs.at( index );
		npc->LinkItemsFromXMLItemList( items );
		npc->AddToMap( &map );

		Cell* cell = map.GetCell( npc->GetPosition() );
		if ( cell == nullptr ) return;
		cell->SetOccupyingAgent( npc );
	}

	// give items to player
	player.LinkItemsFromXMLItemList( items );

	// place remaining items in the world
	std::map< unsigned int, Item* >::iterator iter;
	for ( iter = items.begin(); iter != items.end(); ++ iter )
	{
		iter->second->AddToMap( &map );
	}

// 	TiXmlDocument xmlDocument;
// 	bool isDocumentLoaded = xmlDocument.LoadFile( filePath );
// 	CellCoords currentCoords;
// 
// 	if ( !isDocumentLoaded ) return;
// 
// 	TiXmlElement* rootElement = xmlDocument.RootElement();
// 	TiXmlElement* dataElement = rootElement->FirstChildElement( "mapdata" );
// 
// 	const char* mapData = dataElement->GetText();
// 
// 	for ( int y = MAP_HEIGHT_CELLS - 1; y >= 0; -- y )
// 	{
// 		for ( int x = 0; x < MAP_WIDTH_CELLS; ++ x )
// 		{
// 			currentCoords = CellCoords( x, y );
// 
// 			if ( isspace( *mapData ) ) ++ mapData;
// 
// 			Cell* currentCell = map.GetCell( currentCoords );
// 
// 			if ( currentCell != nullptr )
// 			{
// 				switch ( *mapData )
// 				{
// 				case '#':
// 					{
// 						currentCell->SetCellProperties( CellTypes::WALL );
// 						break;
// 					}
// 				case '.':
// 					{
// 						currentCell->SetCellProperties( CellTypes::FLOOR );
// 						break;
// 					}
// 				default:
// 					{
// 						currentCell->SetCellProperties( CellTypes::FLOOR );
// 						break;
// 					}
// 				}
// 			}
// 
// 			++ mapData;
// 		}
// 	}
}


//---------------------------------------------------------------------------
void DataMapGenerator::LoadTerrainData( TiXmlNode* root, Map& map )
{
	std::map< unsigned char, unsigned char > legend;
	std::map< unsigned char, Rgba > colorLegend;
	TiXmlElement* legendElement = root->FirstChildElement( "Legend" );
	for ( TiXmlElement* legendGlyphElement = legendElement->FirstChildElement( "Glyph" ); legendGlyphElement != nullptr; legendGlyphElement = legendGlyphElement->NextSiblingElement( "Glyph" ) )
	{
		unsigned char legendGlyph = ( unsigned char ) GetAttributeAsUnsignedInt( legendGlyphElement, "legend" );
		unsigned char glyph = ( unsigned char ) GetAttributeAsUnsignedInt( legendGlyphElement, "glyph" );
		Rgba color;
		color.r = ( unsigned char ) GetAttributeAsInt( legendGlyphElement, "r" );
		color.g = ( unsigned char ) GetAttributeAsInt( legendGlyphElement, "g" );
		color.b = ( unsigned char ) GetAttributeAsInt( legendGlyphElement, "b" );
		color.a = 255;

		legend.insert( std::make_pair( legendGlyph, glyph ) );
		colorLegend.insert( std::make_pair( legendGlyph, color ) );
	}

	// load map cells
	CellCoords mapCoordinates( 0, MAP_HEIGHT_CELLS - 1 );
	TiXmlElement* terrainData = root->FirstChildElement( "TerrainData" );
	for ( TiXmlElement* terrain = terrainData->FirstChildElement( "Terrain" ); terrain != nullptr; terrain = terrain->NextSiblingElement( "Terrain" ) )
	{
		std::string terrainString( terrain->GetText() );
		for ( unsigned int index = 0; index < terrainString.length(); ++ index )
		{
			unsigned char legendGlyph = terrainString.at( index );
			unsigned char glyph = legend.at( legendGlyph ); // translate to the proper glyph
			Rgba color = colorLegend.at( legendGlyph ); // find the color for this glyph

			Cell* cell = map.GetCell( mapCoordinates );

			if ( cell != nullptr )
			{
				switch ( glyph )
				{
					case '.':
					{
						cell->SetCellProperties( TYPE_AIR, color, glyph, legendGlyph );
						break;
					}
					case '#':
					{
						cell->SetCellProperties( TYPE_WALL, color, glyph, legendGlyph );
						break;
					}
					case '~':
					{
						cell->SetCellProperties( TYPE_WATER | TYPE_FIRE, color, glyph, legendGlyph );
						break;
					}
					case '-':
					{
						cell->SetCellProperties( TYPE_WATER | TYPE_AIR, color, glyph, legendGlyph );
						break;
					}
				}
			}

			++ mapCoordinates.x;
		}
		mapCoordinates.x = 0;
		-- mapCoordinates.y;
	}
}


//---------------------------------------------------------------------------
void DataMapGenerator::LoadVisibilityData( TiXmlNode* root, Map& map )
{
	CellCoords mapCoordinates( 0, MAP_HEIGHT_CELLS - 1 );
	TiXmlElement* visibility = root->FirstChildElement( "VisibilityData" );
	for ( TiXmlElement* visibilityData = visibility->FirstChildElement( "Visibility" ); visibilityData != nullptr; visibilityData = visibilityData->NextSiblingElement( "Visibility" ) )
	{
		std::string data( visibilityData->GetText() );

		for ( unsigned int index = 0; index < data.length(); ++ index )
		{
			Cell* cell = map.GetCell( mapCoordinates );
			if ( cell == nullptr ) continue;

			unsigned char glyph = data.at( index );

			if ( glyph == 'V' )
			{
				cell->SetCellVisibility( 0.2f );
			}
			else
			{
				cell->SetCellVisibility( 0.f );
			}
			++ mapCoordinates.x;
		}
		mapCoordinates.x = 0;
		-- mapCoordinates.y;
	}
}


//---------------------------------------------------------------------------
void DataMapGenerator::LoadFeatures( TiXmlNode* root, Map& map )
{
	TiXmlElement* features = root->FirstChildElement( "Features" );
	if ( features == nullptr ) return;
	for ( TiXmlElement* currentFeatureNode = features->FirstChildElement( "Feature" ); currentFeatureNode != nullptr; currentFeatureNode = currentFeatureNode->NextSiblingElement( "Feature" ) )
	{
		FeatureFactory* factory = FeatureFactory::GetFactory( GetAttributeAsString( currentFeatureNode, "name" ) );
		Feature* feature = factory->SpawnFeature();
		CellCoords position( GetAttributeAsUnsignedInt( currentFeatureNode, "x" ), GetAttributeAsUnsignedInt( currentFeatureNode, "y" ) );

		feature->SetPosition( position );
		feature->SetActivatedGlyph( ( unsigned char ) GetAttributeAsUnsignedInt( currentFeatureNode, "activated" ) );
		feature->SetDeactivatedGlyph( ( unsigned char ) GetAttributeAsUnsignedInt( currentFeatureNode, "deactivated" ) );
		int isActivated = GetAttributeAsInt( currentFeatureNode, "isActivated" );
		if ( isActivated > 0 )
		{
			feature->UseFeature();
		}

		feature->AddToMap( &map );
	}
}


//---------------------------------------------------------------------------
void DataMapGenerator::LoadAndPlacePlayer( TiXmlNode* root, Player& player, Map& map )
{
	TiXmlElement* playerDataNode = root->FirstChildElement( "PlayerData" );
	TiXmlElement* playerNode = playerDataNode->FirstChildElement( "Player" );
	player.LoadFromXML( playerNode );
	Cell* cell = map.GetCell( player.GetPosition() );
	if ( cell == nullptr ) return;
	cell->SetOccupyingAgent( &player );
	player.SetPosition( player.GetPosition() ); // set the lighting by resetting player's position
}


//---------------------------------------------------------------------------
void DataMapGenerator::LoadNPCs( TiXmlNode* root, std::vector< NPC* >& npcs )
{
	TiXmlElement* npcRootNode = root->FirstChildElement( "NPCs" );
	if ( npcRootNode == nullptr ) return;
	for ( TiXmlElement* npcNode = npcRootNode->FirstChildElement( "NPC" ); npcNode != nullptr; npcNode = npcNode->NextSiblingElement( "NPC" ) )
	{
		NPCFactory* factory = NPCFactory::GetFactory( GetAttributeAsString( npcNode, "name" ) );
		NPC* npc = factory->SpawnNPC();
		npc->LoadFromXML( npcNode );
		npcs.push_back( npc );
	}
}


//---------------------------------------------------------------------------
void DataMapGenerator::LoadItems( TiXmlNode* root, std::map< unsigned int, Item* >& items )
{
	TiXmlElement* itemRootNode = root->FirstChildElement( "Items" );
	if ( itemRootNode == nullptr ) return;
	for ( TiXmlElement* itemNode = itemRootNode->FirstChildElement( "Item" ); itemNode != nullptr; itemNode = itemNode->NextSiblingElement( "Item" ) )
	{
		ItemFactory* factory = ItemFactory::GetFactory( GetAttributeAsString( itemNode, "name" ) );
		Item* item = factory->SpawnItem();
		item->LoadFromXML( itemNode );

		items.insert( std::make_pair( GetAttributeAsUnsignedInt( itemNode, "id"), item ) );
	}
}


//---------------------------------------------------------------------------
void DataMapGenerator::PlacePlayer( Map& map, Player& player )
{
	CellCoords coords = map.GetRandomValidLocationInMap( player.GetMovementPropertes() );
	player.SetPosition( coords );
	Cell* cell = map.GetCell( coords );
	if ( cell != nullptr ) cell->SetOccupyingAgent( &player );
	player.AddToMap( &map );
}


//---------------------------------------------------------------------------
void DataMapGenerator::ProcessOneStep( Map& )
{
	// do nothing
}


//---------------------------------------------------------------------------
void DataMapGenerator::FinishGeneration( Map& )
{
	// do nothing
}