//---------------------------------------------------------------------------
// NPCFactory.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_NPCFACTORY_
#define _INCLUDED_NPCFACTORY_

#include <set>
#include <vector>

class NPC;
class NPCFactory;
class Faction;
class TiXmlElement;
class BaseAIBehavior;

typedef std::set< NPCFactory* >	NPCFactorySet;

/////////////////////////////////////////////////////////////////////////////
class NPCFactory
{
public:
	NPCFactory( TiXmlElement* factoryBlueprintNode );
	~NPCFactory();

	static void						DestroyFactories();
	void							PopulateFromXML( TiXmlElement* factoryBlueprintNode );
	static bool						DoesFactoryExist( const std::string& name );
	NPC*							SpawnNPC();

	std::string						GetName() { return m_name; }
	static NPCFactory*				GetFactory( const std::string& name );

protected:
	std::string						m_name;
	int								m_factionID;
	int								m_movementProperties;
	char							m_glyph;
	float							m_maxHealth;
	float							m_baseAttackDamage;
	float							m_fieldOfViewRadius;
	Rgba							m_color;
	std::vector< BaseAIBehavior* >	m_aiBehaviors;
	static NPCFactorySet			s_factorySet;

};

#endif