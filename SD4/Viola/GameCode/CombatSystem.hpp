//---------------------------------------------------------------------------
// CombatSystem.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_COMBATSYSTEM_
#define _INCLUDED_COMBATSYSTEM_

class GameEntity;
class Item;

/////////////////////////////////////////////////////////////////////////////
enum AttackTypes
{
	MELEE,
	MAGIC,
	NUM_ATTACKTYPES
};


/////////////////////////////////////////////////////////////////////////////
struct CombatInformation
{
	GameEntity*		m_attackingEntity;
	GameEntity*		m_defendingEntity;
	float			m_specificDamageToApply;
	AttackTypes		m_attackType;
	Item*			m_attackingWeapon;
	float			m_damageDealt_out;
	bool			m_didDefenderDie_out;

	CombatInformation()
		: m_attackingEntity( nullptr )
		, m_defendingEntity( nullptr )
		, m_specificDamageToApply( 0.f )
		, m_attackType( MELEE )
		, m_attackingWeapon( nullptr )
		, m_damageDealt_out( 0.f )
		, m_didDefenderDie_out( false )
	{

	}
};


/////////////////////////////////////////////////////////////////////////////
class CombatSystem
{
public:
	CombatSystem();
	~CombatSystem();

	static void		ApplyAttackAndGetResult( CombatInformation& attackInformation_out );

private:


};

#endif