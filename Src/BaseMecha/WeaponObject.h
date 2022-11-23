#pragma once

class WeaponParts;
class WeaponFunction;
class WeaponData;
class SwordData;
class GunData;

#include"MechaPartsObject.h"

class WeaponObject:public MechaPartsObject
{
public:
	friend WeaponParts;

	void Init()override;

	void ChangeAttackType();

	void Attack();

	void SubFunction();

	void Update();

private:

	std::vector<ChPtr::Shared<WeaponFunction>>weaponFunc;

	unsigned long useAttackType = 0;
};

class WeaponFunction
{
public:

	void Attack();

	virtual void SubFunction() {}

	virtual void Update() {}

	void SetBaseData(WeaponData* _data);

protected:

	virtual void AttackFunction() = 0;

	virtual void SetData(WeaponData* _data) = 0;

	virtual void Init() = 0;

protected:

	WeaponData* data = nullptr;

	//次の攻撃可能までの時間//
	unsigned long nowWeatTime = 0;

	ChStd::Bool attackFlg = false;
};

class SwordFunction: public WeaponFunction
{
public:

	void AttackFunction()override;

private:

	void SetData(WeaponData* _data)override;

	void Init()override;

private:

	SwordData* swordData = nullptr;

	//攻撃開始から現在までの時間//
	unsigned long nowAttackTime = 0;


};

class GunFunction : public WeaponFunction
{
public:

	void AttackFunction()override;

	void SubFunction()override;

	void Update()override;

private:

	void SetData(WeaponData* _data)override;

	void Init()override;

private:

	GunData* gunData = nullptr;

	ChPtr::Shared<ChCpp::FrameObject>shotPos = nullptr;

	ChPtr::Shared<BulletData>createBulletData = nullptr;

	ChLMat lastShotPos;

	ChStd::Bool reloadFlg = false;

	//残りの弾数//
	unsigned long nowBulletNum = 0;
	//残りのマガジン数//
	unsigned long nowMagazineNum = 0;
	//残りのリロード時間//
	unsigned long nowReloadTime = 0;


};

class BulletObject:public ChCpp::BaseObject
{
public:



};