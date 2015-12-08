//---------------------------------------------------------------------------
// Item.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_ITEM_
#define _INCLUDED_ITEM_

#include "GameEntity.hpp"

/////////////////////////////////////////////////////////////////////////////
enum ItemType
{
	ITEM_TYPE_UNDEFINED,
	ITEM_TYPE_POTION,
	ITEM_TYPE_WEAPON,
	ITEM_TYPE_ARMOR,
	NUM_ITEM_TYPES
};


/////////////////////////////////////////////////////////////////////////////
class Item : public GameEntity
{
public:
	Item( const std::string& name, char glyph, ItemType itemType );
	~Item();

	ItemType		GetItemType() const { return m_itemType; }
	float			GetItemDefense() const { return m_defense; }
	float			GetItemAttack() const { return GetBaseAttackDamage(); }
	int				GetItemUses() const { return m_numUses; }

	void			SetItemType( ItemType itemType ) { m_itemType = itemType; }
	void			SetItemDefense( float defense ) { m_defense = defense; }
	void			SetItemAttack( float attack ) { SetBaseAttackDamage( attack ); }
	void			SetNumUses( int numUses ) { m_numUses = numUses; }
	void			AddToMap( Map* map );
	void			UseItem();
	void			SaveToXML( TiXmlNode* node );
	void			LoadFromXML( TiXmlElement* node );

	bool			IsDead() const;
protected:
	ItemType		m_itemType;
	int				m_numUses;
	float			m_defense;

};

#endif