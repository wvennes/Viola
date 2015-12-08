//---------------------------------------------------------------------------
// TheGame.cpp
//---------------------------------------------------------------------------

#include "GameCommon.hpp"
#include "Engine/External/TinyXML/tinyxml.h"
#include "Engine/Core/EngineCommon.hpp"
//#include "Engine/Math/"
#include "Engine/Math/Colors.hpp"
#include "Engine/Rendering/VennRenderer.hpp"
#include "Engine/Rendering/ShaderProgram.hpp"
#include "Engine/Rendering/TextRenderer.hpp"
#include "Engine/Input/InputManager.hpp"
#include "Engine/Core/FileSystem.hpp"
#include "Engine/Core/StringTable.hpp"
#include <time.h>
#include "Map.hpp"
#include "ItemFactory.hpp"
#include "Item.hpp"
#include "NPCFactory.hpp"
#include "BaseAIBehavior.hpp"
#include "Faction.hpp"
#include "FeatureFactory.hpp"
#include "Player.hpp"
#include "BaseMapGenerator.hpp"
#include "MessageBar.hpp"
#include "StatusBar.hpp"
#include "TheGame.hpp"

MessageBar*		g_gameMessageLog = nullptr;

//---------------------------------------------------------------------------
TheGame::TheGame()
	: m_generator( nullptr ),
	m_map( nullptr ),
	m_player( nullptr ),
	m_statusBar( nullptr ),
	m_shaderProgram( nullptr ),
	m_isAutoModeActive( true ),
	m_mapFilePath( "../Run_Win32/Data/Maps/TestMap.xml" ),
	m_gameState( STATE_STARTUP )
{
	Initialize();
}


//---------------------------------------------------------------------------
TheGame::~TheGame()
{
	delete g_gameMessageLog;
	delete m_shaderProgram;
	delete m_player;
	delete m_statusBar;
	if ( m_map != nullptr ) delete m_map;
	BaseMapGenerator::DestroyGenerators();
	NPCFactory::DestroyFactories();
	ItemFactory::DestroyFactories();
	FeatureFactory::DestroyFactories();
	BaseAIBehavior::DestroyAIBehaviorRegistrationMap();
	Faction::DestroyFactionMap();
}


//---------------------------------------------------------------------------
void TheGame::Initialize()
{
	// initialize message log
	g_gameMessageLog = new MessageBar();

	// initialize random for generators
	// TODO: Move this
	srand( ( unsigned int ) time( NULL ) );

	// 2D game, change coordinate system to tiles
	SetGameState( STATE_STARTUP );
	SetGameState( STATE_LOADING );
	SetGameState( STATE_SHOWING_MAIN_MENU );

	BaseMapGenerator::CreateGenerators();

	Faction::GenerateFactionsFromXML( "Data/Factions/Factions.xml" );
	LoadAndRegisterNPCFactoriesFromXML();
	LoadAndRegisterItemFactoriesFromXML();
	LoadAndRegisterFeatureFactoriesFromXML();

	m_player = new Player( "Bob", g_theInputManager );
	m_statusBar = new StatusBar();
	m_statusBar->SetPlayer( m_player );

	m_shaderProgram = new ShaderProgram( "Data/Shaders/DoNothing_vs.glsl", "Data/Shaders/DoNothing_fs.glsl" );
}


//---------------------------------------------------------------------------
void TheGame::LoadAndRegisterNPCFactoriesFromXML()
{
	TiXmlDocument doc;
	TiXmlElement* root = nullptr;
	bool isFileLoaded;
	FileSystem fileSystem;
	std::vector< std::string > filePaths;

	fileSystem.EnumerateFilesInDirectory( "Data/NPCFactories/*.npc.xml", filePaths );

	for ( unsigned int index = 0; index < ( unsigned int ) filePaths.size(); ++ index )
	{
		isFileLoaded = doc.LoadFile( filePaths.at( index ).c_str() );

		if ( isFileLoaded )
		{
			root = doc.RootElement();
			new NPCFactory( root->FirstChildElement( "NPC" ) );
		}
	}

	filePaths.clear();
	std::vector< std::string >().swap( filePaths );
}


//---------------------------------------------------------------------------
void TheGame::LoadAndRegisterItemFactoriesFromXML()
{
	TiXmlDocument doc;
	TiXmlElement* root = nullptr;
	bool isFileLoaded;
	FileSystem fileSystem;
	std::vector< std::string > filePaths;

	fileSystem.EnumerateFilesInDirectory( "Data/ItemFactories/*.item.xml", filePaths );

	for ( unsigned int index = 0; index < ( unsigned int ) filePaths.size(); ++ index )
	{
		isFileLoaded = doc.LoadFile( filePaths.at( index ).c_str() );

		if ( isFileLoaded )
		{
			root = doc.RootElement();

			for ( TiXmlElement* element = root->FirstChildElement( "Item" ); element != nullptr; element = element->NextSiblingElement( "Item" ) )
			{
				new ItemFactory( element );
			}
		}
	}

	filePaths.clear();
	std::vector< std::string >().swap( filePaths );
}


//---------------------------------------------------------------------------
void TheGame::LoadAndRegisterFeatureFactoriesFromXML()
{
	TiXmlDocument doc;
	TiXmlElement* root = nullptr;
	bool isFileLoaded;
	FileSystem fileSystem;
	std::vector< std::string > filePaths;

	fileSystem.EnumerateFilesInDirectory( "Data/FeatureFactories/*.feature.xml", filePaths );

	for ( unsigned int index = 0; index < ( unsigned int ) filePaths.size(); ++ index )
	{
		isFileLoaded = doc.LoadFile( filePaths.at( index ).c_str() );

		if ( isFileLoaded )
		{
			root = doc.RootElement();

			for ( TiXmlElement* element = root->FirstChildElement( "Feature" ); element != nullptr; element = element->NextSiblingElement( "Feature" ) )
			{
				new FeatureFactory( element );
			}
		}
	}

	filePaths.clear();
	std::vector< std::string>().swap( filePaths );
}


//---------------------------------------------------------------------------
void TheGame::SetGameState( GameState gameState )
{
	if ( m_gameState != gameState )
	{
		// TODO: print gamestate stuff to console
		m_gameState = gameState;
	}
	else
	{
		// TODO: print warnings/errors to console
	}
}


//---------------------------------------------------------------------------
void TheGame::ProcessInput()
{
	switch ( GetGameState() )
	{
		case STATE_SHOWING_MAIN_MENU:
		{
			ProcessInputShowingMainMenu();
			break;
		}
		case STATE_SHOWING_QUEST_MENU:
		{
			ProcessInputShowingQuestMenu();
			break;
		}
		case STATE_SHOWING_GAME:
		{
			ProcessInputShowingGame();
			break;
		}
		case STATE_GAME_OVER:
		{
			ProcessInputShowingGameOver();
			break;
		}
	}
}


//---------------------------------------------------------------------------
void TheGame::ProcessInputShowingMainMenu()
{
	if ( g_theInputManager->HasKeyBeenPressed( 'N' ) )
	{
		SetGameState( STATE_SHOWING_QUEST_MENU );
	}
	if ( g_theInputManager->HasKeyBeenPressed( Input::KEY_ESCAPE ) )
	{
		g_isQuitting = true;
	}
}


//---------------------------------------------------------------------------
void TheGame::ProcessInputShowingQuestMenu()
{
	if ( g_theInputManager->HasKeyBeenPressed( Input::KEY_ESCAPE ) )
	{
		SetGameState( STATE_SHOWING_MAIN_MENU );
	}
	if ( g_theInputManager->HasKeyBeenPressed( 'N' ) )
	{
		m_generator = BaseMapGenerator::GetGenerator( "NecromancerLair" );
		m_map = m_generator->GenerateMap();
		if ( m_isAutoModeActive ) m_generator->FinishGeneration( *m_map );
		m_player->AddToMap( m_map );
		m_generator->PlacePlayer( *m_map, *m_player );
		SetGameState( STATE_SHOWING_GAME );
	}
	if ( g_theInputManager->HasKeyBeenPressed( 'G' ) )
	{
		m_generator = BaseMapGenerator::GetGenerator( "Graveyard" );
		m_map = m_generator->GenerateMap();
		if ( m_isAutoModeActive ) m_generator->FinishGeneration( *m_map );
		m_player->AddToMap( m_map );
		m_generator->PlacePlayer( *m_map, *m_player );
		SetGameState( STATE_SHOWING_GAME );
	}
	if ( g_theInputManager->HasKeyBeenPressed( 'C' ) )
	{
		m_generator = BaseMapGenerator::GetGenerator( "CellularAutomata" );
		m_map = m_generator->GenerateMap();
		if ( m_isAutoModeActive ) m_generator->FinishGeneration( *m_map );
		m_player->AddToMap( m_map );
		m_generator->PlacePlayer( *m_map, *m_player );
		SetGameState( STATE_SHOWING_GAME );
	}
	if ( g_theInputManager->HasKeyBeenPressed( 'D' ) )
	{
		m_generator = BaseMapGenerator::GetGenerator( "Dungeon" );
		m_map = m_generator->GenerateMap();
		if ( m_isAutoModeActive ) m_generator->FinishGeneration( *m_map );
		m_player->AddToMap( m_map );
		m_generator->PlacePlayer( *m_map, *m_player );
		SetGameState( STATE_SHOWING_GAME );
	}
	if ( g_theInputManager->HasKeyBeenPressed( 'L' ) )
	{
		m_generator = BaseMapGenerator::GetGenerator( "Labyrinth" );
		m_map = m_generator->GenerateMap();
		if ( m_isAutoModeActive ) m_generator->FinishGeneration( *m_map );
		m_player->AddToMap( m_map );
		m_generator->PlacePlayer( *m_map, *m_player );
		SetGameState( STATE_SHOWING_GAME );
	}
	if ( g_theInputManager->HasKeyBeenPressed( 'M' ) )
	{
		TiXmlDocument saveFile;
		saveFile.LoadFile( "Data/Savedata/save.xml" );

		m_generator = BaseMapGenerator::GetGenerator( "Data" );
		m_map = m_generator->GenerateMap();
		//m_generator->LoadFromFile( m_mapFilePath.c_str(), *m_map );
		m_player->AddToMap( m_map );
		m_generator->LoadFromFile( saveFile.RootElement(), *m_map, *m_player );
		//m_generator->PlacePlayer( *m_map, *m_player );
		SetGameState( STATE_SHOWING_GAME );
	}
	if ( g_theInputManager->HasKeyBeenPressed( 'A' ) )
	{
		m_isAutoModeActive = !m_isAutoModeActive;
	}
}


//---------------------------------------------------------------------------
void TheGame::ProcessInputShowingGame()
{
	static bool isMapVisible = false;
	if ( g_theInputManager->HasKeyBeenPressed( Input::KEY_ESCAPE ) )
	{
		SetGameState( STATE_SHOWING_QUEST_MENU );
		delete m_map;
		m_player->ResetPlayer();
		g_gameMessageLog->ClearMessages();
		m_map = nullptr;
	}
	if ( g_theInputManager->HasKeyBeenPressed( Input::KEY_SPACE ) && !m_isAutoModeActive/* && !g_theInputManager->IsKeyDown( Input::KEY_SPACE )*/ )
	{
		// process one step
		m_generator->ProcessOneStep( *m_map );
		g_gameMessageLog->ReadMessage();
	}
	else if ( g_theInputManager->HasKeyBeenPressed( Input::KEY_SPACE ) )
	{
		g_gameMessageLog->ReadMessage();
	}
	if ( g_theInputManager->HasKeyBeenPressedOnce( 'M' ) )
	{
		isMapVisible = !isMapVisible;
		m_map->SetIsMapVisible( isMapVisible );
	}
	if ( g_theInputManager->HasKeyBeenPressedOnce( Input::KEY_F6 ) )
	{
		SaveGameToXML();
		g_gameMessageLog->LogPrintf( "Saved game to file!" );
	}
}


//---------------------------------------------------------------------------
void TheGame::ProcessInputShowingGameOver()
{
	if ( g_theInputManager->HasKeyBeenPressedOnce( Input::KEY_ESCAPE ) )
	{
		g_gameMessageLog->ClearMessages();
		SetGameState( STATE_SHOWING_QUEST_MENU );
	}
	if ( g_theInputManager->HasKeyBeenPressedOnce( 'Q' ) )
	{
		g_isQuitting = true;
	}
}


//---------------------------------------------------------------------------
bool TheGame::Update()
{
	bool isQuitting = false;

	switch ( GetGameState() )
	{
		case STATE_SHOWING_MAIN_MENU:
		{
			isQuitting = UpdateShowingMainMenu();
			break;
		}
		case STATE_SHOWING_QUEST_MENU:
		{
			isQuitting = UpdateShowingQuestMenu();
			break;
		}
		case STATE_SHOWING_GAME:
		{
			isQuitting = UpdateShowingGame();
			break;
		}
	}

	return isQuitting;
}


//---------------------------------------------------------------------------
bool TheGame::UpdateShowingMainMenu()
{
	return false;
}


//---------------------------------------------------------------------------
bool TheGame::UpdateShowingQuestMenu()
{
	return false;
}


//---------------------------------------------------------------------------
bool TheGame::UpdateShowingGame()
{
	if ( g_gameMessageLog->HasSeenAllMessages() )
	{
		m_player->Update();
		//bool didPlayerMove = m_player->Update();
// 		if ( didPlayerMove )
// 		{
// 			m_map->Update();
// 		}
	}

	if ( m_player->IsDead() )
	{
		SetGameState( STATE_GAME_OVER );
		m_player->RemoveFromMap();
		m_player->ResetPlayer();
		delete m_map;
		m_map = nullptr;
	}
	return false;
}


//---------------------------------------------------------------------------
bool TheGame::UpdateShowingGameOver()
{
	return false;
}


//---------------------------------------------------------------------------
void TheGame::Render()
{
	switch ( GetGameState() )
	{
		case STATE_SHOWING_MAIN_MENU:
		{
			RenderShowingMainMenu();
			break;
		}
		case STATE_SHOWING_QUEST_MENU:
		{
			RenderShowingQuestMenu();
			break;
		}
		case STATE_SHOWING_GAME:
		{
			RenderShowingGame();
			break;
		}
		case STATE_GAME_OVER:
		{
			RenderShowingGameOver();
			break;
		}
	}
}


//---------------------------------------------------------------------------
void TheGame::RenderShowingMainMenu()
{
	float cellHeight = 64.f;
	float halfX = GAME_WINDOW_X * 0.5f;
	g_theRenderer->ApplyOrthoProjection( 0.f, GAME_WINDOW_X, 0.f, GAME_WINDOW_Y, 0.f, 1.f );
	m_shaderProgram->UseShaderProgram();
	g_theTextRenderer->RenderText2D( "~ Viola ~", cellHeight, Vec2f( 0.f, GAME_WINDOW_Y - 110.f ), Colors::WHITE, m_shaderProgram->m_programID );
	g_theTextRenderer->RenderText2D( "(N) New Game", cellHeight, Vec2f( halfX, 600.f ), Colors::WHITE, m_shaderProgram->m_programID );
	g_theTextRenderer->RenderText2D( "(ESC) Quit", cellHeight, Vec2f( halfX, 200.f ), Colors::WHITE, m_shaderProgram->m_programID );
}


//---------------------------------------------------------------------------
void TheGame::RenderShowingQuestMenu()
{
	float cellHeight = 64.f;
	float halfX = GAME_WINDOW_X * 0.5f;
	g_theRenderer->ApplyOrthoProjection( 0.f, GAME_WINDOW_X, 0.f, GAME_WINDOW_Y, 0.f, 1.f );
	m_shaderProgram->UseShaderProgram();
	g_theTextRenderer->RenderText2D( "~ Viola ~", cellHeight, Vec2f( 0.f, GAME_WINDOW_Y - 110.f ), Colors::WHITE, m_shaderProgram->m_programID );
	g_theTextRenderer->RenderText2D( "(N) Necromancer Lair", cellHeight, Vec2f( halfX, 700.f ), Colors::WHITE, m_shaderProgram->m_programID );
	g_theTextRenderer->RenderText2D( "(G) Graveyard", cellHeight, Vec2f( halfX, 600.f), Colors::WHITE, m_shaderProgram->m_programID );
	g_theTextRenderer->RenderText2D( "(C) Caves", cellHeight, Vec2f( halfX, 500.f ), Colors::WHITE, m_shaderProgram->m_programID );
	g_theTextRenderer->RenderText2D( "(D) Dungeon", cellHeight, Vec2f( halfX, 400.f ), Colors::WHITE, m_shaderProgram->m_programID );
	g_theTextRenderer->RenderText2D( "(L) Labyrinth", cellHeight, Vec2f( halfX, 300.f ), Colors::WHITE, m_shaderProgram->m_programID );
	g_theTextRenderer->RenderText2D( "(M) Load From File", cellHeight, Vec2f( halfX, 200.f ), Colors::WHITE, m_shaderProgram->m_programID );
	g_theTextRenderer->RenderText2D( "(ESC) Back to Main Menu", cellHeight, Vec2f( halfX, 100.f ), Colors::WHITE, m_shaderProgram->m_programID );
	g_theTextRenderer->RenderText2D( "(A) Toggle Auto Generation Mode : ", cellHeight, Vec2f( 0.f, 10.f ), Colors::WHITE, m_shaderProgram->m_programID );
	float bottomTextOffset = g_theTextRenderer->CalculateTextWidth( "(A) Toggle Auto Generation Mode : ", cellHeight );
	if ( m_isAutoModeActive )
	{
		g_theTextRenderer->RenderText2D( "ON", cellHeight, Vec2f( bottomTextOffset, 10.f ), Colors::RED, m_shaderProgram->m_programID );
	}
	else
	{
		g_theTextRenderer->RenderText2D( "OFF", cellHeight, Vec2f( bottomTextOffset, 10.f ), Colors::RED, m_shaderProgram->m_programID );
	}
}


//---------------------------------------------------------------------------
void TheGame::RenderShowingGame()
{
	//char buffer[10];
	//sprintf_s( buffer, sizeof( buffer ), "%i", m_player->GetCurrentHealth() );
	//float cellHeight = 64.f;
	m_shaderProgram->UseShaderProgram();
	m_map->Render();
	RenderPlayerInformation();
	RenderGameMessageLog();
//	g_theRenderer->ApplyOrthoProjection( 0.f, GAME_WINDOW_X, 0.f, GAME_WINDOW_Y, 0.f, 1.f );
//	g_theTextRenderer->RenderText2D( "Health: " + std::string( buffer ), cellHeight, Vec2f( GAME_WINDOW_X - 300.f, GAME_WINDOW_Y - ( cellHeight + 10.f ) ), Colors::GREEN, m_shaderProgram->m_programID );
//	g_theTextRenderer->RenderText2D( "(M) Toggle Map Visibility", cellHeight, Vec2f( 10.f, GAME_WINDOW_Y - ( cellHeight + 10.f ) ), Colors::WHITE, m_shaderProgram->m_programID );
//	g_theTextRenderer->RenderText2D( "(ESC) Quest Menu", cellHeight, Vec2f( 10.f, 10.f ), Colors::WHITE, m_shaderProgram->m_programID );
//	if ( !m_isAutoModeActive ) g_theTextRenderer->RenderText2D( "(SPACE) Gegenerate Next Step", cellHeight, Vec2f( g_theTextRenderer->CalculateTextWidth( "(ESC) Quest Menu", 75.f ) + 100.f, 10.f ), Colors::WHITE, m_shaderProgram->m_programID );
}


//---------------------------------------------------------------------------
void TheGame::RenderPlayerInformation()
{
	float cellHeight = 28.f;
	g_theRenderer->ApplyOrthoProjection( 0.f, GAME_WINDOW_X, 0.f, GAME_WINDOW_Y, 0.f, 1.f );
	g_theRenderer->ApplyTranslation( Vec3f( 300.f, 0.f, 0.f ) );
	m_statusBar->Render( cellHeight );
}


//---------------------------------------------------------------------------
void TheGame::RenderGameMessageLog()
{
// 	for ( unsigned int index = 0; index < g_gameMessageLog->m_historySize; ++ index )
// 	{
// 
// 	}
	float cellHeight = 64.f;
	g_theRenderer->ApplyOrthoProjection( 0.f, GAME_WINDOW_X, 0.f, GAME_WINDOW_Y, 0.f, 1.f );
	g_theRenderer->ApplyTranslation( Vec3f( 10.f, GAME_WINDOW_Y - cellHeight - 10.f, 0.f ) );

	g_gameMessageLog->Render( cellHeight );
}


//---------------------------------------------------------------------------
void TheGame::RenderShowingGameOver()
{
	float cellHeight = 64.f;
	float halfX = GAME_WINDOW_X * 0.5f;
	g_theRenderer->ApplyOrthoProjection( 0.f, GAME_WINDOW_X, 0.f, GAME_WINDOW_Y, 0.f, 1.f );
	g_theTextRenderer->RenderText2D( "GAME OVER", cellHeight, Vec2f( halfX, 700.f ), Colors::RED, m_shaderProgram->m_programID );
	g_theTextRenderer->RenderText2D( "(ESC) Back to Quest Menu", cellHeight, Vec2f( halfX, 600.f ), Colors::WHITE, m_shaderProgram->m_programID );
	g_theTextRenderer->RenderText2D( "(Q) Quit", cellHeight, Vec2f( halfX, 500.f ), Colors::WHITE, m_shaderProgram->m_programID );
}


//---------------------------------------------------------------------------
void TheGame::SaveGameToXML()
{
	TiXmlDocument doc;
	TiXmlDeclaration* declaration = new TiXmlDeclaration( "1.0", "utf-8", "" );
	doc.LinkEndChild( declaration );

	// <MapData> </MapData>
	TiXmlElement* root = new TiXmlElement( "MapData" );
	doc.LinkEndChild( root );

	// write map to file
	std::vector< Item* > itemList;
	SaveMapToXMLAndPopulateItems( root, itemList );

	doc.SaveFile( "Data/Savedata/save.xml" );
}


//---------------------------------------------------------------------------
void TheGame::SaveMapToXMLAndPopulateItems( TiXmlNode* node, std::vector< Item* >& itemList )
{
	if ( m_map != nullptr ) m_map->SaveMapToXML( node, itemList );
	if ( m_player != nullptr )
	{
		TiXmlElement* player = new TiXmlElement( "PlayerData" );
		m_player->SaveToXMLAndPopulateItemList( player, itemList );
		node->LinkEndChild( player );
	}

	if ( itemList.empty() ) return;

	TiXmlElement* items = new TiXmlElement( "Items" );
	std::vector< Item* >::iterator iter;
	for ( iter = itemList.begin(); iter != itemList.end(); ++ iter )
	{
		( *iter )->SaveToXML( items );
	}

	node->LinkEndChild( items );
}