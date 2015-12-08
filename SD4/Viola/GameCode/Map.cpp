//---------------------------------------------------------------------------
// Map.cpp
//---------------------------------------------------------------------------

#include <set>
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Rendering/VennRenderer.hpp"
#include "Engine/Rendering/TextRenderer.hpp"
#include "Engine/Rendering/Vertex3D.hpp"
#include "Engine/Rendering/RenderFunctions.hpp"
#include "Engine/Math/Colors.hpp"
#include "Engine/Core/TinyXMLHelper.hpp"
#include "MessageBar.hpp"
#include "Agent.hpp"
#include "Feature.hpp"
#include "NPC.hpp"
#include "Item.hpp"
#include "Map.hpp"


//---------------------------------------------------------------------------
Map::Map()
	: m_isMapVisible( false )
{
	Initialize();
}


//---------------------------------------------------------------------------
Map::Map( const Map& otherMap )
	: m_isMapVisible( false )
{
	m_cells.reserve( MAP_WIDTH_CELLS * MAP_HEIGHT_CELLS );
	
	for ( unsigned int index = 0; index < otherMap.m_cells.size(); ++ index )
	{
		m_cells.push_back( Cell( otherMap.m_cells.at( index ) ) );
	}

	//m_cells.shrink_to_fit();
	std::vector< Cell >( m_cells ).swap( m_cells );
}


//---------------------------------------------------------------------------
Map::~Map()
{
// 	for ( unsigned int index = 0; index < m_cells.size(); ++ index )
// 	{
// 		Cell& currentCell = m_cells[ index ];
// 
// 		if ( currentCell.IsCellOccupied() && currentCell.GetOccupyingAgent()->GetName() != "player" ) delete currentCell.GetOccupyingAgent();
// 	}
// 	NPCMap::iterator iter;
// 	for ( iter = m_npcRegistry.begin(); iter != m_npcRegistry.end(); ++ iter )
// 	{
// 		if ( iter->second != nullptr ) delete iter->second;
// 	}
// 	for ( unsigned int index = 0; index < ( unsigned int ) m_npcRegistry.size(); ++ index )
// 	{
// 		delete m_npcRegistry[ index ];
// 	}
	DestroyMap( m_npcRegistry );
	DestroyMap( m_featureRegistry );
	m_cells.clear();
	std::vector< Cell >().swap( m_cells );
}


//---------------------------------------------------------------------------
void Map::Initialize()
{
	unsigned int totalNumCells = MAP_WIDTH_CELLS * MAP_HEIGHT_CELLS;
	m_cells.reserve( totalNumCells );
	Cell nextCell;

	for ( CellIndex index = 0; index < totalNumCells; ++ index )
	{
		m_cells.push_back( nextCell );
	}

	//m_cells.shrink_to_fit();
	std::vector< Cell >( m_cells ).swap( m_cells );
}


//---------------------------------------------------------------------------
CellCoords Map::GetRandomValidLocationInMap( int movementProperties )
{
	CellCoords result;
	bool hasFoundPosition = false;
	while ( !hasFoundPosition )
	{
		result = CellCoords( rand() % MAP_WIDTH_CELLS, rand() % MAP_HEIGHT_CELLS );
		Cell* cell = GetCell( result );
		if ( cell != nullptr && !cell->IsCellOccupiedByAgent() && !cell->IsCellOccupiedByFeature() && !cell->IsBlockedByCell( movementProperties ) )
		{
			hasFoundPosition = true;
		}
	}

	return result;
}


//---------------------------------------------------------------------------
void Map::GetSurroundingNonBlockedCoordinates( const CellCoords& coordinates, int movementProperties, std::vector< Vec2i >& outCoordinates )
{
	if ( !IsLocationInMap( coordinates ) ) return;

	CellCoords currentCoords;

	for ( int x = -1; x <= 1; ++ x )
	{
		for ( int y = -1; y <= 1; ++ y )
		{
			if ( x != 0 || y != 0 )
			{
				currentCoords = coordinates + CellCoords( x, y );
				Cell* cell = GetCell( currentCoords );
				if ( cell != nullptr && !cell->IsBlockedByCell( movementProperties ) /*&& !GetCell( currentCoords ).IsCellOccupied()*/ )
				{
					outCoordinates.push_back( currentCoords );
				}
			}
		}
	}
}


//---------------------------------------------------------------------------
void Map::AddNPCToRegistry( NPC* npc )
{
	//m_npcRegistry.push_back( npc );
	if ( m_npcRegistry.find( npc->GetID() ) == m_npcRegistry.end() )
	{
		m_npcRegistry.insert( std::make_pair( npc->GetID(), npc ) );
	}
}


//---------------------------------------------------------------------------
void Map::RemoveNPCFromRegistry( unsigned int id )
{
	if ( m_npcRegistry.find( id ) != m_npcRegistry.end() )
	{
		//NPC* npcToRemove = m_npcRegistry[ id ];
		m_npcRegistry[ id ] = nullptr;
		//m_npcRegistry.erase( id );
		//delete npcToRemove;
	}
}


//---------------------------------------------------------------------------
void Map::AddFeatureToRegistry( Feature* feature )
{
	if ( m_featureRegistry.find( feature->GetID() ) == m_featureRegistry.end() )
	{
		m_featureRegistry.insert( std::make_pair( feature->GetID(), feature ) );
	}
}


//---------------------------------------------------------------------------
void Map::RemoveFeatureFromRegistry( unsigned int id )
{
	if ( m_featureRegistry.find( id ) != m_featureRegistry.end() )
	{
		m_featureRegistry[ id ] = nullptr;
	}
}


//---------------------------------------------------------------------------
void Map::ClearCellVisibilityFlags()
{
	for ( unsigned int index = 0; index < ( unsigned int ) m_cells.size(); ++ index )
	{
		if ( m_cells.at( index ).GetCellVisibility() > 0.f )
		{
			m_cells.at( index ).SetCellVisibility( 0.05f );
		}
		//m_cells[ index ].SetIsCellVisible( false );
	}
}


//---------------------------------------------------------------------------
void Map::SetIsMapVisible( bool isVisible )
{
	m_isMapVisible = isVisible;
}


//---------------------------------------------------------------------------
void Map::SendFactionAttackEvent( const Agent* attacker, const Agent* defender )
{
	NPCMap::iterator iter;
	for ( iter = m_npcRegistry.begin(); iter != m_npcRegistry.end(); ++ iter )
	{
		if ( iter->second != nullptr ) iter->second->HandleFactionAttackEvent( attacker, defender );
	}
}


//---------------------------------------------------------------------------
void Map::Update()
{
	FeatureMap::iterator featureIter;
	for ( featureIter = m_featureRegistry.begin(); featureIter != m_featureRegistry.end(); ++ featureIter )
	{
		if ( featureIter->second == nullptr ) continue;

		featureIter->second->Update();
	}

	NPCMap::iterator npcIter;
	for ( npcIter = m_npcRegistry.begin(); npcIter != m_npcRegistry.end(); ++ npcIter )
	{
		if ( npcIter->second == nullptr ) continue;

		npcIter->second->Update();

		if ( npcIter->second->IsDead() )
		{
			g_gameMessageLog->LogPrintf( Colors::RED, "The %s died!", npcIter->second->GetName().c_str() );
			delete npcIter->second;
		}
	}
}


//---------------------------------------------------------------------------
void Map::Render()
{
	std::vector< Vertex3D > vertices;
	CellCoords currentCellCoords;
	Vertex3D currentVertex;
	char glyphToRender;
	Rgba colorToRender;
	char glyphAsString[ 2 ];
	glyphAsString[ 1 ] = '\0';

	g_theRenderer->LoadIdentityMatrix();
	g_theRenderer->ApplyOrthoProjection( 0.f, ( float ) GAME_WIDTH_CELLS, 0.f, ( float ) GAME_HEIGHT_CELLS, 0.f, 1.f );

	g_theRenderer->PushMatrix();
	g_theRenderer->ApplyTranslation( Vec3f( ( float ) GAME_CELL_MULTIPLIER, ( float ) GAME_CELL_MULTIPLIER, 0.f ) );

	// populate vertices
	for ( CellIndex index = 0; index < ( CellIndex ) m_cells.size(); ++ index )
	{
		Cell& currentCell = m_cells.at( index );
		float visibility = currentCell.GetCellVisibility();
		currentCellCoords = GetCellCoordsForCellIndex( index );
		//colorToRender = Rgba( ( unsigned char ) ( 255.f * visibility ), ( unsigned char ) ( 255.f * visibility ), ( unsigned char ) ( 255.f * visibility ), 255 )

		// TODO: Put NPC drawing in a separate loop
		if ( currentCell.IsCellOccupiedByAgent() && visibility > 0.2f || currentCell.IsCellOccupiedByAgent() && m_isMapVisible )
		{
			glyphToRender = currentCell.GetOccupyingAgent()->GetGlyph();
			colorToRender = currentCell.GetOccupyingAgent()->GetColor();
		}
		else if ( currentCell.IsCellOccupiedByFeature() && visibility > 0.2f || currentCell.IsCellOccupiedByAgent() && m_isMapVisible )
		{
			glyphToRender = currentCell.GetOccupyingFeature()->GetCurrentGlyph();
			colorToRender = currentCell.GetOccupyingFeature()->GetColor();
		}
		else if ( currentCell.DoesCellHaveItems() && visibility > 0.2f || currentCell.IsCellOccupiedByAgent() && m_isMapVisible )
		{
			if ( currentCell.GetOccupyingItems()->size() > 1 )
			{
				glyphToRender = '&';
				colorToRender = Colors::YELLOW;
			}
			else
			{
				glyphToRender = currentCell.GetOccupyingItems()->back()->GetGlyph();
				colorToRender = currentCell.GetOccupyingItems()->back()->GetColor();
			}
		}
		else
		{
			glyphToRender = currentCell.GetCellGlyph();
			colorToRender = currentCell.GetCellColor();
		}

		if ( !m_isMapVisible )
		{
			colorToRender.r = ( unsigned char ) ( ( float ) colorToRender.r * visibility );
			colorToRender.g = ( unsigned char ) ( ( float ) colorToRender.g * visibility );
			colorToRender.b = ( unsigned char ) ( ( float ) colorToRender.b * visibility );
		}

// 		CellCoords coords = GetCellCoordsForCellIndex( index );
// 		g_theRenderer->VennLoadMatrixf( g_theRenderer->GetCurrentMatrixData() );
// 		g_theRenderer->VennPushMatrix();
// 		g_theRenderer->VennTranslatef( ( float ) coords.x, ( float ) coords.y, 0.f );
// 		g_theRenderer->VennUseProgram( 0 );
// 		//g_theRenderer->VennColor3f(  );
// 		//DrawQuad();
// 		//DrawQuadWithShader( g_currentShaderProgramID );
// 		DrawQuadColor( Vec3f( ( ( float ) colorToRender.r ) / 255.f, ( ( float ) colorToRender.g ) / 255.f, ( ( float ) colorToRender.b ) / 255.f ) );
// 		g_theRenderer->VennPopMatrix();
// 		g_theRenderer->VennUseProgram( g_currentShaderProgramID );
		glyphAsString[ 0 ] = glyphToRender;

		g_theTextRenderer->RenderText2D( 
		glyphAsString, 
		1.5f, 
		Vec2f( ( float ) currentCellCoords.x, ( float ) currentCellCoords.y ), 
		colorToRender, 
		g_currentShaderProgramID );
	}

// 	for ( NPCMap::iterator iter = m_npcRegistry.begin(); iter != m_npcRegistry.end(); ++ iter )
// 	{
// 		NPC& currentNPC = *iter->second;
// 		Cell& npcCell = GetCell( currentNPC.GetPosition() );
// 
// 		if ( npcCell.IsCellVisible() || m_isMapVisible )
// 		{
// 			std::string glyphString( 1, currentNPC.GetGlyph() );
// 
// 			g_theTextRenderer->RenderText2D( 
// 				glyphString, 
// 				1.5f, 
// 				Vec2f( ( float ) currentNPC.GetPosition().x, ( float ) currentNPC.GetPosition().y ), 
// 				currentNPC.GetColor(), 
// 				g_currentShaderProgramID );
// 		}
// 	}

	g_theRenderer->PopMatrix();
}


//---------------------------------------------------------------------------
void Map::SaveMapToXML( TiXmlNode* node, std::vector< Item* >& itemList )
{
	SaveMapCellsToXML( node, itemList );
	SaveNPCsToXML( node, itemList );
}


//---------------------------------------------------------------------------
void Map::SaveMapCellsToXML( TiXmlNode* node, std::vector< Item* >& itemList )
{
	std::vector< Feature* > features;
	std::string terrainData;
	std::string visibilityData;
	std::map< unsigned char, CellProperties > currentLegend;


	// link legend node
	TiXmlElement* legend = new TiXmlElement( "Legend" );
	node->LinkEndChild( legend );

	// link the terrain node
	TiXmlElement* terrain = new TiXmlElement( "TerrainData" );
	node->LinkEndChild( terrain );

	// link the visibility node
	TiXmlElement* visibility = new TiXmlElement( "VisibilityData" );
	node->LinkEndChild( visibility );

	// loop with a double loop on the cell list so we can easily insert newlines into the text string
	for ( int y = MAP_HEIGHT_CELLS - 1; y >= 0; -- y )
	{
		for ( int x = 0; x < MAP_WIDTH_CELLS; ++ x )
		{
			Cell* cell = GetCell( CellCoords( x, y ) );
			if ( cell == nullptr ) continue;

			CellProperties properties = cell->GetCellProperties();
			if ( currentLegend.find( properties.m_legendGlyph ) == currentLegend.end() )
			{
				currentLegend.insert( std::make_pair( properties.m_legendGlyph, properties ) );
			}
			terrainData += properties.m_legendGlyph;

			if ( cell->GetCellVisibility() > 0.f )
			{
				visibilityData += 'V'; // VISIBLE
			}
			else
			{
				visibilityData += 'H'; // HIDDEN
			}

			// push back any items that are in this cell
			if ( cell->DoesCellHaveItems() )
			{
				const OccupyingItems* cellItems = cell->GetOccupyingItems();
				for ( unsigned int index = 0; index < ( unsigned int ) cellItems->size(); ++ index )
				{
					itemList.push_back( cellItems->at( index ) );
				}
			}

			if ( cell->IsCellOccupiedByFeature() )
			{
				features.push_back( cell->GetOccupyingFeature() );
			}
		}
		
		TiXmlElement* terrainLine = new TiXmlElement( "Terrain" );
		terrainLine->LinkEndChild( new TiXmlText( terrainData.c_str() ) );
		terrain->LinkEndChild( terrainLine );

		TiXmlElement* visibilityLine = new TiXmlElement( "Visibility" );
		visibilityLine->LinkEndChild( new TiXmlText( visibilityData.c_str() ) );
		visibility->LinkEndChild( visibilityLine );

		terrainData.clear();
		visibilityData.clear();
// 		terrainData += '\n';
// 		visibilityData += '\n';
	}

	// write out legend data
	for ( std::map< unsigned char, CellProperties >::iterator iter = currentLegend.begin(); iter != currentLegend.end(); ++ iter )
	{
		TiXmlElement* glyphNode = new TiXmlElement( "Glyph" );
		glyphNode->SetAttribute( "legend", iter->second.m_legendGlyph );
		glyphNode->SetAttribute( "glyph", iter->second.m_glyph );
		glyphNode->SetAttribute( "r", iter->second.m_color.r );
		glyphNode->SetAttribute( "g", iter->second.m_color.g );
		glyphNode->SetAttribute( "b", iter->second.m_color.b );

		legend->LinkEndChild( glyphNode );
	}

	if ( features.empty() ) return;

	// link feature node
	TiXmlElement* featureElement = new TiXmlElement( "Features" );
	node->LinkEndChild( featureElement );
	for ( unsigned int index = 0; index < ( unsigned int ) features.size(); ++ index )
	{
		features.at( index )->SaveToXML( featureElement );
	}
}


//---------------------------------------------------------------------------
void Map::SaveNPCsToXML( TiXmlNode* node, std::vector< Item* >& itemList )
{
	TiXmlElement* npcs = new TiXmlElement( "NPCs" );
	NPCMap::iterator iter;

	for ( iter = m_npcRegistry.begin(); iter != m_npcRegistry.end(); ++ iter )
	{
		if ( iter->second == nullptr ) continue;
		NPC& currentNPC = *iter->second;
		currentNPC.SaveToXMLAndPopulateItemList( npcs, itemList );
	}

	node->LinkEndChild( npcs );
}