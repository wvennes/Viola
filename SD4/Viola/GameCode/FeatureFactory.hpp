//---------------------------------------------------------------------------
// FeatureFactory.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_FEATUREFACTORY_
#define _INCLUDED_FEATUREFACTORY_

#include <set>
#include <vector>
#include "Feature.hpp"

class FeatureFactory;
class TiXmlElement;
class BaseAIBehavior;

typedef std::set< FeatureFactory* >	FeatureFactorySet;

/////////////////////////////////////////////////////////////////////////////
class FeatureFactory
{
public:
	FeatureFactory( TiXmlElement* factoryBlueprintNode );
	~FeatureFactory();

	static void						DestroyFactories();
	void							PopulateFromXML( TiXmlElement* factoryBlueprintNode );
	static bool						DoesFactoryExist( const std::string& name );
	Feature*						SpawnFeature();

	std::string						GetName() { return m_name; }
	static FeatureFactory*			GetFactory( const std::string& name );
	static FeatureFactory*			GetRandomFactory();

	static FeatureType				GetAttributeAsFeatureType( TiXmlElement* node, const char* name );

protected:
	bool							m_canWalkOn;
	char							m_deactivatedGlyph;
	char							m_activatedGlyph;
	float							m_fieldOfViewRadius;
	Rgba							m_color;
	FeatureType						m_featureType;
	std::string						m_name;
	static FeatureFactorySet		s_factorySet;

};

#endif