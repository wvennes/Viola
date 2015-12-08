//---------------------------------------------------------------------------
// Cell.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_CELL_
#define _INCLUDED_CELL_

#include <vector>
#include "Engine/Math/Colors.hpp"
#include "MovementProperties.hpp"

class Agent;
class Feature;
class Item;

typedef std::vector< Item* >	OccupyingItems;

/////////////////////////////////////////////////////////////////////////////
typedef enum
{
	TYPE_WALL		= Bit( 0 ),
	TYPE_AIR		= Bit( 1 ),
	TYPE_WATER		= Bit( 2 ),
	TYPE_DOOR		= Bit( 3 ),
	TYPE_FIRE		= Bit( 4 ),
} CellType;


/////////////////////////////////////////////////////////////////////////////
struct CellProperties
{
	int				m_type;
	Rgba			m_color;
	unsigned char	m_glyph;
	unsigned char	m_legendGlyph;

	CellProperties() : m_type( TYPE_AIR ), m_color( Colors::WHITE ), m_glyph( ' ' ), m_legendGlyph( ' ' ) {}
	CellProperties( const int type, const Rgba& color, const unsigned char glyph, const unsigned char legendGlyph ) : m_type( type ), m_color( color ), m_glyph( glyph ), m_legendGlyph( legendGlyph ) {}
	CellProperties( const CellProperties& otherProperties ) : m_type( otherProperties.m_type ), m_color( otherProperties.m_color ), m_glyph( otherProperties.m_glyph ), m_legendGlyph( otherProperties.m_legendGlyph ) {}
};


/////////////////////////////////////////////////////////////////////////////
class Cell
{
public:
	CellProperties			m_properties;
	bool					m_hasBeenVisited; // for generators

public:
	Cell();
	Cell( const int type, const Rgba& color, const unsigned char glyph, const unsigned char legendGlyph );
	Cell( const CellProperties& properties );
	Cell( const Cell& otherCell );
	~Cell();

	void					SetCellProperties( const int type, const Rgba& color, const unsigned char glyph, const unsigned char legendGlyph );
	void					SetCellProperties( const CellProperties& properties );
	void					SetOccupyingAgent( Agent* agentToPlace );
	void					SetOccupyingFeature( Feature* featureToPlace );
	void					SetIsCellVisible( bool isVisible );
	void					SetCellVisibility( float visibility );
	bool					GiveItemToAgent( Agent* agent );
	void					AddOccupyingItem( Item* item );
	bool					IsCellVisible() const;
	bool					IsCellOccupiedByAgent() const;
	bool					IsCellOccupiedByFeature() const;
	bool					IsBlockedByCell( const int movementProperties ) const;
	bool					DoesCellHaveItems() const;
	bool					IsType( int type ) const;
	const OccupyingItems*	GetOccupyingItems();
	float					GetCellVisibility() const;
	char					GetCellGlyph() const;
	int						GetCellType() const;
	Rgba					GetCellColor() const;
	Agent*					GetOccupyingAgent() const;
	Feature*				GetOccupyingFeature() const;
	CellProperties			GetCellProperties() const { return m_properties; }

private:
	Agent*					m_occupyingAgent;
	Feature*				m_occupyingFeature;
	OccupyingItems			m_occupyingItems;
	float					m_cellVisibility;
	bool					m_isCellVisible;

};

#endif