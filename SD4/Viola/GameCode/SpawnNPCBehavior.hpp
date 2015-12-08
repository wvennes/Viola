//---------------------------------------------------------------------------
// SpawnNPCBehavior.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_SPAWNNPCBEHAVIOR_
#define _INCLUDED_SPAWNNPCBEHAVIOR_

#include "BaseAIBehavior.hpp"

/////////////////////////////////////////////////////////////////////////////
class SpawnNPCBehavior : public BaseAIBehavior
{
public:
	static BaseAIBehavior*			CreateBehaviorFromXMLNode( TiXmlElement* element );
	float							Evaluate( Map& map );
	bool							Think( Map& map );
	BaseAIBehavior*					Clone();
	~SpawnNPCBehavior() {}

protected:
	SpawnNPCBehavior( const char* name ); // TO DO: Create from XML node

protected:
	static AIBehaviorRegistration	s_spawnNPCBehaviorRegistration;

	int								m_spawnThreshold;
	float							m_chanceToSpawnNPC;
	std::string						m_npcName;

};

#endif