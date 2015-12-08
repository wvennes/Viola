//---------------------------------------------------------------------------
// NPC.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/TinyXMLHelper.hpp"
#include "BaseAIBehavior.hpp"
#include "Faction.hpp"
#include "Item.hpp"
#include "NPC.hpp"


//---------------------------------------------------------------------------
NPC::NPC( const std::string& name, char glyph )
	: Agent( name, glyph )
{
	// do nothing yet
}


//---------------------------------------------------------------------------
NPC::NPC( const NPC& otherNPC )
	: Agent( otherNPC )
{
	for ( unsigned int index = 0; index < ( unsigned int ) otherNPC.m_aiBehaviors.size(); ++ index )
	{
		AddAIBehavior( otherNPC.m_aiBehaviors.at( index ) );
	}
}


//---------------------------------------------------------------------------
NPC::~NPC()
{
	RemoveFromMap();
// 	for ( unsigned int index = 0; index < ( unsigned int ) m_aiBehaviors.size(); ++ index )
// 	{
// 		delete m_aiBehaviors.at( index );
// 	}
	DestroyVector( m_aiBehaviors );

	m_aiBehaviors.clear();
	std::vector< BaseAIBehavior* >().swap( m_aiBehaviors );
}


//---------------------------------------------------------------------------
bool NPC::Update()
{
	Agent::Update();
	BaseAIBehavior* highestScoringBehavior = ChooseBehaviorToThink();
	if ( highestScoringBehavior != nullptr )
	{
		highestScoringBehavior->Think( *m_map );
		return true;
	}
	return false;
}


//---------------------------------------------------------------------------
BaseAIBehavior* NPC::ChooseBehaviorToThink()
{
	float currentHighestScore = 0.f;
	float score = 0.f;
	BaseAIBehavior* behaviorToReturn = nullptr;
	for ( unsigned int index = 0; index < ( unsigned int ) m_aiBehaviors.size(); ++ index )
	{
		score = m_aiBehaviors.at( index )->Evaluate( *m_map );
		if ( score > currentHighestScore )
		{
			behaviorToReturn = m_aiBehaviors.at( index );
			currentHighestScore = score;
		}
	}

	return behaviorToReturn;
}


//---------------------------------------------------------------------------
void NPC::AddAIBehavior( BaseAIBehavior* aiBehavior )
{
	BaseAIBehavior* clonedBehavior = aiBehavior->Clone();
	clonedBehavior->SetOwningAgent( this );
	m_aiBehaviors.push_back( clonedBehavior );
}


//---------------------------------------------------------------------------
void NPC::AddToMap( Map* map )
{
	m_map = map;
	m_map->AddNPCToRegistry( this );
	Cell* cell = m_map->GetCell( m_position );
	if ( cell == nullptr ) return;
	cell->SetOccupyingAgent( this );
}


//---------------------------------------------------------------------------
void NPC::RemoveFromMap()
{
	m_map->RemoveNPCFromRegistry( m_id );
	Agent::RemoveFromMap();
}


//---------------------------------------------------------------------------
void NPC::HandleFactionAttackEvent( const Agent* attacker, const Agent* defender )
{
	Vec2i distanceFromEvent = m_position - defender->GetPosition();
	int distance = abs( distanceFromEvent.x ) + abs( distanceFromEvent.y );

	if ( distance > 10 ) return; // do nothing

	int defendingFactionID = defender->GetFaction()->GetFactionNameID();
	int relationshipChange = 0;

	if ( defender == this )
	{
		relationshipChange = FACTION_ACTION_DAMAGED_SELF;
	}
	else if ( m_faction->GetRelationshipValue( defendingFactionID ) > 0 )
	{
		if ( defender->IsDead() ) relationshipChange = FACTION_ACTION_KILLED_ALLY;
		else relationshipChange = FACTION_ACTION_DAMAGED_ALLY;
	}
	else if ( m_faction->GetRelationshipValue( defendingFactionID ) < 0 )
	{
		if ( defender->IsDead() ) relationshipChange = FACTION_ACTION_KILLED_ENEMY;
		else relationshipChange = FACTION_ACTION_DAMAGED_ENEMY;
	}

	m_faction->AdjustRelationship( attacker->GetFaction()->GetFactionNameID(), relationshipChange );
}


//---------------------------------------------------------------------------
void NPC::SaveToXMLAndPopulateItemList( TiXmlNode* node, std::vector< Item* >& itemList )
{
	TiXmlElement* npc = new TiXmlElement( "NPC" );

	npc->SetAttribute( "id", m_id );
	npc->SetAttribute( "name", m_name.c_str() );
	npc->SetAttribute( "x", m_position.x );
	npc->SetAttribute( "y", m_position.y );
	npc->SetDoubleAttribute( "health", m_currentHealth );

	// save faction relationships to XML
	m_faction->SaveFactionToXML( npc );

	// inventory
	if ( !m_inventory.empty() )
	{
		TiXmlElement* inventory = new TiXmlElement( "Inventory" );

		Inventory::iterator inventoryIter;
		for ( inventoryIter = m_inventory.begin(); inventoryIter != m_inventory.end(); ++ inventoryIter )
		{
			TiXmlElement* item = new TiXmlElement( "Item" );
			item->SetAttribute( "id", ( *inventoryIter )->GetID() );
			itemList.push_back( *inventoryIter );
			inventory->LinkEndChild( item );
		}

		npc->LinkEndChild( inventory );
	}

	// weapon list
	if ( !m_weapons.empty() )
	{
		TiXmlElement* weapons = new TiXmlElement( "Weapons" );

		Weapons::iterator weaponIter;
		for ( weaponIter = m_weapons.begin(); weaponIter != m_weapons.end(); ++ weaponIter )
		{
			TiXmlElement* item = new TiXmlElement( "Item" );
			item->SetAttribute( "id", ( *weaponIter )->GetID() );
			itemList.push_back( *weaponIter );
			weapons->LinkEndChild( item );
		}

		npc->LinkEndChild( weapons );
	}

	// armor list
	if ( !m_armors.empty() )
	{
		TiXmlElement* armors = new TiXmlElement( "Armors" );

		Armors::iterator armorIter;
		for ( armorIter = m_armors.begin(); armorIter != m_armors.end(); ++ armorIter )
		{
			TiXmlElement* item = new TiXmlElement( "Item" );
			item->SetAttribute( "id", ( *armorIter )->GetID() );
			itemList.push_back( *armorIter );
			armors->LinkEndChild( item );
		}
		
		npc->LinkEndChild( armors );
	}

	// equipped weapon
	if ( m_equippedWeapon != nullptr )
	{
		TiXmlElement* equippedWeapon = new TiXmlElement( "EquippedWeapon" );
		equippedWeapon->SetAttribute( "id", m_equippedWeapon->GetID() );
		itemList.push_back( m_equippedWeapon );
		npc->LinkEndChild( equippedWeapon );
	}

	// equipped armor
	if ( m_equippedArmor != nullptr )
	{
		TiXmlElement* equippedArmor = new TiXmlElement( "EquippedArmor" );
		equippedArmor->SetAttribute( "id", m_equippedArmor->GetID() );
		itemList.push_back( m_equippedArmor );
		npc->LinkEndChild( equippedArmor );
	}

	node->LinkEndChild( npc );
}


//---------------------------------------------------------------------------
// NOTE: Don't call this outside of an entire load from xml, as it stores temporary ID values as item pointers - NOT SAFE
void NPC::LoadFromXML( TiXmlElement* node )
{
	Agent::LoadFromXML( node );
}