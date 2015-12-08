//---------------------------------------------------------------------------
// NPC.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_NPC_
#define _INCLUDED_NPC_

#include "Agent.hpp"

class BaseAIBehavior;


/////////////////////////////////////////////////////////////////////////////
class NPC : public Agent
{
public:
	NPC( const std::string& name, char glyph );
	NPC( const NPC& otherNPC );
	~NPC();

	bool							Update();
	void							AddAIBehavior( BaseAIBehavior* aiBehavior );
	void							AddToMap( Map* map );
	void							RemoveFromMap();
	void							HandleFactionAttackEvent( const Agent* attacker, const Agent* defender );
	void							SaveToXMLAndPopulateItemList( TiXmlNode* node, std::vector< Item* >& itemList );
	void							LoadFromXML( TiXmlElement* node );

private:
	std::vector< BaseAIBehavior* >	m_aiBehaviors;

private:
	BaseAIBehavior*					ChooseBehaviorToThink();

};

#endif