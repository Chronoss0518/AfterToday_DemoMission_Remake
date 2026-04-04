#pragma once

#include"CPUActionBase.h"

class CPUTargetSelector;

class CPUWeaponSelect :public CPUActionBase
{
public:

public:

	ChPtr::Shared<ChCpp::JsonObject<wchar_t>> Serialize()override;

	void Deserialize(const ChPtr::Shared<ChCpp::JsonObject<wchar_t>>& _jsonObject)override;

	inline void SetAttackType(AttackType _type) { attackType = _type; }

	inline AttackType GetAttackType() { return attackType; }

	inline void SetUnStopFlg(const bool _flg) { unStopFlg = _flg; }

	bool IsUnStopFlg() { return unStopFlg; }

	bool Update(unsigned long _lookTarget, GameFrame& _frame, CPUAttack& _attack, CPUController& _controller);

private:

	AttackType attackType = AttackType::Sword;
	//この命令を行ってもスクリプトを抜けないフラグ//
	bool unStopFlg = false;

};

class CPUWeaponSelector : public FunctionsBase<CPUWeaponSelect>
{
public:

	ChPtr::Shared<ChCpp::JsonObject<wchar_t>> Serialize()override;

	void Deserialize(const ChPtr::Shared<ChCpp::JsonObject<wchar_t>>& _jsonObject)override;

	inline ChPtr::Shared<CPUWeaponSelect> GetSelectWeapon()
	{
		if (selectWeaponsQueue.empty())return nullptr;
		
		ChPtr::Shared<CPUWeaponSelect> res = selectWeaponsQueue[0];
		selectWeaponsQueue.erase(selectWeaponsQueue.begin());

		return res;
	}

	bool IsRunEnd() { return selectWeaponsQueue.empty(); }

	void Update(CPUTargetSelector& _targetSelector, GameFrame& _frame, CPUAttack& _attack,CPUController& _controller);

	void ActionCansel() { selectWeaponsQueue.clear(); }

private:

	unsigned long beforeTarget = -1;

	std::vector<ChPtr::Shared<CPUWeaponSelect>>selectWeaponsQueue;

};