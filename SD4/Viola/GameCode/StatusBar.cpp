//---------------------------------------------------------------------------
// StatusBar.cpp
//---------------------------------------------------------------------------

#include "Engine/Rendering/TextRenderer.hpp"
#include "Player.hpp"
#include "Item.hpp"
#include "StatusBar.hpp"


//---------------------------------------------------------------------------
StatusBar::StatusBar()
	: m_player( nullptr )
{

}


//---------------------------------------------------------------------------
StatusBar::~StatusBar()
{

}


//---------------------------------------------------------------------------
void StatusBar::SetPlayer( Player* player )
{
	m_player = player;
}


//---------------------------------------------------------------------------
void StatusBar::Render( float cellHeight )
{
	//Vec2f offset( 10.f, 1.f );
	float overallAttack = m_player->GetBaseAttackDamage();
	char healthBuffer[ 1024 ];
	char attackBuffer[ 100 ];
	char currentWeaponBuffer[ 1024 ];
	char currentWeaponUsesBuffer[ 100 ];
	char currentArmorBuffer[ 1024 ];
	char currentArmorUsesBuffer[ 100 ];
	char nextItemBuffer[ 1024 ];

	sprintf_s( healthBuffer, sizeof( healthBuffer ), "HP: %i / %i", ( int ) m_player->GetCurrentHealth(), ( int ) m_player->GetMaxHealth() );

	// fill in weapon information
	if ( m_player->GetEquippedWeapon() != nullptr )
	{
		sprintf_s( currentWeaponBuffer, sizeof( currentWeaponBuffer ), "Current Weapon: %s", m_player->GetEquippedWeapon()->GetName().c_str() );
		sprintf_s( currentWeaponUsesBuffer, sizeof( currentWeaponUsesBuffer ), "Uses: %i", m_player->GetEquippedWeapon()->GetItemUses() );
		overallAttack += m_player->GetEquippedWeapon()->GetItemAttack();
	}
	else
	{
		sprintf_s( currentWeaponBuffer, sizeof( currentWeaponBuffer ), "Current Weapon: N/A" );
		sprintf_s( currentWeaponUsesBuffer, sizeof( currentWeaponUsesBuffer ), "Uses: " );
	}

	// fill in armor information
	if ( m_player->GetEquippedArmor() != nullptr )
	{
		sprintf_s( currentArmorBuffer, sizeof( currentArmorBuffer ), "Current Armor: %s", m_player->GetEquippedArmor()->GetName().c_str() );
		sprintf_s( currentArmorUsesBuffer, sizeof( currentArmorUsesBuffer ), "Uses: %i", m_player->GetEquippedArmor()->GetItemUses() );
	}
	else
	{
		sprintf_s( currentArmorBuffer, sizeof( currentArmorBuffer ), "Current Armor: N/A" );
		sprintf_s( currentArmorUsesBuffer, sizeof( currentArmorUsesBuffer ), "Uses: " );
	}

	// fill in item information
	if ( m_player->GetNextInventoryItem() != nullptr )
	{
		sprintf_s( nextItemBuffer, sizeof( nextItemBuffer ), "Next Item: %s", m_player->GetNextInventoryItem()->GetName().c_str() );
	}
	else
	{
		sprintf_s( nextItemBuffer, sizeof( nextItemBuffer ), "Next Item: N/A" );
	}

	// print overall attack
	sprintf_s( attackBuffer, sizeof( attackBuffer ), "ATK: %i", ( int ) overallAttack );

	// TOP ROW
	g_theTextRenderer->RenderText2D( 
		( m_player->GetNickname() + "'s Stats" ).c_str(), 
		cellHeight, 
		Vec2f( 0.f, 2 * cellHeight ), 
		Colors::WHITE, g_currentShaderProgramID );

	g_theTextRenderer->RenderText2D(
		nextItemBuffer,
		cellHeight,
		Vec2f( 200.f, 2 * cellHeight ),
		Colors::WHITE, g_currentShaderProgramID );

	// SECOND ROW
	g_theTextRenderer->RenderText2D( 
		healthBuffer, 
		cellHeight, 
		Vec2f( 0.f, 1 * cellHeight ), 
		Colors::WHITE, g_currentShaderProgramID );

	g_theTextRenderer->RenderText2D(
		currentArmorBuffer,
		cellHeight,
		Vec2f( 200.f, 1 * cellHeight ),
		Colors::WHITE, g_currentShaderProgramID );

	g_theTextRenderer->RenderText2D(
		currentArmorUsesBuffer,
		cellHeight,
		Vec2f( 800.f, 1 * cellHeight ),
		Colors::WHITE, g_currentShaderProgramID );

	// THIRD ROW
	g_theTextRenderer->RenderText2D( 
		attackBuffer, 
		cellHeight, 
		Vec2f( 0.f, 0 * cellHeight ),
		Colors::WHITE, g_currentShaderProgramID );

	g_theTextRenderer->RenderText2D(
		currentWeaponBuffer,
		cellHeight,
		Vec2f( 200.f, 0 * cellHeight ),
		Colors::WHITE, g_currentShaderProgramID );

	g_theTextRenderer->RenderText2D(
		currentWeaponUsesBuffer,
		cellHeight,
		Vec2f( 800.f, 0 * cellHeight ),
		Colors::WHITE, g_currentShaderProgramID );
}