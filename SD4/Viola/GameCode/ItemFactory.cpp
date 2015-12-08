//---------------------------------------------------------------------------
// ItemFactory.cpp
//---------------------------------------------------------------------------

#include <iterator>
#include "Engine/External/TinyXML/tinyxml.h"
#include "Engine/Core/TinyXMLHelper.hpp"
#include "Engine/Core/Utility.hpp"
#include "ItemFactory.hpp"
#define STATIC

STATIC ItemFactorySet	ItemFactory::s_factorySet;


//---------------------------------------------------------------------------
STATIC ItemType ItemFactory::GetAttributeAsItemType( const TiXmlElement* element, const char* name )
{
	std::string value( element->Attribute( name ) );
	ToLower( value );

	if ( value == "potion" )	return ITEM_TYPE_POTION;
	if ( value == "weapon" )	return ITEM_TYPE_WEAPON;
	if ( value == "armor" )		return ITEM_TYPE_ARMOR;

	return ITEM_TYPE_UNDEFINED;
}


//---------------------------------------------------------------------------
ItemFactory::ItemFactory( TiXmlElement* factoryBlueprintNode )
	: m_name( "name" )
	, m_glyph( ' ' )
	, m_itemType( ITEM_TYPE_POTION )
	, m_numUses( 10 )
	, m_attack( -10.f )
	, m_defense( 0.f )
	, m_color( Colors::WHITE )
{
	PopulateFromXML( factoryBlueprintNode );

	s_factorySet.insert( this );
}


//---------------------------------------------------------------------------
ItemFactory::~ItemFactory()
{

}


//---------------------------------------------------------------------------
STATIC void ItemFactory::DestroyFactories()
{
	for ( ItemFactorySet::iterator iter = s_factorySet.begin(); iter != s_factorySet.end(); ++ iter )
	{
		delete *iter;
	}

	s_factorySet.clear();
}


//---------------------------------------------------------------------------
void ItemFactory::PopulateFromXML( TiXmlElement* factoryBlueprintNode )
{
	m_name = GetAttributeAsString( factoryBlueprintNode, "name" );
	m_glyph = GetAttributeAsChar( factoryBlueprintNode, "glyph" );
	m_itemType = GetAttributeAsItemType( factoryBlueprintNode, "type" );
	m_color.r = ( unsigned char ) GetAttributeAsInt( factoryBlueprintNode, "r" );
	m_color.g = ( unsigned char ) GetAttributeAsInt( factoryBlueprintNode, "g" );
	m_color.b = ( unsigned char ) GetAttributeAsInt( factoryBlueprintNode, "b" );
	m_numUses = GetAttributeAsInt( factoryBlueprintNode, "uses" );
	m_attack = GetAttributeAsFloat( factoryBlueprintNode, "attack" );
	m_defense = GetAttributeAsFloat( factoryBlueprintNode, "defense" );
}


//---------------------------------------------------------------------------
STATIC bool ItemFactory::DoesFactoryExist( const std::string& name )
{
	for ( ItemFactorySet::iterator iter = s_factorySet.begin(); iter != s_factorySet.end(); ++ iter )
	{
		ItemFactory* factory = *iter;
		if ( name == factory->GetName() )
		{
			return true;
		}
	}

	return false;
}


//---------------------------------------------------------------------------
STATIC ItemFactory* ItemFactory::GetFactory( const std::string& name )
{
	for ( ItemFactorySet::iterator iter = s_factorySet.begin(); iter != s_factorySet.end(); ++ iter )
	{
		ItemFactory* factory = *iter;
		if ( name == factory->GetName() )
		{
			return factory;
		}
	}

	return nullptr;
}


//---------------------------------------------------------------------------
STATIC ItemFactory* ItemFactory::GetRandomFactory()
{
	ItemFactorySet::iterator iter = s_factorySet.begin();
	std::advance( iter, rand() % s_factorySet.size() );
	return ( *iter );
}


//---------------------------------------------------------------------------
Item* ItemFactory::SpawnItem()
{
	Item* item = new Item( m_name, m_glyph, m_itemType );
	item->SetColor( m_color );
	item->SetNumUses( m_numUses );
	item->SetItemAttack( m_attack );
	item->SetItemDefense( m_defense );
	return item;
}