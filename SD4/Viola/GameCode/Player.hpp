//---------------------------------------------------------------------------
// Player.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_PLAYER_
#define _INCLUDED_PLAYER_

#include "Agent.hpp"

class InputManager;

/////////////////////////////////////////////////////////////////////////////
class Player : public Agent
{
public:
	Player();
	Player( const Player& otherPlayer );
	Player( const std::string& nickname, InputManager* inputManager );
	~Player();

	void				AddToMap( Map* map );
	void				ResetPlayer();
	bool				ProcessInput();
	bool				Update();
	void				Render();
	void				MoveAgent( Map* map, const Vec2i& coordinates );
	bool				UsePotion();
	void				GiveItem( Item* item );
	void				SaveToXMLAndPopulateItemList( TiXmlNode* node, std::vector< Item* >& itemList );
	void				LoadFromXML( TiXmlElement* node );

	std::string			GetNickname() const { return m_nickname; }

	void				SetPosition( const Vec2i& position );
	void				SetNickname( const std::string& nickname ) { m_nickname = nickname; }

private:
	unsigned int		m_currentTurn;
	InputManager*		m_inputManager;
	std::string			m_nickname;

private:
	void				Initialize();
	void				SetFieldOfViewLighting();

};

#endif