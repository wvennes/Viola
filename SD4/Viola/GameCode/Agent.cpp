//---------------------------------------------------------------------------
// Agent.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/TinyXMLHelper.hpp"
#include "Map.hpp"
#include "Faction.hpp"
#include "Agent.hpp"
#include "Feature.hpp"
#include "Item.hpp"
#include "MessageBar.hpp"
#define UNUSED( x ) ( void ) ( x )


//---------------------------------------------------------------------------
Agent::Agent()
	: GameEntity()
	, m_isInMap( false )
	, m_movementSpeed( 1 )
	, m_movementProperties( IS_BLOCKED_BY_WALLS )
	, m_faction( nullptr )
	, m_equippedWeapon( nullptr )
	, m_equippedArmor( nullptr )
{

}


//---------------------------------------------------------------------------
Agent::Agent( const Agent& otherAgent )
	: GameEntity( otherAgent )
	, m_isInMap( otherAgent.IsAgentInMap() )
	, m_movementSpeed( otherAgent.GetMovementSpeed() )
	, m_movementProperties( otherAgent.GetMovementPropertes() )
	, m_faction( new Faction( *otherAgent.GetFaction() ) )
	, m_equippedWeapon( nullptr )
	, m_equippedArmor( nullptr )
{

}


//---------------------------------------------------------------------------
Agent::Agent( const std::string& name, char glyph )
	: GameEntity( name, glyph )
	, m_isInMap( false )
	, m_movementSpeed( 1 )
	, m_movementProperties( IS_BLOCKED_BY_WALLS )
	, m_faction( nullptr )
	, m_equippedWeapon( nullptr )
	, m_equippedArmor( nullptr )
{

}


//---------------------------------------------------------------------------
Agent::~Agent()
{
	DestroyVector( m_inventory );
	DestroyVector( m_weapons );
	DestroyVector( m_armors );

	if ( m_equippedArmor != nullptr ) delete m_equippedArmor;
	if ( m_equippedWeapon != nullptr ) delete m_equippedWeapon;
	if ( m_faction != nullptr ) delete m_faction;

	//GameEntity::~GameEntity();
}


//---------------------------------------------------------------------------
void Agent::SetFaction( Faction* faction )
{
	if ( m_faction != nullptr ) delete m_faction;
	m_faction = faction;
}


//---------------------------------------------------------------------------
float Agent::ApplyDamage( float damageToInflict )
{
	if ( damageToInflict < 0.f ) // separate path for healing
	{
		return GameEntity::ApplyDamage( damageToInflict );
	}

	float finalDamage = damageToInflict;
	if ( m_equippedArmor != nullptr )
	{
		finalDamage -= m_equippedArmor->GetItemDefense();
		if ( finalDamage < 0.f ) finalDamage = 0.f;
		m_equippedArmor->UseItem();

		if ( m_equippedArmor->IsDead() )
		{
			g_gameMessageLog->LogPrintf( Colors::PURPLE, "The %s's %s broke!", GetName().c_str(), m_equippedArmor->GetName().c_str() );
			delete m_equippedArmor;
			m_equippedArmor = nullptr;

			if ( !m_armors.empty() )
			{
				m_equippedArmor = m_armors.back();
				m_armors.pop_back();

				g_gameMessageLog->LogPrintf( Colors::PURPLE, "The %s equipped the %s!", GetName().c_str(), m_equippedArmor->GetName().c_str() );
			}
		}
	}

	return GameEntity::ApplyDamage( finalDamage );
}


//---------------------------------------------------------------------------
bool Agent::CanMoveToLocation( Map* map, const Vec2i& coordinates )
{
	Cell* cell = map->GetCell( coordinates );
	if ( cell != nullptr && 
		!cell->IsCellOccupiedByAgent() &&
		!cell->IsBlockedByCell( m_movementProperties ) )
	{
		if ( cell->IsCellOccupiedByFeature() && !cell->GetOccupyingFeature()->CanWalkOn() )
		{
			return false;
		}
		return true;
	}

	return false;
}


//---------------------------------------------------------------------------
bool Agent::UsePotion()
{
	if ( m_inventory.empty() ) return false;

	Item* currentItem = m_inventory.back();
	m_currentHealth -= currentItem->GetItemAttack();
	if ( m_currentHealth > m_maxHealth )
	{
		m_currentHealth = m_maxHealth;
	}
	currentItem->UseItem();
	if ( currentItem->IsDead() )
	{
		delete currentItem;
		m_inventory.pop_back();
	}
	return true;
}


//---------------------------------------------------------------------------
void Agent::GiveItem( Item* item )
{
	switch ( item->GetItemType() )
	{
		case ITEM_TYPE_POTION:
		{
			m_inventory.push_back( item );
			break;
		}
		case ITEM_TYPE_WEAPON:
		{
			if ( m_equippedWeapon == nullptr )
			{
				m_equippedWeapon = item;
			}
			else
			{
				m_weapons.push_back( item );
			}
			break;
		}
		case ITEM_TYPE_ARMOR:
		{
			if ( m_equippedArmor == nullptr )
			{
				m_equippedArmor = item;
			}
			else
			{
				m_armors.push_back( item );
			}
			break;
		}
	}
}


//---------------------------------------------------------------------------
bool Agent::Update()
{
	Cell* cell = m_map->GetCell( m_position );
	if ( cell != nullptr && cell->IsType( TYPE_FIRE ) )
	{
		if ( ( m_movementProperties & IS_IMMUNE_TO_FIRE ) != IS_IMMUNE_TO_FIRE )
		{
			int damage = 5;
			g_gameMessageLog->LogPrintf( Colors::RED, "The %s gets burned by fire!", m_name.c_str() );
			ApplyDamage( ( float ) damage );
		}
	}
	//m_fieldOfViewResult.m_entitiesHit.clear();
	GameEntity::GetFieldOfViewResult( m_map, m_fieldOfViewRadius, false, m_fieldOfViewResult );
	return true;
}


//---------------------------------------------------------------------------
bool Agent::PickUpItem()
{
	Cell* cell = m_map->GetCell( m_position );
	if ( cell == nullptr ) return false;
	return cell->GiveItemToAgent( this );
}


//---------------------------------------------------------------------------
void Agent::ActivateFeature( Feature* feature )
{
	feature->UseFeature();
}


//---------------------------------------------------------------------------
void Agent::MoveAgent( Map* map, const Vec2i& directionToMove )
{
	if ( !CanMoveToLocation( map, m_position + directionToMove ) ) return;

	Cell* oldCell = map->GetCell( m_position );
	Cell* newCell = map->GetCell( m_position + directionToMove );

	if ( oldCell == nullptr || newCell == nullptr ) return;

	oldCell->SetOccupyingAgent( nullptr );
	m_position += directionToMove;
	newCell->SetOccupyingAgent( this );
}


//---------------------------------------------------------------------------
void Agent::RemoveFromMap()
{
	Cell* cell = m_map->GetCell( m_position );
	if ( cell == nullptr ) return;
	cell->SetOccupyingAgent( nullptr );
	GameEntity::RemoveFromMap();
}


//---------------------------------------------------------------------------
Item* Agent::GetNextInventoryItem()
{
	if ( m_inventory.empty() ) return nullptr;

	return m_inventory.back();
}


//---------------------------------------------------------------------------
void Agent::SaveToXMLAndPopulateItemList( TiXmlNode* node, std::vector< Item* >& itemList )
{
	UNUSED( node );
	UNUSED( itemList );
}


//---------------------------------------------------------------------------
void Agent::LoadFromXML( TiXmlElement* node )
{
	m_position.x = GetAttributeAsInt( node, "x" );
	m_position.y = GetAttributeAsInt( node, "y" );
	m_currentHealth = GetAttributeAsFloat( node, "health" );
	m_faction->LoadFactionFromXML( node->FirstChildElement( "Faction" ) );

	TiXmlElement* inventory = node->FirstChildElement( "Inventory" );
	TiXmlElement* weapons = node->FirstChildElement( "Weapons" );
	TiXmlElement* armors = node->FirstChildElement( "Armors" );
	TiXmlElement* equippedWeapon = node->FirstChildElement( "EquippedWeapon" );
	TiXmlElement* equippedArmor = node->FirstChildElement( "EquippedArmor" );

	// load inventory ids
	if ( inventory )
	{
		for ( TiXmlElement* item = inventory->FirstChildElement( "Item" ); item != nullptr; item = item->NextSiblingElement( "Item" ) )
		{
			m_inventory.push_back( ( Item* ) GetAttributeAsInt( item, "id" ) );
		}
	}

	// load weapon ids
	if ( weapons )
	{
		for ( TiXmlElement* item = weapons->FirstChildElement( "Item" ); item != nullptr; item = item->NextSiblingElement( "Item" ) )
		{
			m_weapons.push_back( ( Item* ) GetAttributeAsInt( item, "id" ) );
		}
	}

	// load armor ids
	if ( armors )
	{
		for ( TiXmlElement* item = armors->FirstChildElement( "Item" ); item != nullptr; item = item->NextSiblingElement( "Item" ) )
		{
			m_armors.push_back( ( Item* ) GetAttributeAsInt( item, "id" ) );
		}
	}

	if ( equippedWeapon ) m_equippedWeapon = ( Item* ) GetAttributeAsInt( equippedWeapon, "id");
	if ( equippedArmor ) m_equippedArmor = ( Item* ) GetAttributeAsInt( equippedArmor, "id" );
}


//---------------------------------------------------------------------------
void Agent::LinkItemsFromXMLItemList( std::map< unsigned int, Item* >& itemList )
{
	if ( !m_inventory.empty() )
	{
		for ( unsigned int index = 0; index < ( unsigned int ) m_inventory.size(); ++ index )
		{
			Item* item = m_inventory.at( index );
			unsigned int id = ( unsigned int ) item;
			item = itemList.at( id );
			itemList.erase( id );
			m_inventory.at( index ) = item;
		}
	}
	

	if ( !m_weapons.empty() )
	{
		for ( unsigned int index = 0; index < ( unsigned int ) m_weapons.size(); ++ index )
		{
			Item* item = m_weapons.at( index );
			unsigned int id = ( unsigned int ) item;
			item = itemList.at( id );
			itemList.erase( id );
			m_weapons.at( id ) = item;
		}
	}

	if ( !m_armors.empty() )
	{
		for ( unsigned int index = 0; index < ( unsigned int ) m_armors.size(); ++ index )
		{
			Item* item = m_armors.at( index );
			unsigned int id = ( unsigned int ) item;
			item = itemList.at( id );
			itemList.erase( id );
			m_armors.at( index ) = item;
		}
	}

	if ( m_equippedWeapon != nullptr )
	{
		unsigned int id = ( unsigned int ) m_equippedWeapon;
		m_equippedWeapon = itemList[ id ];
		itemList.erase( id );
	}

	if ( m_equippedArmor != nullptr )
	{
		unsigned int id = ( unsigned int ) m_equippedArmor;
		m_equippedArmor = itemList[ id ];
		itemList.erase( id );
	}
}