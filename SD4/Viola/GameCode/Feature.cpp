//---------------------------------------------------------------------------
// Feature.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/TinyXMLHelper.hpp"
#include "Cell.hpp"
#include "Map.hpp"
#include "Feature.hpp"


//---------------------------------------------------------------------------
Feature::Feature( const std::string& name, char activatedGlyph, char deactivatedGlyph, FeatureType featureType )
	: GameEntity( name, deactivatedGlyph )
	, m_isActivated( false )
	, m_canWalkOn( false )
	, m_doesApplyLighting( false )
	, m_activatedGlyph( activatedGlyph )
	, m_featureType( featureType )
{
	if ( m_featureType == FEATURE_TYPE_LIGHT ) m_doesApplyLighting = true;
}


//---------------------------------------------------------------------------
Feature::~Feature()
{
	RemoveFromMap();
}


//---------------------------------------------------------------------------
char Feature::GetCurrentGlyph() const
{
	if ( m_isActivated ) return m_activatedGlyph;
	else return m_glyph;
}


//---------------------------------------------------------------------------
void Feature::SwapActivatedGlyphs()
{
	char temp = m_glyph;
	m_glyph = m_activatedGlyph;
	m_activatedGlyph = temp;
}


//---------------------------------------------------------------------------
void Feature::AddToMap( Map* map )
{
	GameEntity::AddToMap( map );
	map->AddFeatureToRegistry( this );

	Cell* cell = m_map->GetCell( m_position );

	if ( cell == nullptr ) return;
	
	cell->SetOccupyingFeature( this );
	Update();
}


//---------------------------------------------------------------------------
void Feature::RemoveFromMap()
{
	m_map->RemoveFeatureFromRegistry( m_id );
	GameEntity::RemoveFromMap();
}


//---------------------------------------------------------------------------
void Feature::UseFeature()
{
	switch ( m_featureType )
	{
		case FEATURE_TYPE_UNDEFINED:
		case FEATURE_TYPE_DOOR:
		{
			m_isActivated = true;
			m_canWalkOn = true;
			break;
		}
		case FEATURE_TYPE_LIGHT:
		{
			m_isActivated = !m_isActivated;
			m_doesApplyLighting = !m_doesApplyLighting;
			break;
		}
	}
}


//---------------------------------------------------------------------------
void Feature::SaveToXML( TiXmlNode* node )
{
	TiXmlElement* feature = new TiXmlElement( "Feature" );
	feature->SetAttribute( "id", m_id );
	feature->SetAttribute( "name", m_name.c_str() );
	feature->SetAttribute( "activated", m_activatedGlyph );
	feature->SetAttribute( "deactivated", m_glyph );
	feature->SetAttribute( "isActivated", m_isActivated );
	feature->SetAttribute( "x", m_position.x );
	feature->SetAttribute( "y", m_position.y );
	node->LinkEndChild( feature );
}


//---------------------------------------------------------------------------
bool Feature::Update()
{
	if ( m_doesApplyLighting )
	{
		//m_fieldOfViewResult.m_entitiesHit.clear();
		GameEntity::GetFieldOfViewResult( m_map, m_fieldOfViewRadius, true, m_fieldOfViewResult );
	}
	return true;
}