#pragma once


#include"../MechaPartsObject.h"


class WeaponData;
class SwordData;
class GunData;

class WeaponFunction :public ExternalFunction
{
public:

	virtual void Release()override;

	virtual void AttackUpdate();

	virtual void Update()override;

	inline void SetBaseData(WeaponData* _data)
	{
		if (_data == nullptr)return;

		data = _data;

		SetData(_data);
	}

	inline ChPtr::Shared<Attack> GetAttackData()
	{
		return attackData;
	}

public:

	virtual void StartSubFunction() {};

	virtual void AttackFunction() {};

	virtual void UpdateFunction() {};

	virtual void SetData(WeaponData* _data) = 0;

protected:


	WeaponData* data = nullptr;

	ChD3D::X3DAudioObject se;

	//���̍U���\�܂ł̎���//
	unsigned long nowWeatTime = 0;

	ChPtr::Shared<Attack>attackData = nullptr;

	bool attackFlg = false;
};

class SwordFunction : public WeaponFunction
{
public:

	void Init(ChD3D11::Shader::BaseDrawMesh11* _drawer, ID3D11Device* _device)override;

	void AttackFunction()override;

	void UpdateFunction()override {};

	//�����蕔���Ɠ��g�̋�؂蕔��//
	inline void SetObjectPos(ChPtr::Shared<ChCpp::FrameObject> _hitStart) { hitObjectStart = _hitStart; }

private:

	void SetData(WeaponData* _data)override;

private:

	SwordData* swordData = nullptr;

	ChPtr::Shared<ChCpp::FrameObject>hitObjectStart = nullptr;

	//�U���J�n���猻�݂܂ł̎���//
	unsigned long nowAttackTime = 0;


};

class GunFunction : public WeaponFunction
{
public:

	void Init(ChD3D11::Shader::BaseDrawMesh11* _drawer, ID3D11Device* _device)override;

	void AttackFunction()override;

	void StartSubFunction();

	void UpdateFunction()override;

	void PosUpdate()override;

	//�e���o�Ă���ꏊ//
	inline void SetObjectPos(ChPtr::Shared<ChCpp::FrameObject> _shotPos) { shotPos = _shotPos; }

private:

	void SetData(WeaponData* _data)override;

private:

	GunData* gunData = nullptr;

	ChPtr::Shared<ChCpp::FrameObject>shotPos = nullptr;

	ChLMat lastShotPos;

	bool reloadFlg = false;

	//�c��̒e��//
	unsigned long nowBulletNum = 0;
	//�c��̃}�K�W����//
	unsigned long nowMagazineNum = 0;
	//�c��̃����[�h����//
	unsigned long nowReloadTime = 0;

};
