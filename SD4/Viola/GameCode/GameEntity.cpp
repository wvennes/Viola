//---------------------------------------------------------------------------
// GameEntity.cpp
//---------------------------------------------------------------------------

#include "Engine/Math/GameMath.hpp"
#include "Cell.hpp"
#include "Map.hpp"
#include "Feature.hpp"
#include "MovementProperties.hpp"
#include "GameEntity.hpp"
#define STATIC
#define UNUSED(x) (void)(x)

STATIC int GameEntity::s_nextGameEntityID = 1;

//---------------------------------------------------------------------------
GameEntity::GameEntity()
	: m_position( Vec2i( 0, 0 ) )
	, m_name( "unnamed" )
	, m_glyph( '!' )
	, m_color( Colors::WHITE )
	, m_map( nullptr )
	, m_maxHealth( 10.f )
	, m_currentHealth( 10.f )
	, m_fieldOfViewRadius( 0.f )
	, m_debugLevel( 1 )
	, m_id( s_nextGameEntityID ++ )
{

}


//---------------------------------------------------------------------------
GameEntity::GameEntity( const GameEntity& otherEntity )
	: m_position( otherEntity.GetPosition() )
	, m_name( otherEntity.GetName() )
	, m_glyph( otherEntity.GetGlyph() )
	, m_color( otherEntity.GetColor() )
	, m_map( nullptr )
	, m_maxHealth( otherEntity.GetMaxHealth() )
	, m_currentHealth( otherEntity.GetCurrentHealth() )
	, m_fieldOfViewRadius( otherEntity.GetFieldOfViewRadius() )
	, m_debugLevel( otherEntity.GetDebugLevel() )
	, m_id( s_nextGameEntityID ++ )
{

}


//---------------------------------------------------------------------------
GameEntity::GameEntity( const std::string& name, char glyph )
	: m_position( Vec2i( 0, 0 ) )
	, m_name( name )
	, m_glyph( glyph )
	, m_color( Colors::WHITE )
	, m_map( nullptr )
	, m_maxHealth( 10.f )
	, m_currentHealth( 10.f )
	, m_fieldOfViewRadius( 0.f )
	, m_debugLevel( 1 )
	, m_id( s_nextGameEntityID ++ )
{

}


//---------------------------------------------------------------------------
GameEntity::GameEntity( const std::string& name, const Vec2i& position, char glyph, const Rgba& color, float maxHealth, int debugLevel )
	: m_position( position )
	, m_name( name )
	, m_glyph( glyph )
	, m_color( color )
	, m_map( nullptr )
	, m_maxHealth( maxHealth )
	, m_currentHealth( maxHealth )
	, m_fieldOfViewRadius( 0.f )
	, m_debugLevel( debugLevel )
	, m_id( s_nextGameEntityID ++ )
{

}


//---------------------------------------------------------------------------
GameEntity::~GameEntity()
{
// 	CellSet::iterator iter;
// 	for ( iter = m_fieldOfViewResult.m_cellsHit.begin(); iter != m_fieldOfViewResult.m_cellsHit.end(); ++ iter )
// 	{
// 		delete *iter;
// 	}
	m_fieldOfViewResult.m_entitiesHit.clear();
	//std::set< Vec2i >().swap( m_fieldOfViewResult.m_cellsHit );
}


//---------------------------------------------------------------------------
FieldOfViewResult GameEntity::GetFieldOfViewResult( Map* map, float radius, bool shouldLightCells )
{
	FieldOfViewResult result;
	//return result;
	Vec2f unitVector;
	for ( Degrees currentDegrees = 0; currentDegrees < 360.f; ++ currentDegrees )
	{
		unitVector = Vec2f( CosDegrees( currentDegrees ), SinDegrees( currentDegrees ) );
		CastRay( map, radius, unitVector, shouldLightCells, result );
	}

// 	std::set< Cell* >::iterator iter;
// 	for ( iter = result.m_cellsHit.begin(); iter != result.m_cellsHit.end(); ++ iter )
// 	{
// 		if ( ( *iter )->IsCellOccupied() )
// 		{
// 			result.m_entitiesHit.insert( ( *iter )->GetOccupyingAgent() );
// 		}
// 	}
	return result;
}


//---------------------------------------------------------------------------
void GameEntity::GetFieldOfViewResult( Map* map, float radius, bool shouldLightCells, FieldOfViewResult& result_out )
{
	//return;
	result_out.m_entitiesHit.clear();
	Vec2f unitVector;
	for ( Degrees currentDegrees = 0; currentDegrees < 360.f; currentDegrees += 2.f )
	{
		unitVector = Vec2f( CosDegrees( currentDegrees ), SinDegrees( currentDegrees ) );
		CastRay( map, radius, unitVector, shouldLightCells, result_out );
	}
}


//---------------------------------------------------------------------------
void GameEntity::CastRay( Map* map, float radius, const Vec2f& unitVector, bool shouldLightCells, FieldOfViewResult& outResult )
{
	//return;
	Vec2f positionOffset = Vec2f( ( float ) m_position.x, ( float ) m_position.y ) + Vec2f( 0.5f, 0.5f ); // start in the middle of the tile
	CellCoords tilePositionOffset = m_position;
	bool hasHitWall = false;
	for ( float count = 0; count < radius; ++ count )
	{	
		// return if cell is outside of the map
		Cell* hitCell = map->GetCell( tilePositionOffset );
		if ( hitCell == nullptr ) return;

		// insert if we have not hit a wall yet
		if ( !hasHitWall && !hitCell->IsType( TYPE_WALL ) && hitCell->IsCellOccupiedByAgent() )
		{
			outResult.m_entitiesHit.insert( ( GameEntity*) ( hitCell->GetOccupyingAgent() ) ); // add this cell to set of visited cells
		}

		// light up cells if bool is set
		if ( shouldLightCells )
		{
			if ( !hasHitWall )
			{
				// TODO: move the lighting range to a const global instead of magic numbers
				float lightValue = MapValueInOldRangeToNewRange( radius - count, 0.f, 10.f, 0.3f, 1.f );
				hitCell->SetCellVisibility( lightValue );
			}
// 			else
// 			{
// 				// TODO: move hard coded value elsewhere
// 				if ( hitCell->GetCellVisibility() > 0.2f )
// 				{
// 					hitCell->SetCellVisibility( 0.2f );
// 				}
// 			}
		}

		if ( hitCell->IsType( TYPE_WALL ) || 
			( hitCell->IsCellOccupiedByFeature() && !hitCell->GetOccupyingFeature()->CanWalkOn() && hitCell->GetOccupyingFeature()->GetFeatureType() != FEATURE_TYPE_LIGHT ) )
		{
			hasHitWall = true;
		}

		positionOffset += unitVector;
		tilePositionOffset = CellCoords( ( int ) positionOffset.x, ( int ) positionOffset.y );
	}

	// do one more step for lighting
	if ( shouldLightCells )
	{
		positionOffset += unitVector;
		tilePositionOffset = CellCoords( ( int ) positionOffset.x, ( int ) positionOffset.y );

		Cell* hitCell = map->GetCell( tilePositionOffset );
		if ( hitCell == nullptr ) return;
		if ( hitCell->GetCellVisibility() > 0.2f )
		{
			hitCell->SetCellVisibility( 0.2f );
		}
	}
}


//---------------------------------------------------------------------------
float GameEntity::ApplyDamage( float damageToInflict )
{
	m_currentHealth -= damageToInflict;
	if ( m_currentHealth > m_maxHealth ) m_currentHealth = m_maxHealth;
	return damageToInflict;
}


//---------------------------------------------------------------------------
void GameEntity::SaveToXML( TiXmlNode* node )
{
	UNUSED( node );
}


//---------------------------------------------------------------------------
void GameEntity::LoadFromXML( TiXmlElement* node )
{
	UNUSED( node );
}