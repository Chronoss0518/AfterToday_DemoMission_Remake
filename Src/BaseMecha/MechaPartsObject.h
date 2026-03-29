#pragma once

#include"MechaParts.h"

#include"WeaponHandType.h"

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

	void SerializeWeapon(ChPtr::Shared<ChCpp::JsonObject<wchar_t>>& _obj, const std::wstring& _jsonParameterText, WeaponHandType _type);

public:

	void AddWeaponFunction(ChPtr::Shared<WeaponFunction> _weapon)
	{
		weaponFunctions.push_back(_weapon);
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

	float GetDamage(ChCpp::SphereCollider& _sphereCollider, AttackObject& _bullet);

	float GetDamage(ChCpp::BoxCollider& _collider);

	float GetDamage(AttackObject& _bullet);

	ChVec3 GetColliderSize() { return collider.GetScl(); }

	size_t GetLookAnchorNo() { return lookAnchorNo; }

	std::wstring GetPartsName();

public:

	void Update()override;

public:

	void DrawBegin()override;

	void Draw3D()override;

	void DrawEnd();

protected:

	ChCpp::SphereCollider collider;
	MechaParts* baseParts;
	ChLMat positionLastDrawMat;

private:

	GameFrame* frame = nullptr;

	StartRotatePosture startRotatePosture = StartRotatePosture::None;
	bool isInitRunFlg = false;

	std::vector<ChPtr::Shared<WeaponFunction>>weaponFunctions;

	std::map<std::wstring,ChPtr::Shared<MechaPartsObject>> positions;

	//パーツの解除フラグ//
	bool releaseFlg = false;

	//パーツの耐久地//
	float durableValue = 100.0f;

	unsigned char partsPosName = -1;
	size_t partsPosNo = 0;

	size_t lookAnchorNo = -1;

	BaseMecha* mecha = nullptr;
};
