
#include"../../BaseIncluder.h"

#include"../../AllStruct.h"

#include"WeaponComponent.h"
#include"WeaponComponents.h"

#include"../MechaPartsObject.h"

void WeaponComponent::Update()
{

	if (weapon.empty())return;

	if (IsPushFlg(attack))
	{
		Attack();
		return;
	}

	if (IsPushFlg(weaponUpChange))
	{
		WeaponUpChange();
		return;
	}

	if (IsPushFlg(weaponDownChange))
	{
		WeaponDownChange();
		return;
	}

	if (IsPushFlg(attackTypeUpChange))
	{
		AttackTypeUpChange();
		return;
	}

	if (IsPushFlg(attackTypeDownChange))
	{
		AttackTypeDownChange();
		return;
	}
}

void WeaponComponent::Attack()
{
	weapon[useWeaponNo]->Attack();
}

void WeaponComponent::StartSubFunction()
{
	weapon[useWeaponNo]->StartSubFunction();
}

void WeaponComponent::WeaponUpChange()
{
	useWeaponNo = (useWeaponNo + 1) % weapon.size();
}

void WeaponComponent::WeaponDownChange()
{
	useWeaponNo = (useWeaponNo + weapon.size() - 1) % weapon.size();
}

void WeaponComponent::AttackTypeUpChange()
{
	weapon[useWeaponNo]->UpUseAttackType();
}

void WeaponComponent::AttackTypeDownChange()
{
	weapon[useWeaponNo]->DownUseAttackType();
}
