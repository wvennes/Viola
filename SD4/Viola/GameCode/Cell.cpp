//---------------------------------------------------------------------------
// Cell.cpp
//---------------------------------------------------------------------------

//#include "Engine/Math/Colors.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Agent.hpp"
#include "Feature.hpp"
#include "Item.hpp"
#include "CellTypes.hpp"
#include "Cell.hpp"


//---------------------------------------------------------------------------
Cell::Cell()
	: m_properties( CellTypes::FLOOR )
	, m_hasBeenVisited( false )
	, m_occupyingAgent( nullptr )
	, m_occupyingFeature( nullptr )
	, m_cellVisibility( 0.f )
	, m_isCellVisible( false )
{
	//m_properties = CellProperties( TYPE_FLOOR, false, Colors::WHITE, ' ' );
}


//---------------------------------------------------------------------------
Cell::Cell( const Cell& otherCell )
	: m_properties( otherCell.GetCellProperties() )
	, m_hasBeenVisited( otherCell.m_hasBeenVisited )
	, m_occupyingAgent( otherCell.GetOccupyingAgent() )
	, m_occupyingFeature( otherCell.GetOccupyingFeature() )
	, m_cellVisibility( otherCell.GetCellVisibility() )
	, m_isCellVisible( otherCell.m_isCellVisible )
{

}


//---------------------------------------------------------------------------
Cell::Cell( const int type, const Rgba& color, const unsigned char glyph, const unsigned char legendGlyph )
	: m_properties( CellProperties( type, color, glyph, legendGlyph ) )
	, m_hasBeenVisited( false )
	, m_occupyingAgent( nullptr )
	, m_occupyingFeature( nullptr )
	, m_cellVisibility( 0.f )
	, m_isCellVisible( false )
{
	//m_properties = CellProperties( type, isSolid, color, glyph );
}


//---------------------------------------------------------------------------
Cell::Cell(  const CellProperties& properties )
	: m_properties( properties )
	, m_hasBeenVisited( false )
	, m_occupyingAgent( nullptr )
	, m_occupyingFeature( nullptr )
	, m_cellVisibility( 0.f )
	, m_isCellVisible( false )
{

}


//---------------------------------------------------------------------------
Cell::~Cell()
{
	DestroyVector( m_occupyingItems );
// 	if ( !m_occupyingItems.empty() )
// 	{
// 		for ( unsigned int index = 0; index < ( unsigned int ) m_occupyingItems.size(); ++ index )
// 		{
// 			// delete the item if not null
// 			if ( m_occupyingItems[ index ] ) delete m_occupyingItems[ index ];
// 		}
// 	}

	//if ( m_occupyingFeature != nullptr ) delete m_occupyingFeature;
}


//---------------------------------------------------------------------------
void Cell::SetCellProperties( const int type, const Rgba& color, const unsigned char glyph, const unsigned char legendGlyph )
{
	m_properties = CellProperties( type, color, glyph, legendGlyph );
}


//---------------------------------------------------------------------------
void Cell::SetCellProperties( const CellProperties& properties )
{
	m_properties = properties;
}


//---------------------------------------------------------------------------
void Cell::SetOccupyingAgent( Agent* agentToPlace )
{
	m_occupyingAgent = agentToPlace;
}


//---------------------------------------------------------------------------
void Cell::SetOccupyingFeature( Feature* featureToPlace )
{
	m_occupyingFeature = featureToPlace;
}


//---------------------------------------------------------------------------
void Cell::SetIsCellVisible( bool isVisible )
{
	m_isCellVisible = isVisible;
}


//---------------------------------------------------------------------------
void Cell::SetCellVisibility( float visibility )
{
	m_cellVisibility = visibility;
}


//---------------------------------------------------------------------------
bool Cell::GiveItemToAgent( Agent* agent )
{
	// put functionality here!
	if ( !m_occupyingItems.empty() )
	{
		agent->GiveItem( m_occupyingItems.back() );
		m_occupyingItems.pop_back();
		return true;
	}
	return false;
}


//---------------------------------------------------------------------------
void Cell::AddOccupyingItem( Item* item )
{
	m_occupyingItems.push_back( item );
}


//---------------------------------------------------------------------------
bool Cell::DoesCellHaveItems() const
{
	return !m_occupyingItems.empty();
}


//---------------------------------------------------------------------------
bool Cell::IsType( int type ) const
{
	return ( ( m_properties.m_type & type ) == type );
}


//---------------------------------------------------------------------------
const OccupyingItems* Cell::GetOccupyingItems()
{
	return &m_occupyingItems;
}


//---------------------------------------------------------------------------
float Cell::GetCellVisibility() const
{
	return m_cellVisibility;
}


//---------------------------------------------------------------------------
bool Cell::IsCellVisible() const
{
	return m_cellVisibility > 0.3f;
}


//---------------------------------------------------------------------------
bool Cell::IsCellOccupiedByAgent() const
{
	return ( m_occupyingAgent != nullptr );
}


//---------------------------------------------------------------------------
bool Cell::IsCellOccupiedByFeature() const
{
	return ( m_occupyingFeature != nullptr );
}


//---------------------------------------------------------------------------
bool Cell::IsBlockedByCell( const int movementProperties ) const
{
	const int& terrainProperties = m_properties.m_type;
	if ( ( terrainProperties & movementProperties ) > 0 ) return true;
	return false;
	//return m_properties.m_isSolid;
}


//---------------------------------------------------------------------------
char Cell::GetCellGlyph() const
{
	return m_properties.m_glyph;
}


//---------------------------------------------------------------------------
int Cell::GetCellType() const
{
	return m_properties.m_type;
}


//---------------------------------------------------------------------------
Rgba Cell::GetCellColor() const
{
	return m_properties.m_color;
}


//---------------------------------------------------------------------------
Agent* Cell::GetOccupyingAgent() const
{
	return m_occupyingAgent;
}


//---------------------------------------------------------------------------
Feature* Cell::GetOccupyingFeature() const
{
	return m_occupyingFeature;
}