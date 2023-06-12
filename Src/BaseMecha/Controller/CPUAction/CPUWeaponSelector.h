#pragma once

#include"CPUActionBase.h"

class CPUTargetSelector;

class CPUWeaponSelect :public CPUActionBase
{
public:

public:

	std::string Serialize()override;

	void Deserialize(const std::string& _text)override;

	inline void SetAttackType(AttackType _type) { attackType = _type; }

	inline AttackType GetAttackType() { return attackType; }

	inline void SetUnStopFlg(const bool _flg) { unStopFlg = _flg; }

	bool IsUnStopFlg() { return unStopFlg; }

	bool Update(unsigned long _lookTarget, GameFrame& _frame, CPUAttack& _attack);

private:

	std::string GetValue(unsigned char _no);

	enum class SerializeNo :unsigned char
	{
		ActiveFlg,//実行するしないの判定//
		CenterLength,//中心点からどの程度離れているかの確認距離//
		MemberType,//対象とする陣営の判定//
		DistanceType,//距離の比較種類//
		TestDistance,//比較対象の距離//
		DistanceComparison,//距離判定で利用する比較演算子//
		DamageType,//ダメージの比較種類//
		TestDamage,//比較対象のダメージ//
		DamageComparison,//ダメージ判定で利用する比較演算子//
		AttackType,//利用する武器の種類//
		UnStop,//ここを通過してもテストを実行するフラグ//
		None//終了文字
	};

	AttackType attackType = AttackType::Sword;
	//この命令を行ってもスクリプトを抜けないフラグ//
	bool unStopFlg = false;

};

class CPUWeaponSelector : public FunctionsBase<CPUWeaponSelect>
{
public:

	std::string Serialize()override;

	void Deserialize(const std::string& _text)override;

	inline ChPtr::Shared<CPUWeaponSelect> GetSelectWeapon()
	{
		if (selectWeaponsQueue.empty())return nullptr;
		
		ChPtr::Shared<CPUWeaponSelect> res = selectWeaponsQueue[0];
		selectWeaponsQueue.erase(selectWeaponsQueue.begin());

		return res;
	}

	bool IsRunEnd() { return selectWeaponsQueue.empty(); }

	void Update(CPUTargetSelector& _targetSelector, GameFrame& _frame, CPUAttack& _attack);

	void ActionCansel() { selectWeaponsQueue.clear(); }

private:

	unsigned long beforeTarget = -1;

	std::vector<ChPtr::Shared<CPUWeaponSelect>>selectWeaponsQueue;

};