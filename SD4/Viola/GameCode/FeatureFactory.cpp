//---------------------------------------------------------------------------
// FeatureFactory.cpp
//---------------------------------------------------------------------------

#include <iterator>
#include "Engine/External/TinyXML/tinyxml.h"
#include "Engine/Core/TinyXMLHelper.hpp"
#include "Engine/Core/Utility.hpp"
#include "Feature.hpp"
#include "FeatureFactory.hpp"
#define STATIC

STATIC FeatureFactorySet	FeatureFactory::s_factorySet;


//---------------------------------------------------------------------------
STATIC FeatureType FeatureFactory::GetAttributeAsFeatureType( TiXmlElement* element, const char* name )
{
	std::string value( element->Attribute( name ) );
	ToLower( value );

	if ( value == "door" )	return FEATURE_TYPE_DOOR;
	else if ( value == "light" ) return FEATURE_TYPE_LIGHT;

	return FEATURE_TYPE_UNDEFINED;
}


//---------------------------------------------------------------------------
FeatureFactory::FeatureFactory( TiXmlElement* factoryBlueprintNode )
	: m_canWalkOn( false )
	, m_deactivatedGlyph( ' ' )
	, m_activatedGlyph( ' ' )
	, m_fieldOfViewRadius( 0.f )
	, m_color( Colors::WHITE )
	, m_featureType( FEATURE_TYPE_UNDEFINED )
	, m_name( "name" )
{
	PopulateFromXML( factoryBlueprintNode );

	s_factorySet.insert( this );
}


//---------------------------------------------------------------------------
FeatureFactory::~FeatureFactory()
{

}


//---------------------------------------------------------------------------
STATIC void FeatureFactory::DestroyFactories()
{
	for ( FeatureFactorySet::iterator iter = s_factorySet.begin(); iter != s_factorySet.end(); ++ iter )
	{
		delete *iter;
	}

	s_factorySet.clear();
}


//---------------------------------------------------------------------------
void FeatureFactory::PopulateFromXML( TiXmlElement* factoryBlueprintNode )
{
//	std::string canWalkOn = GetAttributeAsString( factoryBlueprintNode, "canWalkOn" );
	m_name = GetAttributeAsString( factoryBlueprintNode, "name" );
	m_deactivatedGlyph = GetAttributeAsChar( factoryBlueprintNode, "deactivated" );
	m_activatedGlyph = GetAttributeAsChar( factoryBlueprintNode, "activated" );
	m_featureType = GetAttributeAsFeatureType( factoryBlueprintNode, "type" );
	m_fieldOfViewRadius = GetAttributeAsFloat( factoryBlueprintNode, "fieldOfViewRadius" );
	m_color.r = ( unsigned char ) GetAttributeAsInt( factoryBlueprintNode, "r" );
	m_color.g = ( unsigned char ) GetAttributeAsInt( factoryBlueprintNode, "g" );
	m_color.b = ( unsigned char ) GetAttributeAsInt( factoryBlueprintNode, "b" );

// 	if ( canWalkOn == "true" )
// 	{
// 		m_canWalkOn = true;
// 	}
// 	else
// 	{
// 		m_canWalkOn = false;
// 	}

	switch ( m_featureType )
	{
		case FEATURE_TYPE_UNDEFINED:
		case FEATURE_TYPE_DOOR:
		case FEATURE_TYPE_LIGHT:
		{
			m_canWalkOn = false;
			break;
		}
	}
}


//---------------------------------------------------------------------------
STATIC bool FeatureFactory::DoesFactoryExist( const std::string& name )
{
	for ( FeatureFactorySet::iterator iter = s_factorySet.begin(); iter != s_factorySet.end(); ++ iter )
	{
		FeatureFactory* factory = *iter;
		if ( name == factory->GetName() )
		{
			return true;
		}
	}

	return false;
}


//---------------------------------------------------------------------------
STATIC FeatureFactory* FeatureFactory::GetFactory( const std::string& name )
{
	for ( FeatureFactorySet::iterator iter = s_factorySet.begin(); iter != s_factorySet.end(); ++ iter )
	{
		FeatureFactory* factory = *iter;
		if ( name == factory->GetName() )
		{
			return factory;
		}
	}

	return nullptr;
}


//---------------------------------------------------------------------------
STATIC FeatureFactory* FeatureFactory::GetRandomFactory()
{
	FeatureFactorySet::iterator iter = s_factorySet.begin();
	std::advance( iter, rand() % s_factorySet.size() );
	return ( *iter );
}


//---------------------------------------------------------------------------
Feature* FeatureFactory::SpawnFeature()
{
	Feature* feature = new Feature( m_name, m_activatedGlyph, m_deactivatedGlyph, m_featureType );
	feature->SetColor( m_color );
	feature->SetCanWalkOn( m_canWalkOn );
	feature->SetFieldOfViewRadius( m_fieldOfViewRadius );
	return feature;
}