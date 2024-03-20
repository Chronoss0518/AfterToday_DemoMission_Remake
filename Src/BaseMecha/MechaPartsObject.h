#pragma once

#include"MechaParts.h"

class GameFrame;
class ExternalFunction;
class WeaponFunction;

class Attack;
class AttackObject;

enum class PostureDirection : unsigned char
{
	X,
	Y,
	Z
};

enum class PostrueType : unsigned char
{
	//垂直//
	Vertical,
	//水平//
	Horizontal
};

struct FramePosture
{
	PostureDirection direction;
	PostrueType type;
};

class MechaPartsObject
{
public:

	friend MechaParts;

public:

	void CreateAnchor();

	void CreateFramePosture(ChCpp::FrameObject* _frameObejct);

public:

	void Release();

public:

	ChPtr::Shared<ChCpp::JsonObject> Serialize();

public:

	void AddWeaponFunction(ChPtr::Shared<WeaponFunction> _weapon)
	{
		weaponFunctions.push_back(_weapon);
	}

	inline void AddChildObject(const std::string& _objectType, ChPtr::Shared<MechaPartsObject> _partsObject)
	{
		if (_partsObject == nullptr)return;

		auto&& tmpObject = positions.find(_objectType);
		if (tmpObject == positions.end())
		{
			positions[_objectType] = _partsObject;
			return;
		}

		if (tmpObject->second != nullptr)
		{
			tmpObject->second->Release();
			tmpObject->second = nullptr;
		}

		tmpObject->second = _partsObject;
	}

public:

	inline void RemoveChildObject(const std::string& _objectType)
	{
		auto findObject = positions.find(_objectType);
		if (findObject == positions.end())return;
		
		findObject->second->baseParts->RemoveParameter(*mecha);

		for (auto&& child : findObject->second->positions)
		{
			child.second->RemoveChildObject(child.first);
		}

		positions.erase(findObject);
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

	void SetHitSize();

	void SetPositionObjectRotationYAxis(float _rot);
	
	void SetPositionObjectRotationXAxis(float _rot);

	void SetPositionObjectRotationZAxis(float _rot);

	void SetParentRotationYAxis(unsigned long _no,float _rot);

	void SetParentRotationXAxis(unsigned long _no, float _rot);

	void SetParentRotationZAxis(unsigned long _no, float _rot);

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

	std::wstring GetPartsName();

	std::wstring GetWeaponName();

	std::wstring GetNowBulletNum();

	std::wstring GetReloadCount();

	ChPtr::Shared<MechaPartsObject> GetChildParts(const std::string& _childPosition)
	{
		auto findObject = positions.find(_childPosition);
		if (findObject == positions.end())return nullptr;

		return (*findObject).second;
	}

	MechaPartsObject* GetParent() { return parentObject; }

	ChPtr::Shared<ChCpp::FrameObject> GetPositionObejct() { return positionObject; }

public:

	void Update();

	void UpdateFramePosture(ChCpp::FrameObject* _frameObject);

public:

	virtual void Draw(const ChLMat& _drawMat);

	void  DrawEnd();

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

	//Autoで敵を追尾すると時用に修正//
	std::map<ChCpp::FrameObject*, ChPtr::Shared<FramePosture>> framePostures;

	std::vector<ChPtr::Shared<ExternalFunction>>externulFunctions;
	std::vector<ChPtr::Shared<WeaponFunction>>weaponFunctions;

	std::map<std::string,ChPtr::Shared<MechaPartsObject>> positions;

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
