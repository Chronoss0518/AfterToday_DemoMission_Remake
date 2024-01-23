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

	//次の攻撃可能までの時間//
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

	//持ち手部分と刀身の区切り部分//
	inline void SetObjectPos(ChPtr::Shared<ChCpp::FrameObject> _hitStart) { hitObjectStart = _hitStart; }

private:

	void SetData(WeaponData* _data)override;

private:

	SwordData* swordData = nullptr;

	ChPtr::Shared<ChCpp::FrameObject>hitObjectStart = nullptr;

	//攻撃開始から現在までの時間//
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

	//弾が出てくる場所//
	inline void SetObjectPos(ChPtr::Shared<ChCpp::FrameObject> _shotPos) { shotPos = _shotPos; }

private:

	void SetData(WeaponData* _data)override;

private:

	GunData* gunData = nullptr;

	ChPtr::Shared<ChCpp::FrameObject>shotPos = nullptr;

	ChLMat lastShotPos;

	bool reloadFlg = false;

	//残りの弾数//
	unsigned long nowBulletNum = 0;
	//残りのマガジン数//
	unsigned long nowMagazineNum = 0;
	//残りのリロード時間//
	unsigned long nowReloadTime = 0;

};
