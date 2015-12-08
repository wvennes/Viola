//---------------------------------------------------------------------------
// Player.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Input/InputManager.hpp"
#include "Engine/Core/StringTable.hpp"
#include "Engine/Core/TinyXMLHelper.hpp"
#include "MessageBar.hpp"
#include "Cell.hpp"
#include "Faction.hpp"
#include "Feature.hpp"
#include "Map.hpp"
#include "Item.hpp"
#include "CombatSystem.hpp"
#include "Player.hpp"


//---------------------------------------------------------------------------
Player::Player()
	: Agent()
	, m_currentTurn( 0 )
	, m_inputManager( nullptr )
	, m_nickname( "nickname" )
{
	Initialize();
}


//---------------------------------------------------------------------------
Player::Player( const Player& otherPlayer )
	: Agent( otherPlayer )
	, m_currentTurn( 0 )
	, m_inputManager( nullptr )
	, m_nickname( otherPlayer.GetNickname() )
{
	
}


//---------------------------------------------------------------------------
Player::Player( const std::string& nickname, InputManager* inputManager )
	: Agent()
	, m_currentTurn( 0 )
	, m_inputManager( inputManager )
	, m_nickname( nickname )
{
	Initialize();
}


//---------------------------------------------------------------------------
Player::~Player()
{
	// do nothing for now
}


//---------------------------------------------------------------------------
void Player::Initialize()
{
	SetMovementSpeed( 1 );
	SetName( "player" );
	SetGlyph( '@' );
	SetColor( Colors::WHITE );
	SetFaction( Faction::SpawnFaction( StringID( "humans" ) ) );
	SetMaxHealth( 100.f );
	SetCurrentHealth( m_maxHealth );
	SetFieldOfViewRadius( 10.f );
	SetBaseAttackDamage( 10.f );
}


//---------------------------------------------------------------------------
void Player::SetFieldOfViewLighting()
{
// 	m_map->ClearCellVisibilityFlags();
// 
// 	//FieldOfViewResult result = GetFieldOfViewResult( *m_map, range );
// 
// 	float oldRange = m_fieldOfViewRadius - 0.f;
// 	float newRange = 1.f - 0.3f;
// 	float lightValue = 0.3f;
// 
// 	Vec2i distance;
// 	float distanceValue;
// 
// 	EntitySet::iterator iter;
// 	for ( iter = m_fieldOfViewResult.m_entitiesHit.begin(); iter != m_fieldOfViewResult.m_entitiesHit.end(); ++ iter )
// 	{
// 		Cell* currentCell = m_map->GetCell( *iter );
// 		distance = *iter - m_position;
// 		distanceValue = Vec2f( ( float ) distance.x, ( float ) distance.y ).CalculateDistance();
// 
// 		lightValue = ( ( distanceValue * newRange ) / oldRange ) + 0.3f;
// 		if ( currentCell != nullptr ) currentCell->SetCellVisibility( 1.f - lightValue );
// 		//( *iter )->SetIsCellVisible( true );
// 	}
}


//---------------------------------------------------------------------------
void Player::AddToMap( Map* map )
{
	m_map = map;
	//SetFieldOfViewLighting();
}


//---------------------------------------------------------------------------
void Player::ResetPlayer()
{
	if ( !m_inventory.empty() )
	{
// 		for ( unsigned int index = 0; index < ( unsigned int ) m_inventory.size(); ++ index )
// 		{
// 			// delete any items in this agent's inventory
// 			if ( m_inventory.at( index ) ) delete m_inventory.at( index );
// 		}
		DestroyVector( m_inventory );
	}

	if ( !m_weapons.empty() )
	{
// 		for ( unsigned int index = 0; index < ( unsigned int ) m_weapons.size(); ++ index )
// 		{
// 			// delete any weapons in this agent's inventory
// 			if ( m_weapons[ index ] ) delete m_weapons[ index ];
// 		}
		DestroyVector( m_weapons );
	}

	if ( !m_armors.empty() )
	{
// 		for ( unsigned int index = 0; index < ( unsigned int ) m_armors.size(); ++ index )
// 		{
// 			// delete any armors in this agent's inventory
// 			if ( m_armors[ index ] ) delete m_armors[ index ];
// 		}
		DestroyVector( m_armors );
	}

	if ( m_equippedArmor != nullptr ) delete m_equippedArmor;
	if ( m_equippedWeapon != nullptr ) delete m_equippedWeapon;
	m_equippedArmor = nullptr;
	m_equippedWeapon = nullptr;
// 	if ( m_faction != nullptr ) delete m_faction;
// 	m_faction = nullptr;

	m_fieldOfViewResult.m_entitiesHit.clear();
	SetMovementSpeed( 1 );
	SetName( "player" );
	SetGlyph( '@' );
	SetColor( Colors::WHITE );
//	SetFaction( Faction::SpawnFaction( StringID( "humans" ) ) );
	SetMaxHealth( 100.f );
	SetCurrentHealth( m_maxHealth );
	SetBaseAttackDamage( 10.f );
}


//---------------------------------------------------------------------------
bool Player::ProcessInput()
{
	bool didPlayerMove = false;
	bool didPlayerPickUpItem = false;
	bool didPlayerUseItem = false;
	Vec2i movementDirection;
	//if ( inputManager->has
	//if ( m_inputManager->HasKeyBeenPressed( ))

	// LEFT
	if ( m_inputManager->HasKeyBeenPressedOnce( 'H' ) )
	{
		movementDirection = Vec2i( -1, 0 );
		didPlayerMove = true;
	}

	// DOWN
	if ( m_inputManager->HasKeyBeenPressedOnce( 'J' ) )
	{
		movementDirection = Vec2i( 0, -1 );
		didPlayerMove = true;
	}

	// UP
	if ( m_inputManager->HasKeyBeenPressedOnce( 'K' ) )
	{
		movementDirection = Vec2i( 0, 1 );
		didPlayerMove = true;
	}

	// RIGHT
	if ( m_inputManager->HasKeyBeenPressedOnce( 'L' ) )
	{
		movementDirection = Vec2i( 1, 0 );
		didPlayerMove = true;
	}

	// NORTHWEST
	if ( m_inputManager->HasKeyBeenPressedOnce( 'Y' ) )
	{
		movementDirection = Vec2i( -1, 1 );
		didPlayerMove = true;
	}

	// NORTHEAST
	if ( m_inputManager->HasKeyBeenPressedOnce( 'U' ) )
	{
		movementDirection = Vec2i( 1, 1 );
		didPlayerMove = true;
	}

	// SOUTHWEST
	if ( m_inputManager->HasKeyBeenPressedOnce( 'B' ) )
	{
		movementDirection = Vec2i( -1, -1 );
		didPlayerMove = true;
	}

	// SOUTHEAST
	if ( m_inputManager->HasKeyBeenPressedOnce( 'N' ) )
	{
		movementDirection = Vec2i( 1, -1 );
		didPlayerMove = true;
	}

	// PICK UP ITEM
	if ( m_inputManager->HasKeyBeenPressed( Input::KEY_COMMA ) )
	{
		if ( PickUpItem() )
		{
			didPlayerPickUpItem = true;
			didPlayerMove = true;
		}
	}

	// USE POTION
	if ( m_inputManager->HasKeyBeenPressed( 'P' ) )
	{
		if ( UsePotion() )
		{
			didPlayerUseItem = true;
		}
	}

	if ( didPlayerMove )
	{
		if ( !didPlayerPickUpItem && !didPlayerUseItem )
		{
			g_gameMessageLog->ReadMessage();
			MoveAgent( m_map, movementDirection );
		}
		//SetFieldOfViewLighting();

// 		if ( didPlayerPickUpItem )
// 		{
// 			g_gameMessageLog->ReadMessage();
// 			g_gameMessageLog->LogPrintf( Colors::YELLOW, "%s picked up a %s!", m_nickname.c_str(), m_inventory.back()->GetName().c_str() );
// 		}
	}

	return didPlayerMove;
}


//---------------------------------------------------------------------------
bool Player::Update()
{
	//m_inputManager->Update();
	//Agent::Update();
	bool didPlayerMove = ProcessInput();
	
	if ( didPlayerMove )
	{
		//m_fieldOfViewResult.m_entitiesHit.clear();

		m_map->Update();
		//Agent::Update();
		Cell* cell = m_map->GetCell( m_position );
		if ( cell != nullptr && cell->IsType( TYPE_FIRE ) )
		{
			int damage = 5;
			g_gameMessageLog->LogPrintf( Colors::RED, "%s gets burned by fire!", m_nickname.c_str() );
			ApplyDamage( ( float ) damage );
		}

		GameEntity::GetFieldOfViewResult( m_map, m_fieldOfViewRadius, true, m_fieldOfViewResult );
	}
	return didPlayerMove;
}


//---------------------------------------------------------------------------
void Player::Render()
{

}


//---------------------------------------------------------------------------
void Player::MoveAgent( Map* map, const Vec2i& directionToMove )
{
	//if ( !CanMoveToLocation( map, m_position + directionToMove ) ) return;

	Cell* cell = map->GetCell( m_position + directionToMove );

	if ( cell != nullptr && cell->IsCellOccupiedByAgent() )
	{
		CombatInformation info;
		info.m_attackingEntity = this;
		info.m_defendingEntity = cell->GetOccupyingAgent();
		info.m_attackingWeapon = m_equippedWeapon;
		info.m_attackType = MELEE;
		CombatSystem::ApplyAttackAndGetResult( info );

		map->SendFactionAttackEvent( this, cell->GetOccupyingAgent() );

		g_gameMessageLog->LogPrintf( Colors::GREEN, "%s attacks the %s for %i damage!", m_nickname.c_str(), info.m_defendingEntity->GetName().c_str(), ( int ) info.m_damageDealt_out );

		if ( m_equippedWeapon != nullptr && m_equippedWeapon->IsDead() )
		{
			g_gameMessageLog->LogPrintf( Colors::RED, "The %s broke!", m_equippedWeapon->GetName().c_str() );
			delete m_equippedWeapon;
			m_equippedWeapon = nullptr;

			if ( !m_weapons.empty() )
			{
				m_equippedWeapon = m_weapons.back();
				m_weapons.pop_back();
				g_gameMessageLog->LogPrintf( Colors::GREEN, "%s equips the %s!", GetNickname().c_str(), m_equippedWeapon->GetName().c_str() );
			}
		}
	}
	else if ( cell != nullptr && cell->IsCellOccupiedByFeature() && !cell->GetOccupyingFeature()->CanWalkOn() )
	{
		Feature* feature = cell->GetOccupyingFeature();
		ActivateFeature( feature );
		g_gameMessageLog->LogPrintf( Colors::YELLOW, "%s used the %s...", m_nickname.c_str(), feature->GetName().c_str() );
	}
	else
	{
		Agent::MoveAgent( map, directionToMove );
		Cell* currentCell = map->GetCell( m_position );
		if ( currentCell != nullptr && currentCell->DoesCellHaveItems() )
		{
			g_gameMessageLog->LogPrintf( Colors::YELLOW, "There are items at %s's feet...", m_nickname.c_str() );
		}
	}
}


//---------------------------------------------------------------------------
bool Player::UsePotion()
{
	if ( m_inventory.empty() ) return false;

	Item* item = m_inventory.back();
	g_gameMessageLog->LogPrintf( Colors::GREEN, "%s used a %s!", m_nickname.c_str(), item->GetName().c_str() );
	return Agent::UsePotion();
}


//---------------------------------------------------------------------------
void Player::GiveItem( Item* item )
{
	g_gameMessageLog->LogPrintf( Colors::GREEN, "%s picks up a %s!", GetNickname().c_str(), item->GetName().c_str() );

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
				g_gameMessageLog->LogPrintf( Colors::YELLOW, "%s equips the %s!", GetNickname().c_str(), item->GetName().c_str() );
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
				g_gameMessageLog->LogPrintf( Colors::YELLOW, "%s equips the %s!", GetNickname().c_str(), item->GetName().c_str() );
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
void Player::SaveToXMLAndPopulateItemList( TiXmlNode* node, std::vector< Item* >& itemList )
{
	TiXmlElement* player = new TiXmlElement( "Player" );

	player->SetAttribute( "id", m_id );
	player->SetAttribute( "name", m_nickname.c_str() );
	player->SetAttribute( "x", m_position.x );
	player->SetAttribute( "y", m_position.y );
	player->SetDoubleAttribute( "health", m_currentHealth );

	// save faction relationships to XML
	m_faction->SaveFactionToXML( player );

	// inventory
	if ( !m_inventory.empty() )
	{
		TiXmlElement* inventory = new TiXmlElement( "Inventory" );

		Inventory::iterator inventoryIter;
		for ( inventoryIter = m_inventory.begin(); inventoryIter != m_inventory.end(); ++ inventoryIter )
		{
			TiXmlElement* item = new TiXmlElement( "Item" );
			item->SetAttribute( "id", ( *inventoryIter )->GetID() );
			itemList.push_back( *inventoryIter );
			inventory->LinkEndChild( item );
		}

		player->LinkEndChild( inventory );
	}

	// weapon list
	if ( !m_weapons.empty() )
	{
		TiXmlElement* weapons = new TiXmlElement( "Weapons" );

		Weapons::iterator weaponIter;
		for ( weaponIter = m_weapons.begin(); weaponIter != m_weapons.end(); ++ weaponIter )
		{
			TiXmlElement* item = new TiXmlElement( "Item" );
			item->SetAttribute( "id", ( *weaponIter )->GetID() );
			itemList.push_back( *weaponIter );
			weapons->LinkEndChild( item );
		}

		player->LinkEndChild( weapons );
	}

	// armor list
	if ( !m_armors.empty() )
	{
		TiXmlElement* armors = new TiXmlElement( "Armors" );

		Armors::iterator armorIter;
		for ( armorIter = m_armors.begin(); armorIter != m_armors.end(); ++ armorIter )
		{
			TiXmlElement* item = new TiXmlElement( "Item" );
			item->SetAttribute( "id", ( *armorIter )->GetID() );
			itemList.push_back( *armorIter );
			armors->LinkEndChild( item );
		}

		player->LinkEndChild( armors );
	}

	// equipped weapon
	if ( m_equippedWeapon != nullptr )
	{
		TiXmlElement* equippedWeapon = new TiXmlElement( "EquippedWeapon" );
		equippedWeapon->SetAttribute( "id", m_equippedWeapon->GetID() );
		itemList.push_back( m_equippedWeapon );
		player->LinkEndChild( equippedWeapon );
	}

	// equipped armor
	if ( m_equippedArmor != nullptr )
	{
		TiXmlElement* equippedArmor = new TiXmlElement( "EquippedArmor" );
		equippedArmor->SetAttribute( "id", m_equippedArmor->GetID() );
		itemList.push_back( m_equippedArmor );
		player->LinkEndChild( equippedArmor );
	}

	node->LinkEndChild( player );
}


//---------------------------------------------------------------------------
void Player::LoadFromXML( TiXmlElement* node )
{
	m_nickname = GetAttributeAsString( node, "name" );
	Agent::LoadFromXML( node );
}


//---------------------------------------------------------------------------
void Player::SetPosition( const Vec2i& position )
{
	Agent::SetPosition( position );

	GameEntity::GetFieldOfViewResult( m_map, m_fieldOfViewRadius, true, m_fieldOfViewResult );
}