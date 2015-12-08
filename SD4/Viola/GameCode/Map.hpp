//---------------------------------------------------------------------------
// Map.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_MAP_
#define _INCLUDED_MAP_

#include <vector>
#include <map>
#include "Engine/Math/Vec2.hpp"
#include "GameCommon.hpp"
#include "Cell.hpp"

class VennRenderer;
class TextRenderer;
class NPC;
class Item;
class MessageBar;
class TiXmlNode;

typedef Vec2i								CellCoords;
typedef unsigned int						CellIndex;
typedef std::vector< Cell >					CellVector;
typedef std::map< unsigned int, NPC* >		NPCMap;
typedef std::map< unsigned int, Feature* >	FeatureMap;

/////////////////////////////////////////////////////////////////////////////
class Map
{
public:
	CellVector				m_cells;

public:
	Map();
	Map( const Map& otherMap );
	~Map();

	CellCoords				GetCellCoordsForCellIndex( const unsigned int index );
	CellIndex				GetCellIndexForCellCoords( const CellCoords& cellCoords );

	Cell*					GetCell( const CellCoords& coordinates );
	bool					IsLocationInMap( const CellCoords& coordinates );
	CellCoords				GetRandomValidLocationInMap( int movementProperties );

	void					GetSurroundingNonBlockedCoordinates( const CellCoords& coordinates, int movementProperties, std::vector< Vec2i >& outCoordinates );
	void					AddNPCToRegistry( NPC* npc );
	void					AddFeatureToRegistry( Feature* feature );
	void					RemoveNPCFromRegistry( unsigned int id );
	void					RemoveFeatureFromRegistry( unsigned int id );
	void					ClearCellVisibilityFlags();
	void					SetIsMapVisible( bool isVisible );
	void					SendFactionAttackEvent( const Agent* attacker, const Agent* defender );

	void					Update();
	void					Render();

	void					SaveMapToXML( TiXmlNode* node, std::vector< Item* >& itemList );
	void					SaveMapCellsToXML( TiXmlNode* node, std::vector< Item* >& itemList );
	void					SaveNPCsToXML( TiXmlNode* node, std::vector< Item* >& itemList );

private:
	bool					m_isMapVisible;
	NPCMap					m_npcRegistry;
	FeatureMap				m_featureRegistry;

private:
	void					Initialize();

};

extern VennRenderer* g_theRenderer;
extern TextRenderer* g_theTextRenderer;
extern unsigned int g_currentShaderProgramID;
extern MessageBar* g_gameMessageLog;


//---------------------------------------------------------------------------
inline CellCoords Map::GetCellCoordsForCellIndex( const CellIndex index )
{
	return CellCoords( index % MAP_WIDTH_CELLS, index / MAP_WIDTH_CELLS );
}


//---------------------------------------------------------------------------
inline CellIndex Map::GetCellIndexForCellCoords( const CellCoords& cellCoords )
{
	return cellCoords.y * MAP_WIDTH_CELLS + cellCoords.x;
}


//---------------------------------------------------------------------------
inline Cell* Map::GetCell( const CellCoords& coordinates )
{
	if ( !IsLocationInMap( coordinates ) ) return nullptr;

	int x = GetCellIndexForCellCoords( coordinates );

	return &m_cells.at( x );
}

//---------------------------------------------------------------------------
inline bool Map::IsLocationInMap( const CellCoords& coordinates )
{
	return ( coordinates.x >= 0 && coordinates.y >= 0 && coordinates.x < MAP_WIDTH_CELLS && coordinates.y < MAP_HEIGHT_CELLS );
}


#endif