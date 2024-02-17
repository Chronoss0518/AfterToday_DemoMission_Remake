#pragma once

#include"MechaParts.h"

class GameFrame;
class ExternalFunction;
class WeaponFunction;

class Attack;
class AttackObject;

class MechaPartsObject
{
public:

	friend MechaParts;

public:

	void CreateAnchor();

public:

	void Release();

public:

	ChPtr::Shared<ChCpp::JsonObject> Serialize();

public:

	void AddWeaponFunction(ChPtr::Shared<WeaponFunction> _weapon)
	{
		weaponFunctions.push_back(_weapon);
	}


	inline void AddChildObject(MechaParts::PartsPosNames _typeName, const std::string& _objectType, ChPtr::Shared<MechaPartsObject> _partsObject)
	{
		if (_partsObject == nullptr)return;
		positions[ChStd::EnumCast(_typeName)][_objectType] = _partsObject;
	}

public:

	void SetPositoinObject(MechaPartsObject* _parent, ChPtr::Shared<ChCpp::FrameObject>_positionObject) { positionObject = _positionObject; parentObject = _parent; }

	void SetPartsPosData(unsigned char _names, unsigned long _no) { partsPosName = _names; partsPosNo = _no; }

	void SetRotation(const ChVec3& _rot) { baseRot = _rot; }

	void SetRWeapon(const bool _flg) { weaponType.SetBitFlg(0,_flg); }

	void SetLWeapon(const bool _flg) { weaponType.SetBitFlg(1, _flg); }

	void SetFrame(GameFrame* _frame) { frame = _frame; }

	void SetBaseMecha(BaseMecha* _mecha) { mecha = _mecha; }

	void SetLookAnchorNo(const unsigned long _no) { lookAnchorNo = _no; }

public:

	MechaParts* GetBaseObject() { return baseParts; }

	BaseMecha* GetBaseMecha() { return mecha; }

	ChPtr::Shared<ChCpp::FrameObject> GetPositionObject() { return positionObject; }

	std::vector<ChPtr::Shared<ExternalFunction>>& GetExternalFunctions()
	{
		return externulFunctions;
	}

	std::vector<ChPtr::Shared<WeaponFunction>> GetWeaponFunctions()
	{
		return weaponFunctions;
	}

	ChCpp::Collider* GetCollider()
	{
		return &collider;
	}

	inline ChLMat GetLastDrawMat() { return lastDrawMat; }

	float GetDurableValue() { return durableValue; }

	unsigned char GetPartsPosName() { return partsPosName; }

	unsigned long GetPartsPosNo() { return partsPosNo; }

	bool GetRWeapon() { return weaponType.GetBitFlg(0); }

	bool GetLWeapon() { return weaponType.GetBitFlg(1); }

	float GetDamage(ChCpp::SphereCollider& _sphereCollider, AttackObject& _bullet);

	float GetDamage(ChCpp::BoxCollider& _collider);

	float GetDamage(AttackObject& _bullet);

	ChVec3 GetBaseRotation() { return baseRot; }

	ChVec3 GetColliderSize() { return collider.GetScl(); }

	unsigned long GetLookAnchorNo() { return lookAnchorNo; }

	unsigned long GetUseAttackType() { return useAttackType; }

	std::wstring GetWeaponName();

	std::wstring GetNowBulletNum();

	std::wstring GetReloadCount();

public:

	void Update();

public:

	virtual void Draw(const ChLMat& _drawMat);

public:

	void AttackUpdate();

	void StartWeaponSubFunction();

	void UpUseAttackType()
	{
		useAttackType = (useAttackType + 1) % weaponFunctions.size();
	}

	void DownUseAttackType()
	{
		useAttackType = (useAttackType + weaponFunctions.size() - 1) % weaponFunctions.size();
	}

protected:

	ChCpp::SphereCollider collider;
	MechaParts* baseParts;
	ChLMat lastDrawMat;

private:

	GameFrame* frame = nullptr;

	std::vector<ChPtr::Shared<ExternalFunction>>externulFunctions;
	std::vector<ChPtr::Shared<WeaponFunction>>weaponFunctions;

	std::map<std::string,ChPtr::Shared<MechaPartsObject>> positions[ChStd::EnumCast(MechaParts::PartsPosNames::None)];

	unsigned long useAttackType = 0;

	ChPtr::Shared<ChCpp::FrameObject>positionObject = nullptr;
	MechaPartsObject* parentObject = nullptr;

	ChVec3 baseRot = ChVec3();

	//パーツの解除フラグ//
	bool releaseFlg = false;

	//パーツの耐久地//
	float durableValue = 100.0f;

	unsigned char partsPosName = -1;
	unsigned long partsPosNo = 0;
	ChCpp::BitBool weaponType;

	unsigned long lookAnchorNo = 0xffffffff;

	BaseMecha* mecha = nullptr;
};

class ExternalFunction
{

public:

	virtual void Init(ChD3D11::Shader::BaseDrawMesh11* _drawer, ID3D11Device* _device) {};

	virtual void Release() {};

	inline void SetFrmae(GameFrame* _frame) { frame = _frame; }

	inline void SetBaseMecha(BaseMecha* _mecha) { mecha = _mecha; }

	virtual void SetObjectPos(ChPtr::Shared<ChCpp::FrameObject> _targetObject) = 0;

	virtual void Update() = 0;

	inline void SetPartsObject(MechaPartsObject* _obj)
	{
		if (_obj == nullptr)return;

		obj = _obj;
	}

	virtual void PosUpdate() {};

protected:

	GameFrame* frame = nullptr;

	BaseMecha* mecha = nullptr;

	MechaPartsObject* obj = nullptr;

};
