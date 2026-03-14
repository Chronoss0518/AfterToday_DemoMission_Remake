#pragma once

#include"MechaParts.h"

class GameFrame;
class ExternalFunction;
class WeaponFunction;
class MechaPartsObject;

class Attack;
class AttackObject;

enum class StartRotatePosture
{
	XY,
	YX,
	None
};

class MechaPartsObject:public ChCpp::TransformObject<wchar_t>
{
public:

	friend MechaParts;

public:

	void CreateAnchor();

public:

	void Release();

public:

	ChPtr::Shared<ChCpp::JsonObject<wchar_t>> Serialize();

public:

	void AddWeaponFunction(ChPtr::Shared<WeaponFunction> _weapon)
	{
		weaponFunctions.push_back(_weapon);
	}

	void AddExternalFunction(ChPtr::Shared<ExternalFunction> _function)
	{
		externalFunctions.push_back(_function);
	}

	void AddChildObject(const std::wstring& _objectType, ChPtr::Shared<MechaPartsObject> _partsObject);

public:

	inline void RemoveChildObject(const std::wstring& _objectType)
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

	void SetPartsPosData(unsigned char _names, size_t _no) { partsPosName = _names; partsPosNo = _no; }

	void SetRWeapon(const bool _flg) { weaponType.SetBitFlg(0,_flg); }

	void SetLWeapon(const bool _flg) { weaponType.SetBitFlg(1, _flg); }

	void SetFrame(GameFrame* _frame) { frame = _frame; }

	void SetBaseMecha(BaseMecha* _mecha) { mecha = _mecha; }

	void SetLookAnchorNo(const size_t _no) { lookAnchorNo = _no; }

	void SetHitSize();

public:

	inline ChPtr::Shared<MechaPartsObject>GetChildParts(const std::wstring& _nextPosName)
	{
		auto pos = positions.find(_nextPosName);
		if (pos == positions.end())return nullptr;
		return pos->second;
	}

	MechaParts* GetBaseObject() { return baseParts; }

	BaseMecha* GetBaseMecha() { return mecha; }

	std::vector<ChPtr::Shared<ExternalFunction>>& GetExternalFunctions()
	{
		return externalFunctions;
	}

	std::vector<ChPtr::Shared<WeaponFunction>> GetWeaponFunctions()
	{
		return weaponFunctions;
	}

	ChCpp::Collider* GetCollider()
	{
		return &collider;
	}

	float GetDurableValue() { return durableValue; }

	unsigned char GetPartsPosName() { return partsPosName; }

	size_t GetPartsPosNo() { return partsPosNo; }

	bool GetRWeapon() { return weaponType.GetBitFlg(0); }

	bool GetLWeapon() { return weaponType.GetBitFlg(1); }

	float GetDamage(ChCpp::SphereCollider& _sphereCollider, AttackObject& _bullet);

	float GetDamage(ChCpp::BoxCollider& _collider);

	float GetDamage(AttackObject& _bullet);

	ChVec3 GetColliderSize() { return collider.GetScl(); }

	size_t GetLookAnchorNo() { return lookAnchorNo; }

	size_t GetUseAttackType() { return useAttackType; }

	std::wstring GetPartsName();

	std::wstring GetWeaponName();

	std::wstring GetNowBulletNum();

	std::wstring GetReloadCount();

public:

	void Update()override;

public:

	void DrawBegin()override;

	void Draw3D()override;

	void DrawEnd();

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
	ChLMat positionLastDrawMat;

private:

	GameFrame* frame = nullptr;

	StartRotatePosture startRotatePosture = StartRotatePosture::None;
	bool isInitRunFlg = false;

	std::vector<ChPtr::Shared<ExternalFunction>>externalFunctions;
	std::vector<ChPtr::Shared<WeaponFunction>>weaponFunctions;

	std::map<std::wstring,ChPtr::Shared<MechaPartsObject>> positions;

	size_t useAttackType = 0;

	//パーツの解除フラグ//
	bool releaseFlg = false;

	//パーツの耐久地//
	float durableValue = 100.0f;

	unsigned char partsPosName = -1;
	size_t partsPosNo = 0;

	ChCpp::BitBool weaponType;

	size_t lookAnchorNo = -1;

	BaseMecha* mecha = nullptr;
};

class ExternalFunction
{

public:

	virtual void Init(ChD3D11::Shader::BaseDrawMesh11<wchar_t>* _drawer, ID3D11Device* _device) {};

	virtual void Release() {};

	inline void SetFrmae(GameFrame* _frame) { frame = _frame; }

	inline void SetBaseMecha(BaseMecha* _mecha) { mecha = _mecha; }

	virtual void Update() = 0;

	virtual void DrawBegin() {};

	virtual void DrawEnd() {};

	inline void SetPartsObject(MechaPartsObject* _obj)
	{
		if (_obj == nullptr)return;
		obj = _obj;
	}

	virtual inline ChVec3 GetTargetDirection() { return ChVec3(0.0f,0.0f,0.0f); }

protected:

	GameFrame* frame = nullptr;

	BaseMecha* mecha = nullptr;

	MechaPartsObject* obj = nullptr;



};
