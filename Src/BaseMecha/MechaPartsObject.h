#pragma once

class GameFrame;
class BaseMecha;
class MechaParts;
class WeaponFunction;
class WeaponData;
class SwordData;
class GunData;

class BulletData;
struct PositionObject;

class MechaPartsObject
{
public:

	friend MechaParts;

public:

	void AddWeaponFunction(ChPtr::Shared<WeaponFunction> _weapon)
	{
		weaponFunc.push_back(_weapon);
	}

public:

	void SetPositoinObject(ChPtr::Shared<PositionObject>_positionObject) { positionObject = _positionObject; }

	void SetPartsPosData(unsigned char _names, unsigned long _no) { partsPosName = _names; partsPosNo = _no; }

	void SetRotation(const ChVec3& _rot) { baseRot = _rot; }

	void SetRWeapon(const bool _flg) { weaponType.SetBitFlg(0,_flg); }

	void SetLWeapon(const bool _flg) { weaponType.SetBitFlg(1, _flg); }

	void SetSwordHitObjectPos(ChPtr::Shared<ChCpp::FrameObject> _targetObject);

	void SetGunShotPos(ChPtr::Shared<ChCpp::FrameObject> _targetObject);

	void SetFrame(GameFrame* _frame) { frame = _frame; }

	void SetBaseMecha(BaseMecha* _mecha) { mecha = _mecha; }

public:

	MechaParts* GetBaseObject() { return baseParts; }

	ChPtr::Shared<PositionObject> GetPositionObject() { return positionObject; }

	short GetDurableValue() { return durableValue; }

	unsigned char GetPartsPosName() { return partsPosName; }

	unsigned long GetPartsPosNo() { return partsPosNo; }

	bool GetRWeapon() { return weaponType.GetBitFlg(0); }

	bool GetLWeapon() { return weaponType.GetBitFlg(1); }

public:

	void Update();

public:

	virtual void Draw(const ChLMat& _drawMat);

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

	GameFrame* frame = nullptr;

	std::vector<ChPtr::Shared<WeaponFunction>>weaponFunc;
	unsigned long useAttackType = 0;

	ChPtr::Shared<PositionObject>positionObject = nullptr;
	ChVec3 baseRot = ChVec3();

	//パーツの解除フラグ//
	ChStd::Bool releaseFlg = false;

	//パーツの耐久地//
	short durableValue = 10000;

	unsigned char partsPosName = -1;
	unsigned long partsPosNo = 0;
	ChCpp::BitBool weaponType;
	 
	BaseMecha* mecha = nullptr;
};

class WeaponFunction
{
public:

	virtual void Init(MeshDrawer* _drawer) = 0;

	inline void SetFrmae(GameFrame* _frame) { frame = _frame; }

	void Attack();

	virtual void SubFunction() {}

	virtual void Update() {}

	inline void SetPartsObject(MechaPartsObject* _obj)
	{
		if (_obj == nullptr)return;

		obj = _obj;
	}

	inline void SetBaseData(WeaponData* _data)
	{
		if (_data == nullptr)return;

		data = _data;

		SetData(_data);
	}

	virtual void PosUpdate() {};

protected:

	virtual void AttackFunction() = 0;

	virtual void SetData(WeaponData* _data) = 0;

protected:

	GameFrame* frame = nullptr;

	BaseMecha* mecha = nullptr;

	WeaponData* data = nullptr;

	MechaPartsObject* obj = nullptr;


	//次の攻撃可能までの時間//
	unsigned long nowWeatTime = 0;

	ChStd::Bool attackFlg = false;
};

class SwordFunction : public WeaponFunction
{
public:

	void Init(MeshDrawer* _drawer)override;

	void AttackFunction()override;

	inline void SetHitObjectStart(ChPtr::Shared<ChCpp::FrameObject> _hitStart) { hitObjectStart = _hitStart; }

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

	void Init(MeshDrawer* _drawer)override;

	void AttackFunction()override;

	void SubFunction()override;

	void Update()override;

	void PosUpdate()override;

	inline void SetShotPos(ChPtr::Shared<ChCpp::FrameObject> _shotPos) { shotPos = _shotPos; }

private:
	
	void SetData(WeaponData* _data)override;

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
