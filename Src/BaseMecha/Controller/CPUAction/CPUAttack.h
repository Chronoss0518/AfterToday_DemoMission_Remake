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

	ChPtr::Shared<ChCpp::JsonObject> Serialize();

	void Deserialize(const ChPtr::Shared<ChCpp::JsonObject>& _jsonObject);

	void SetRightWeapons(const ChPtr::Shared<RightWeaponComponent>& _weapons);

	void SetLeftWeapons(const ChPtr::Shared<LeftWeaponComponent>& _weapons);

	inline void SetCenterLength(const float _len) {if(_len > 0.0f) centerLength = _len; }

	inline float GetCenterLength() { return centerLength; }

	inline void SetAttackCount(const unsigned long _count) { attackCount = _count; }

	inline unsigned long GetAttackCount() { return attackCount; }

	inline void SetHaveWeaponType(PriorityWeaponType _type) { priorityWeaponType = _type; }

	inline PriorityWeaponType GetHaveWeaponType() { return priorityWeaponType; }

	inline void SetAttackType(const ChCpp::BitBool& _type) { attackType = _type; }

	inline ChCpp::BitBool GetAttackType() { return attackType; }

	void Update(
		CPUWeaponSelector& _weaponSelector, 
		CPUTargetSelector& _targetSelector,
		CPUController& _controller, 
		GameFrame& _frame);

	void AttackCancel()
	{
		selectWeapon = nullptr;
		leftCommandList.clear();
		rightCommandList.clear();
	}

	bool IsHaveAttackType(AttackType _attackType) { return attackType.GetBitFlg(ChStd::EnumCast(_attackType)); }

private:

	void SetAttackTypeFlgs(WeaponComponent& _weaponFunction);

	void FindRightPriorityAttack();

	void FindLeftPriorityAttack();

	void FindAttackFunction(
		WeaponComponent& _weaponFunction,
		std::vector<ControllerBase::InputName>& _commandList,
		ControllerBase::InputName _attack,
		ControllerBase::InputName _weaponUpChange,
		ControllerBase::InputName _weaponDownChange,
		ControllerBase::InputName _typeUpChange,
		ControllerBase::InputName _typeDownChange);

	void CreateAttackCommand(
		unsigned long maxSize,
		unsigned long nowSelectCount,
		unsigned long targetSelectCount,
		std::vector<ControllerBase::InputName>& _commandList,
		ControllerBase::InputName _upChange,
		ControllerBase::InputName _downChange);

	void AttackFunction(CPUController& _controller,
		std::vector<ControllerBase::InputName>& _commandList);

	ChPtr::Shared<RightWeaponComponent>rightWeaponFunctions;

	ChPtr::Shared<LeftWeaponComponent>leftWeaponFunctions;

	ChCpp::BitBool attackType;
	ChPtr::Shared<CPUWeaponSelect> selectWeapon = nullptr;

	std::vector<ControllerBase::InputName>leftCommandList;

	std::vector<ControllerBase::InputName>rightCommandList;

	PriorityWeaponType priorityWeaponType = PriorityWeaponType::Right;

	float centerLength = 1.0f;

	unsigned long attackCount = 1;



};