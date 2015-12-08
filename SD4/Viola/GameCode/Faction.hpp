//---------------------------------------------------------------------------
// Faction.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_FACTION_
#define _INCLUDED_FACTION_

#include <map>
#include "FactionRelationship.hpp"

class TiXmlElement;
class TiXmlNode;
class Faction;

typedef std::map< unsigned int, FactionRelationship >	FactionRelationshipMap;
typedef std::map< unsigned int, Faction* >				FactionMap;

/////////////////////////////////////////////////////////////////////////////
enum FactionStatus
{
	FACTION_HATED		= -10,
	FACTION_DISLIKED	= -1,
	FACTION_NEUTRAL		= 0,
	FACTION_LIKED		= 1,
	FACTION_LOVED		= 10,
};


/////////////////////////////////////////////////////////////////////////////
enum FactionAction
{
	FACTION_ACTION_KILLED_ALLY		= -12,
	FACTION_ACTION_DAMAGED_SELF		= -5,
	FACTION_ACTION_TRIED_TO_DAMAGE	= -2,
	FACTION_ACTION_DAMAGED_ALLY		= -2,
	FACTION_ACTION_DAMAGED_ENEMY	= 1,
	FACTION_ACTION_HEALED_ALLY		= 2,
	FACTION_ACTION_HEALED_SELF		= 5,
	FACTION_ACTION_KILLED_ENEMY		= 8,
	FACTION_ACTION_SAVED_ALLY		= 12,
	FACTION_ACTION_SAVED_SELF		= 100,
};


/////////////////////////////////////////////////////////////////////////////
class Faction
{
public:
	Faction();
	Faction( const Faction& otherFaction );
	~Faction();

	static void					GenerateFactionsFromXML( const char* filePath );
	static void					DestroyFactionMap();
	static Faction*				SpawnFaction( unsigned int factionID );
	void						AdjustRelationship( unsigned int factionID, int deltaValue );
	int							GetRelationshipValue( unsigned int factionID );
	int							GetFactionNameID() const { return m_factionNameID; }
	void						SaveFactionToXML( TiXmlNode* node );
	void						LoadFactionFromXML( TiXmlNode* node );

private:
	int							m_factionNameID;
	FactionRelationshipMap		m_factionRelationshipMap;
	static FactionMap			s_factionMap;
	
private:
	void						AddFactionRelationship( const std::string& factionName, const int value );
	void						ParseFactionsAndAddRelationships( const std::string& factions, const int value );

};

#endif