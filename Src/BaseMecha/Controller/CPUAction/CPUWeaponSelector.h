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
		ActiveFlg,//���s���邵�Ȃ��̔���//
		CenterLength,//���S�_����ǂ̒��x����Ă��邩�̊m�F����//
		MemberType,//�ΏۂƂ���w�c�̔���//
		DistanceType,//�����̔�r���//
		TestDistance,//��r�Ώۂ̋���//
		DistanceComparison,//��������ŗ��p�����r���Z�q//
		DamageType,//�_���[�W�̔�r���//
		TestDamage,//��r�Ώۂ̃_���[�W//
		DamageComparison,//�_���[�W����ŗ��p�����r���Z�q//
		AttackType,//���p���镐��̎��//
		UnStop,//������ʉ߂��Ă��e�X�g�����s����t���O//
		None//�I������
	};

	AttackType attackType = AttackType::Sword;
	//���̖��߂��s���Ă��X�N���v�g�𔲂��Ȃ��t���O//
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