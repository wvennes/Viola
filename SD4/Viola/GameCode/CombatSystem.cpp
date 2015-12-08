//---------------------------------------------------------------------------
// CombatSystem.cpp
//---------------------------------------------------------------------------

#include "GameEntity.hpp"
#include "Item.hpp"
#include "CombatSystem.hpp"
#define STATIC


//---------------------------------------------------------------------------
CombatSystem::CombatSystem()
{

}


//---------------------------------------------------------------------------
CombatSystem::~CombatSystem()
{

}


//---------------------------------------------------------------------------
STATIC void CombatSystem::ApplyAttackAndGetResult( CombatInformation& attackInformation_out )
{
	GameEntity& attacker = *attackInformation_out.m_attackingEntity;
	GameEntity& defender = *attackInformation_out.m_defendingEntity;

	// bypass regular combat system if we have a special attack type
	if ( attackInformation_out.m_specificDamageToApply < 0.f ) // this is a heal!
	{
		float healToApply = attackInformation_out.m_specificDamageToApply;
		if ( attackInformation_out.m_attackingWeapon != nullptr && attackInformation_out.m_attackingWeapon->GetItemAttack() < 0 ) // healing weapon
		{
			healToApply += attackInformation_out.m_attackingWeapon->GetItemAttack();
			attackInformation_out.m_attackingWeapon->UseItem();
		}

		attackInformation_out.m_damageDealt_out = defender.ApplyDamage( healToApply );

		return;
	}
	else if ( attackInformation_out.m_specificDamageToApply > 0.f )
	{
		defender.ApplyDamage( attackInformation_out.m_specificDamageToApply );
		return;
	}

	float damageToApply = attacker.GetBaseAttackDamage();

	if ( attackInformation_out.m_attackingWeapon != nullptr )
	{
		damageToApply += attackInformation_out.m_attackingWeapon->GetItemAttack();
		attackInformation_out.m_attackingWeapon->UseItem();
	}

	attackInformation_out.m_damageDealt_out = defender.ApplyDamage( damageToApply );
	attackInformation_out.m_didDefenderDie_out = defender.IsDead();
}