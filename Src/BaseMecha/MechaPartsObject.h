#pragma once

class MechaParts;
class WeaponFunction;
class WeaponData;
class SwordData;
class GunData;

class MechaPartsObject:public ChCpp::BaseObject
{
public:

	friend MechaParts;

public:

	void SetPositoinObject(ChPtr::Shared<ChCpp::FrameObject>_positionObject) { positionObject = _positionObject; }

	void SetPartsPosData(unsigned char _names, unsigned long _no) { partsPosName = _names; partsPosNo = _no; }

	void SetWeaponType(unsigned char _type) { weaponType = _type; }

public:

	MechaParts* GetBaseObject() { return baseParts; }

	ChPtr::Shared<ChCpp::FrameObject> GetPositionObject() { return positionObject; }

	short GetDurableValue() { return durableValue; }

	unsigned char GetPartsPosName() { return partsPosName; }

	unsigned long GetPartsPosNo() { return partsPosNo; }

	unsigned char GetWeaponType() { return weaponType; }

public:

	void Update();

public:

	virtual void Draw(MeshDrawer& _meshDrawer,const ChLMat& _drawMat);

public:

	void Damage();

public:

	void Attack();

	void StartSubFunction();

	void UpUseAttackType() { useAttackType = (1 + useAttackType) % weaponFunc.size(); }

	void DownUseAttackType() { useAttackType = (useAttackType + weaponFunc.size() - 1) % weaponFunc.size(); }

protected:

	MechaParts* baseParts;

	ChLMat lastDrawMat;

private:

	std::vector<ChPtr::Shared<WeaponFunction>>weaponFunc;
	unsigned long useAttackType = 0;

	ChPtr::Shared<ChCpp::FrameObject>positionObject = nullptr;

	//�p�[�c�̉����t���O//
	ChStd::Bool releaseFlg = false;

	//�p�[�c�̑ϋv�n//
	short durableValue = 10000;

	unsigned char partsPosName = -1;
	unsigned long partsPosNo = 0;
	unsigned char weaponType = -1;
	 
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

	//���̍U���\�܂ł̎���//
	unsigned long nowWeatTime = 0;

	ChStd::Bool attackFlg = false;
};

class SwordFunction : public WeaponFunction
{
public:

	void AttackFunction()override;

private:

	void SetData(WeaponData* _data)override;

	void Init()override;

private:

	SwordData* swordData = nullptr;

	//�U���J�n���猻�݂܂ł̎���//
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

	//�c��̒e��//
	unsigned long nowBulletNum = 0;
	//�c��̃}�K�W����//
	unsigned long nowMagazineNum = 0;
	//�c��̃����[�h����//
	unsigned long nowReloadTime = 0;


};

class BulletObject :public ChCpp::BaseObject
{
public:



};