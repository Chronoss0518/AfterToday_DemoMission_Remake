
#include"../../BaseIncluder.h"

#include"../../AllStruct.h"

#include"WeaponComponent.h"
#include"WeaponComponents.h"

#include"../MechaPartsObjectFunction/WeaponFunction.h"
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

std::vector<ChPtr::Weak<WeaponFunction>> WeaponComponent::GetRegistWeaponList()
{
	std::vector<ChPtr::Weak<WeaponFunction>> res;

	for (auto&& weaponParts : weapon)
	{
		for (auto&& weap : weaponParts->GetWeaponFunctions())
		{
			res.push_back(weap);
		}
	}

	return res;
}

std::vector<ChPtr::Weak<MechaPartsObject>> WeaponComponent::GetWeaponMechaPartsList()
{
	std::vector<ChPtr::Weak<MechaPartsObject>> res;

	for (auto&& weaponParts : weapon)
	{
		res.push_back(weaponParts);
	}

	return res;
}


void WeaponComponent::Attack()
{
	weapon[useWeaponNo]->AttackUpdate();
}

void WeaponComponent::StartSubFunction()
{
	weapon[useWeaponNo]->StartWeaponSubFunction();
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
