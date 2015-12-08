//---------------------------------------------------------------------------
// Faction.cpp
//---------------------------------------------------------------------------

#include "Engine/External/TinyXML/tinyxml.h"
#include "Engine/Core/TinyXMLHelper.hpp"
#include "Engine/Core/StringTable.hpp"
#include "Faction.hpp"
#define STATIC

STATIC FactionMap Faction::s_factionMap;

//---------------------------------------------------------------------------
Faction::Faction()
	: m_factionNameID( 0 )
{

}


//---------------------------------------------------------------------------
Faction::Faction( const Faction& otherFaction )
	: m_factionNameID( otherFaction.GetFactionNameID() )
{
	FactionRelationshipMap::const_iterator iter;
	for ( iter = otherFaction.m_factionRelationshipMap.begin(); iter != otherFaction.m_factionRelationshipMap.end(); ++ iter )
	{
		m_factionRelationshipMap.insert( std::make_pair( iter->first, iter->second ) );
	}
}


//---------------------------------------------------------------------------
Faction::~Faction()
{

}


//---------------------------------------------------------------------------
STATIC void Faction::DestroyFactionMap()
{
	FactionMap::iterator iter;
	for ( iter = s_factionMap.begin(); iter != s_factionMap.end(); ++ iter )
	{
		delete iter->second;
	}
	s_factionMap.clear();
}


//---------------------------------------------------------------------------
STATIC Faction* Faction::SpawnFaction( unsigned int factionID )
{
	return new Faction( *s_factionMap[ factionID ] );
}


//---------------------------------------------------------------------------
void Faction::AdjustRelationship( unsigned int factionID, int deltaValue )
{
	m_factionRelationshipMap[ factionID ].AdjustRelationshipValue( deltaValue );
}


//---------------------------------------------------------------------------
int Faction::GetRelationshipValue( unsigned int factionID )
{
	return m_factionRelationshipMap[ factionID ].GetValue();
}


//---------------------------------------------------------------------------
void Faction::SaveFactionToXML( TiXmlNode* node )
{
	TiXmlElement* faction = new TiXmlElement( "Faction" );
	node->LinkEndChild( faction );

	FactionRelationshipMap::iterator iter;
	for ( iter = m_factionRelationshipMap.begin(); iter != m_factionRelationshipMap.end(); ++ iter )
	{
		TiXmlElement* relationship = new TiXmlElement( "FactionRelationship" );
		FactionRelationship& currentRelationship = iter->second;
		
		relationship->SetAttribute( "name", StringValue( currentRelationship.GetFactionID() ) );
		relationship->SetAttribute( "value", currentRelationship.GetValue() );
		faction->LinkEndChild( relationship );
	}
}


//---------------------------------------------------------------------------
void Faction::LoadFactionFromXML( TiXmlNode* node )
{
	for ( TiXmlElement* factionRelationship = node->FirstChildElement( "FactionRelationship" ); factionRelationship != nullptr; factionRelationship = factionRelationship->NextSiblingElement( "FactionRelationship" ) )
	{
		FactionRelationship& relationship = m_factionRelationshipMap[ StringID( GetAttributeAsString( factionRelationship, "name") ) ];
		relationship.SetValue( GetAttributeAsInt( factionRelationship, "value" ) );
	}
}


//---------------------------------------------------------------------------
STATIC void Faction::GenerateFactionsFromXML( const char* filePath )
{
	TiXmlDocument doc;
	bool isLoaded = doc.LoadFile( filePath );

	if ( !isLoaded ) return;

	std::string name;
	std::string hatedFactions;
	std::string dislikedFactions;
	std::string neutralFactions;
	std::string likedFactions;
	std::string lovedFactions;

	TiXmlElement* root = doc.RootElement();

	for ( TiXmlElement* element = root->FirstChildElement( "Faction" ); element != nullptr; element = element->NextSiblingElement( "Faction" ) )
	{
		Faction* faction = new Faction();
		name = GetAttributeAsString( element, "name" );
		hatedFactions = GetAttributeAsString( element, "hated" );
		dislikedFactions = GetAttributeAsString( element, "disliked" );
		neutralFactions = GetAttributeAsString( element, "neutral" );
		likedFactions = GetAttributeAsString( element, "liked" );
		lovedFactions = GetAttributeAsString( element, "loved" );

		faction->m_factionNameID = StringID( name );

		faction->ParseFactionsAndAddRelationships( hatedFactions, ( int ) FACTION_HATED );
		faction->ParseFactionsAndAddRelationships( dislikedFactions, ( int ) FACTION_DISLIKED );
		faction->ParseFactionsAndAddRelationships( neutralFactions, ( int ) FACTION_NEUTRAL );
		faction->ParseFactionsAndAddRelationships( likedFactions, ( int ) FACTION_LIKED );
		faction->ParseFactionsAndAddRelationships( lovedFactions, ( int ) FACTION_LOVED );

		s_factionMap.insert( std::make_pair( faction->m_factionNameID, faction ) );
	}
}


//---------------------------------------------------------------------------
void Faction::AddFactionRelationship( const std::string& factionName, const int value )
{
	int stringID = StringID( factionName );
	if ( m_factionRelationshipMap.find( stringID ) == m_factionRelationshipMap.end() )
	{
		m_factionRelationshipMap.insert( std::make_pair( stringID, FactionRelationship( stringID, value ) ) );
	}
}


//---------------------------------------------------------------------------
void Faction::ParseFactionsAndAddRelationships( const std::string& factions, const int value )
{
	size_t previousPosition = 0;
	size_t currentPosition = 0;

	while ( ( currentPosition = factions.find_first_of( ',', previousPosition ) ) != std::string::npos )
	{
		if ( currentPosition > previousPosition )
		{
			AddFactionRelationship( factions.substr( previousPosition, currentPosition - previousPosition ), value );
		}
		previousPosition = currentPosition + 1;
	}

	if ( previousPosition < factions.length() )
	{
		AddFactionRelationship( factions.substr( previousPosition, std::string::npos ), value );
	}
}