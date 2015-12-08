//---------------------------------------------------------------------------
// Feature.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_FEATURE_
#define _INCLUDED_FEATURE_

#include "GameEntity.hpp"

/////////////////////////////////////////////////////////////////////////////
enum FeatureType
{
	FEATURE_TYPE_UNDEFINED,
	FEATURE_TYPE_DOOR,
	FEATURE_TYPE_LIGHT,
	NUM_FEATURE_TYPES
};


/////////////////////////////////////////////////////////////////////////////
class Feature : public GameEntity
{
public:
	Feature( const std::string& name, char activatedGlyph, char deactivatedGlyph, FeatureType featureType );
	~Feature();

	FeatureType		GetFeatureType() const { return m_featureType; }
	char			GetActivatedGlyph() const { return m_activatedGlyph; }
	char			GetDeactivatedGlyph() const { return GetGlyph(); }
	char			GetCurrentGlyph() const;
	bool			IsActivated() const { return m_isActivated; }
	bool			CanWalkOn() const { return m_canWalkOn; }
	bool			DoesApplyLighting() const { return m_doesApplyLighting; }

	void			SetFeatureType( FeatureType featureType ) { m_featureType = featureType; }
	void			SetActivatedGlyph( char activatedGlyph ) { m_activatedGlyph = activatedGlyph; }
	void			SetDeactivatedGlyph( char deactivatedGlyph ) { SetGlyph( deactivatedGlyph ); }
	void			SetCanWalkOn( bool canWalkOn ) { m_canWalkOn = canWalkOn; }
	void			SetDoesApplyLighting( bool doesApplyLighting ) { m_doesApplyLighting = doesApplyLighting; }

	void			SwapActivatedGlyphs();

	void			AddToMap( Map* map );
	void			RemoveFromMap();
	void			UseFeature();

	void			SaveToXML( TiXmlNode* node );

	bool			Update();

protected:
	bool			m_isActivated;
	bool			m_canWalkOn;
	bool			m_doesApplyLighting;
	char			m_activatedGlyph;
	FeatureType		m_featureType;

};

#endif