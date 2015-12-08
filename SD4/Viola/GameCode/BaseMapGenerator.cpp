//---------------------------------------------------------------------------
// BaseMapGenerator.cpp
//---------------------------------------------------------------------------

#include "Engine/Math/Colors.hpp"
#include "NPCFactory.hpp"
#include "ItemFactory.hpp"
#include "FeatureFactory.hpp"
#include "Item.hpp"
#include "NPC.hpp"
#include "Player.hpp"
#include "BaseMapGenerator.hpp"
#include "Cell.hpp"
#include "CellTypes.hpp"
#include "Map.hpp"
#define STATIC
#define UNUSED( x ) ( void ) ( x )

STATIC MapGeneratorRegistrationMap* MapGeneratorRegistration::s_generatorRegistrationMap = new MapGeneratorRegistrationMap();
STATIC MapGeneratorMap BaseMapGenerator::s_mapGeneratorMap;;


//---------------------------------------------------------------------------
STATIC void BaseMapGenerator::CreateGenerators()
{
	MapGeneratorRegistration* curentRegistration = nullptr;
	MapGeneratorRegistrationMap* generatorRegistrationMap = MapGeneratorRegistration::GetGeneratorRegistrationMap();

	if ( generatorRegistrationMap )
	{
		for ( MapGeneratorRegistrationMap::iterator mapIter = generatorRegistrationMap->begin(); mapIter != generatorRegistrationMap->end(); ++ mapIter )
		{
			curentRegistration = mapIter->second;
			if ( s_mapGeneratorMap.find( curentRegistration->GetName() ) == s_mapGeneratorMap.end() )
			{
				s_mapGeneratorMap.insert( std::make_pair( curentRegistration->GetName(), curentRegistration->CreateMapGenerator() ) );
			}
			//s_mapGeneratorMap[ curentRegistration->GetName() ] = curentRegistration->CreateMapGenerator();
		}
	}
}


//---------------------------------------------------------------------------
STATIC BaseMapGenerator* BaseMapGenerator::GetGenerator( const std::string& name )
{
	return s_mapGeneratorMap[ name ];
}


//---------------------------------------------------------------------------
STATIC void BaseMapGenerator::DestroyGenerators()
{
	for ( MapGeneratorMap::iterator mapIter = s_mapGeneratorMap.begin(); mapIter != s_mapGeneratorMap.end(); ++ mapIter )
	{
		delete mapIter->second;
		//mapIter = s_mapGeneratorMap.erase( mapIter++ );
	}

	s_mapGeneratorMap.clear();

	delete MapGeneratorRegistration::GetGeneratorRegistrationMap();
}


//---------------------------------------------------------------------------
void BaseMapGenerator::LoadFromFile( TiXmlNode* node, Map& map, Player& player )
{
	UNUSED( node );
	UNUSED( map );
	UNUSED( player );
}


//---------------------------------------------------------------------------
void BaseMapGenerator::PlaceFeature( Map& map, const Vec2i& location, const std::string& featureName )
{
	Feature* feature = FeatureFactory::GetFactory( featureName )->SpawnFeature();
	feature->SetPosition( location );
	feature->AddToMap( &map );
}


//---------------------------------------------------------------------------
void BaseMapGenerator::PlaceRandomItem( Map& map )
{
	//return;
	//int index = rand() % ItemFactory::
	Item* item = ItemFactory::GetRandomFactory()->SpawnItem();
	item->SetPosition( map.GetRandomValidLocationInMap( IS_BLOCKED_BY_WALLS | IS_BLOCKED_BY_FIRE | IS_BLOCKED_BY_DOORS ) );
	item->AddToMap( &map );
}


//---------------------------------------------------------------------------
void BaseMapGenerator::PlaceRandomItemAtLocation( const CellCoords& location, Map& map )
{
	//return;
	Item* item = ItemFactory::GetRandomFactory()->SpawnItem();
	item->SetPosition( location );
	item->AddToMap( &map );
}


//---------------------------------------------------------------------------
void BaseMapGenerator::PlaceNPCAtRandomLocation( const std::string& npcName, Map& map )
{
	//return;
	if ( !NPCFactory::DoesFactoryExist( npcName ) ) return;

	NPC* npc = NPCFactory::GetFactory( npcName )->SpawnNPC();
	npc->AddToMap( &map );

	CellCoords coords = map.GetRandomValidLocationInMap( npc->GetMovementPropertes() );

	npc->SetPosition( coords );
	Cell* cell = map.GetCell( coords );
	if ( cell == nullptr ) return;
	cell->SetOccupyingAgent( npc );
}


//---------------------------------------------------------------------------
void BaseMapGenerator::PlaceNPCAtLocation( const CellCoords& location, const std::string& npcName, Map& map )
{
	//return;
	if ( !NPCFactory::DoesFactoryExist( npcName ) ) return;

	NPC* npc = NPCFactory::GetFactory( npcName )->SpawnNPC();

	npc->SetPosition( location );
	npc->AddToMap( &map );
}


//---------------------------------------------------------------------------
void BaseMapGenerator::InitializeRandomData( Map& map )
{
	//map = new Map();
	float randomValue;
	float oneOverRandMax = 1.f / RAND_MAX;

	for ( unsigned int index = 0; index < map.m_cells.size(); ++ index )
	{
		Cell& currentCell = map.m_cells.at( index );
		randomValue = ( float ) ( std::rand() ) * oneOverRandMax;

		if ( randomValue <= 0.45f )
		{
			currentCell.SetCellProperties( CellTypes::FLOOR );
		}
		else
		{
			currentCell.SetCellProperties( CellTypes::WALL );
		}
	}
}