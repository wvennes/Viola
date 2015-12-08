//-----------------------------------------------------------------------------------------------
// TheGame.hpp
//-----------------------------------------------------------------------------------------------
#pragma once
#ifndef _INCLUDED_THEGAME_
#define _INCLUDED_THEGAME_

#include <string>

class VennRenderer;
class Map;
class Player;
class Item;
class ShaderProgram;
class TextRenderer;
class InputManager;
class BaseMapGenerator;
class StatusBar;
class TiXmlNode;


/////////////////////////////////////////////////////////////////////////////
enum GameState
{
	STATE_STARTUP,
	STATE_LOADING,
	STATE_SHOWING_MAIN_MENU,
	STATE_SHOWING_QUEST_MENU,
	STATE_SHOWING_GAME,
	STATE_GAME_OVER,
	STATE_QUITTING,
	STATE_SHUTDOWN,
	NUM_STATES
};


/////////////////////////////////////////////////////////////////////////////
class TheGame
{
public:
	TheGame();
	~TheGame();

	void				ProcessInput();
	void				Render();
	bool				Update();

	GameState			GetGameState() const;
	void				SetGameState( GameState gameState );

private:
	Map*				m_map;
	Player*				m_player;
	StatusBar*			m_statusBar;
	BaseMapGenerator*	m_generator;
	GameState			m_gameState;
	ShaderProgram*		m_shaderProgram;
	bool				m_isAutoModeActive;
	std::string			m_mapFilePath;

private:
	void				Initialize();

	void				LoadAndRegisterNPCFactoriesFromXML();
	void				LoadAndRegisterItemFactoriesFromXML();
	void				LoadAndRegisterFeatureFactoriesFromXML();

	void				ProcessInputShowingMainMenu();
	void				ProcessInputShowingQuestMenu();
	void				ProcessInputShowingGame();
	void				ProcessInputShowingGameOver();

	bool				UpdateShowingMainMenu();
	bool				UpdateShowingQuestMenu();
	bool				UpdateShowingGame();
	bool				UpdateShowingGameOver();

	void				RenderShowingMainMenu();
	void				RenderShowingQuestMenu();
	void				RenderShowingGame();
	void				RenderPlayerInformation();
	void				RenderGameMessageLog();
	void				RenderShowingGameOver();

	void				SaveGameToXML();
	void				SaveMapToXMLAndPopulateItems( TiXmlNode* node, std::vector< Item* >& itemList );
	void				SaveEntitiesToXML();
	void				SavePlayerToXML();
	void				SaveAllNPCsToXML();
	void				SaveNPCToXML();
	void				SaveItemsToXML();
	void				SaveFeaturesToXML();

	void				LoadGameFromXML();
};


//---------------------------------------------------------------------------
inline GameState TheGame::GetGameState() const
{
	return m_gameState;
}

extern TextRenderer* g_theTextRenderer;
extern VennRenderer* g_theRenderer;
extern InputManager* g_theInputManager;
extern bool g_keys[];

#endif