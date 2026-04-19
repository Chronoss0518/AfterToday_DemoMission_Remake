
#include"../../BaseIncluder.h"

#include"../../AllStruct.h"

#include"WeaponComponent.h"

#include"../MechaPartsObjectFunction/WeaponFunction.h"
#include"../MechaPartsObject.h"

void WeaponComponent::Update()
{
	for (int i = 0; i < PALETTE_COUNT; i++)
	{
		auto weapon = weapons[ChStd::EnumCast(WeaponHandType::Left)][i];
		if (weapon != nullptr)
			weapon->Update();

		weapon = weapons[ChStd::EnumCast(WeaponHandType::Right)][i];
		if (weapon != nullptr)
			weapon->Update();
	}

	auto weapon = weapons[ChStd::EnumCast(WeaponHandType::Left)][useWeaponNo[ChStd::EnumCast(WeaponHandType::Left)]];
	if (weapon != nullptr)
		weapon->SelectedUpdate();

	weapon = weapons[ChStd::EnumCast(WeaponHandType::Right)][useWeaponNo[ChStd::EnumCast(WeaponHandType::Right)]];
	if (weapon != nullptr)
		weapon->SelectedUpdate();

	UpdateFunctions(
		WeaponHandType::Left,
		InputName::LWeaponAction,
		InputName::LAttack,
		InputName::LWSubFunction,
		InputName::LWUChange,
		InputName::LWDChange);

	UpdateFunctions(
		WeaponHandType::Right,
		InputName::RWeaponAction,
		InputName::RAttack,
		InputName::RWSubFunction,
		InputName::RWUChange,
		InputName::RWDChange);
}

int WeaponComponent::GetWeaponCount(ChPtr::Shared<WeaponFunction>& _func, WeaponHandType _type)
{
	if (_func == nullptr)return NOT_WEAPON_REGIST_NUM;

	for (int i = 0; i < PALETTE_COUNT; i++)
	{
		if (weapons[ChStd::EnumCast(_type)][i] == nullptr)continue;
		if (weapons[ChStd::EnumCast(_type)][i].get() != _func.get())continue;

		return i;
	}

	return NOT_WEAPON_REGIST_NUM;
}

ChPtr::Shared<WeaponFunction> WeaponComponent::GetWeaponFunction(WeaponHandType _type, unsigned char _num)
{
	if (_num < 0 || _num >= PALETTE_COUNT)return nullptr;
	return weapons[ChStd::EnumCast(_type)][_num];
}

std::wstring WeaponComponent::GetPartsName(WeaponHandType _type)
{
	auto&& weapon = weapons[ChStd::EnumCast(_type)][useWeaponNo[ChStd::EnumCast(_type)]];
	if (weapon == nullptr)return WeaponFunction::GetDefaultPartsName();
	return weapon->GetPartsName();
}

std::wstring WeaponComponent::GetWeaponName(WeaponHandType _type)
{
	auto&& weapon = weapons[ChStd::EnumCast(_type)][useWeaponNo[ChStd::EnumCast(_type)]];
	if (weapon == nullptr)return WeaponFunction::GetDefaultWeaponName();
	return weapon->GetWeaponName();
}

std::wstring WeaponComponent::GetNowBulletNum(WeaponHandType _type)
{
	auto&& weapon = weapons[ChStd::EnumCast(_type)][useWeaponNo[ChStd::EnumCast(_type)]];
	if (weapon == nullptr)return WeaponFunction::GetDefaultBulletNum();
	return weapon->GetBulletNum();
}

std::wstring WeaponComponent::GetReloadCount(WeaponHandType _type)
{
	auto&& weapon = weapons[ChStd::EnumCast(_type)][useWeaponNo[ChStd::EnumCast(_type)]];
	if (weapon == nullptr)return WeaponFunction::GetDefaultReloadCount();
	return weapon->GetReloadCount();
}

void WeaponComponent::Attack(WeaponHandType _type)
{
	auto&& weapon = weapons[ChStd::EnumCast(_type)][useWeaponNo[ChStd::EnumCast(_type)]];
	if (weapon == nullptr)return;
	weapon->AttackUpdate();
}

void WeaponComponent::StartSubFunction(WeaponHandType _type)
{
	auto&& weapon = weapons[ChStd::EnumCast(_type)][useWeaponNo[ChStd::EnumCast(_type)]];
	if (weapon == nullptr)return;
	weapon->StartSubFunction();
}

void WeaponComponent::WeaponUpChange(WeaponHandType _type)
{
	useWeaponNo[ChStd::EnumCast(_type)] = (useWeaponNo[ChStd::EnumCast(_type)] + 1) % PALETTE_COUNT;
}

void WeaponComponent::WeaponDownChange(WeaponHandType _type)
{
	useWeaponNo[ChStd::EnumCast(_type)] = (useWeaponNo[ChStd::EnumCast(_type)] + PALETTE_COUNT - 1) % PALETTE_COUNT;
}


void WeaponComponent::UpdateFunctions(
	WeaponHandType _type,
	InputName _weaponAction,
	InputName _attackInput,
	InputName _subFunctionInput,
	InputName _weaponUpChangeInput,
	InputName _weaponDownChangeInput)
{
	if (IsPushFlg(_weaponAction))
	{
		bool isRunning = false;

		if (IsPushFlg(InputName::Attack))
		{
			isRunning = true;
			SetPushFlg(_attackInput);
		}

		if (IsPushFlg(InputName::WeaponSubFunction))
		{
			isRunning = true;
			SetPushFlg(_subFunctionInput);
		}

		if (IsPushFlg(InputName::WeaponUpChange))
		{
			isRunning = true;
			SetPushFlg(_weaponUpChangeInput);
		}

		if (IsPushFlg(InputName::WeaponDownChange))
		{
			isRunning = true;
			SetPushFlg(_weaponDownChangeInput);
		}

		if (!isRunning)SetPushFlg(_attackInput);

	}

	if (IsPushFlg(_attackInput))
		Attack(_type);

	if (IsPushFlg(_subFunctionInput))
		StartSubFunction(_type);

	if (IsPushFlg(_weaponUpChangeInput))
		WeaponUpChange(_type);

	if (IsPushFlg(_weaponDownChangeInput))
		WeaponDownChange(_type);
}
