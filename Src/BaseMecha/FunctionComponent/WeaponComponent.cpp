
#include"../../BaseIncluder.h"

#include"../../AllStruct.h"

#include"WeaponComponent.h"

#include"../MechaPartsObjectFunction/WeaponFunction.h"
#include"../MechaPartsObject.h"

#define	NOT_REGIST_NUM -1

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

	if (IsPushFlg(InputName::LAttack))
		Attack(WeaponHandType::Left);

	if (IsPushFlg(InputName::LWUChange))
		WeaponUpChange(WeaponHandType::Left);

	if (IsPushFlg(InputName::LWDChange))
		WeaponDownChange(WeaponHandType::Left);


	if (IsPushFlg(InputName::RAttack))
		Attack(WeaponHandType::Right);

	if (IsPushFlg(InputName::RWUChange))
		WeaponUpChange(WeaponHandType::Right);

	if (IsPushFlg(InputName::RWDChange))
		WeaponDownChange(WeaponHandType::Right);
}

int WeaponComponent::GetWeaponCount(ChPtr::Shared<WeaponFunction>& _func, WeaponHandType _type)
{
	if (_func == nullptr)return NOT_REGIST_NUM;

	for (int i = 0; i < PALETTE_COUNT; i++)
	{
		if (weapons[ChStd::EnumCast(_type)][i] == nullptr)continue;
		if (weapons[ChStd::EnumCast(_type)][i].get() != _func.get())continue;

		return i;
	}

	return NOT_REGIST_NUM;
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
