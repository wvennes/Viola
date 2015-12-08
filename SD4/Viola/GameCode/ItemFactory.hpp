//---------------------------------------------------------------------------
// ItemFactory.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_ITEMFACTORY_
#define _INCLUDED_ITEMFACTORY_

#include <set>
#include <vector>
#include "Item.hpp"

class ItemFactory;
class TiXmlElement;
class BaseAIBehavior;

typedef std::set< ItemFactory* >	ItemFactorySet;

/////////////////////////////////////////////////////////////////////////////
class ItemFactory
{
public:
	ItemFactory( TiXmlElement* factoryBlueprintNode );
	~ItemFactory();

	static void						DestroyFactories();
	void							PopulateFromXML( TiXmlElement* factoryBlueprintNode );
	static bool						DoesFactoryExist( const std::string& name );
	Item*							SpawnItem();

	std::string						GetName() { return m_name; }
	static ItemFactory*				GetFactory( const std::string& name );
	static ItemFactory*				GetRandomFactory();

	static ItemType					GetAttributeAsItemType( const TiXmlElement* node, const char* name );

protected:
	std::string						m_name;
	char							m_glyph;
	ItemType						m_itemType;
	int								m_numUses;
	float							m_attack;
	float							m_defense;
	Rgba							m_color;
	static ItemFactorySet			s_factorySet;

};

#endif