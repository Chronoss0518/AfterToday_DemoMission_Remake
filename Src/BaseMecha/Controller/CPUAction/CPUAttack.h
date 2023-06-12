#pragma once

#include"../ControllerBase.h"

class CPUWeaponSelector;
class CPUWeaponSelect;
class CPUTargetSelector;

class CPUAttack
{
public:

	enum class PriorityWeaponType
	{
		Right,
		Left
	};

public:

	std::string Serialize();

	void Deserialize(const std::string& _text);

	void SetRightWeapons(const ChPtr::Shared<RightWeaponComponent>& _weapons);

	void SetLeftWeapons(const ChPtr::Shared<LeftWeaponComponent>& _weapons);

	inline void SetCenterLength(const float _len) { centerLength = _len; }

	inline float GetCenterLength() { return centerLength; }

	inline void SetHaveWeaponType(PriorityWeaponType _type) { priorityWeaponType = _type; }

	inline PriorityWeaponType GetHaveWeaponType() { return priorityWeaponType; }

	void Update(
		CPUWeaponSelector& _weaponSelector, 
		CPUTargetSelector& _targetSelector,
		CPUController& _controller, 
		GameFrame& _frame);

	void AttackCancel()
	{
		selectWeapon = nullptr;
		commandList.clear();
	}

	bool IsHaveAttackType(AttackType _attackType) { return attackType.GetBitFlg(ChStd::EnumCast(_attackType)); }

private:

	void SetAttackTypeFlgs(WeaponComponent& _weaponFunction);

	void FindRightPriorityAttack();

	void FindLeftPriorityAttack();

	void FindAttackFunction(
		WeaponComponent& _weaponFunction,
		ControllerBase::InputName _attack,
		ControllerBase::InputName _weaponUpChange,
		ControllerBase::InputName _weaponDownChange,
		ControllerBase::InputName _typeUpChange,
		ControllerBase::InputName _typeDownChange);

	void CreateAttackCommand(
		unsigned long maxSize,
		unsigned long nowSelectCount,
		unsigned long targetSelectCount,
		ControllerBase::InputName _upChange,
		ControllerBase::InputName _downChange);

	void AttackFunction(CPUController& _controller);

	ChPtr::Shared<RightWeaponComponent>rightWeaponFunctions;

	ChPtr::Shared<LeftWeaponComponent>leftWeaponFunctions;

	ChCpp::BitBool attackType;
	ChPtr::Shared<CPUWeaponSelect> selectWeapon = nullptr;

	std::vector<ControllerBase::InputName>commandList;

	PriorityWeaponType priorityWeaponType = PriorityWeaponType::Right;

	float centerLength = 1.0f;

	unsigned long attackCount = 1;



};