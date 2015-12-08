//---------------------------------------------------------------------------
// Item.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/TinyXMLHelper.hpp"
#include "Map.hpp"
#include "Cell.hpp"
#include "Item.hpp"


//---------------------------------------------------------------------------
Item::Item( const std::string& name, char glyph, ItemType itemType )
	: GameEntity( name, glyph )
	, m_itemType( itemType )
	, m_defense( 0.f )
	, m_numUses( 0 )
{

}


//---------------------------------------------------------------------------
Item::~Item()
{

}


//---------------------------------------------------------------------------
void Item::AddToMap( Map* map )
{
	GameEntity::AddToMap( map );

	Cell* cell = m_map->GetCell( m_position );
	if ( cell == nullptr ) return;
	cell->AddOccupyingItem( this );
}


//---------------------------------------------------------------------------
void Item::UseItem()
{
	-- m_numUses;
}


//---------------------------------------------------------------------------
void Item::SaveToXML( TiXmlNode* node )
{
	TiXmlElement* item = new TiXmlElement( "Item" );
	
	item->SetAttribute( "id", m_id );
	item->SetAttribute( "name", m_name.c_str() );
	item->SetAttribute( "numUses", m_numUses );
	item->SetAttribute( "x", m_position.x );
	item->SetAttribute( "y", m_position.y );

	node->LinkEndChild( item );
}


//---------------------------------------------------------------------------
void Item::LoadFromXML( TiXmlElement* node )
{
	m_numUses = GetAttributeAsInt( node, "numUses" );
	m_position.x = GetAttributeAsInt( node, "x" );
	m_position.y = GetAttributeAsInt( node, "y" );
}


//---------------------------------------------------------------------------
bool Item::IsDead() const
{
	return ( m_currentHealth <= 0.f ) || ( m_numUses <= 0 );
}