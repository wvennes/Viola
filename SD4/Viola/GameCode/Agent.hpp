//---------------------------------------------------------------------------
// Agent.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_AGENT_
#define _INCLUDED_AGENT_

#include <vector>
#include <map>
#include "MovementProperties.hpp"
#include "GameEntity.hpp"

class TiXmlElement;
class Feature;
class MessageBar;
class Item;
class Faction;

typedef std::vector< Item* >	Inventory;
typedef std::vector< Item* >	Weapons;
typedef std::vector< Item* >	Armors;

/////////////////////////////////////////////////////////////////////////////
class Agent : public GameEntity
{
public:
	Agent();
	Agent( const Agent& otherAgent );
	Agent( const std::string& name, char glyph );
	//Agent( const Entity& otherEntity );
	virtual ~Agent();

	Faction*			GetFaction() const { return m_faction; }
	bool				IsAgentInMap() const { return m_isInMap; }
	int					GetMovementSpeed() const { return m_movementSpeed; }
	int					GetMovementPropertes() const { return m_movementProperties; }

	void				SetFaction( Faction* faction );
	void				SetIsInMap( bool isInMap ) { m_isInMap = isInMap; }
	void				ToggleIsInMap() { m_isInMap = !m_isInMap; }
	void				SetMovementSpeed( int movementSpeed ) { m_movementSpeed = movementSpeed; }
	void				SetMovementProperties( int movementProperties ) { m_movementProperties = movementProperties; }
	bool				CanMoveToLocation( Map* map, const Vec2i& coordinates );
	virtual float		ApplyDamage( float damageToInflict );
	virtual bool		UsePotion();
	virtual void		GiveItem( Item* item );
	virtual bool		Update();
	virtual bool		PickUpItem();
	virtual void		ActivateFeature( Feature* feature );
	virtual void		MoveAgent( Map* map, const Vec2i& coordinates );
	virtual void		RemoveFromMap();
	virtual Item*		GetNextInventoryItem();
	virtual Item*		GetEquippedWeapon() { return m_equippedWeapon; }
	virtual Item*		GetEquippedArmor() { return m_equippedArmor; }
	virtual void		SaveToXMLAndPopulateItemList( TiXmlNode* node, std::vector< Item* >& itemList );
	virtual void		LoadFromXML( TiXmlElement* node );
	virtual void		LinkItemsFromXMLItemList( std::map< unsigned int, Item* >& itemList );
	//void				AddToMap( Map* map ) {}

protected:
	//std::string			m_faction;
	bool				m_isInMap;
	int					m_movementSpeed;
	int					m_movementProperties;
	Faction*			m_faction;
	Item*				m_equippedWeapon;
	Item*				m_equippedArmor;
	Inventory			m_inventory;
	Weapons				m_weapons;
	Armors				m_armors;

};

extern MessageBar* g_gameMessageLog;

#endif