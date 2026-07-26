#pragma once

#include"MechaParts.h"

#include"WeaponHandType.h"

class GameFrame;
class ExternalFunction;
class WeaponFunction;
class MechaPartsObject;

class Attack;
class AttackObject;

class MechaPartsObject:public ChCpp::TransformObject<wchar_t>
{
public:

	friend MechaParts;

public:

	void CreateEnd();

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

	void RemoveChildObject(const std::wstring& _objectType);

public:

	void SetFrame(GameFrame* _frame) { frame = _frame; }

	void SetBaseMecha(BaseMecha* _mecha) { mecha = _mecha; }

	void SetLookAnchorNo(const size_t _no) { lookAnchorNo = _no; }

	void SetHitSize();

	inline void SetPartsParameters(ChPtr::Shared<PartsParameters> _param)
	{
		if (partsParameter != nullptr)return;
		if (_param == nullptr)return;

		partsParameter = _param;
	}

	void SetLookTargetParts(MechaPartsObject* _weaponParts);

private:

	void SetRotation(float _rad);

public:

	float GetGroundHeight();

	float GetCameraBaseHeight();

	std::vector<MechaPartsObject*>GetParentTree();

	inline RotateDirectionType GetThisRotateType() { return thisRotateType; }

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

	std::wstring GetPartsPosName() { return partsPosName; }

	float GetDurableValue() { return durableValue; }

	float GetDamage(ChCpp::SphereCollider& _sphereCollider, AttackObject& _bullet);

	float GetDamage(ChCpp::BoxCollider& _collider);

	float GetDamage(AttackObject& _bullet);

	float GetColliderSize() { return collider.GetSize(); }

	size_t GetLookAnchorNo() { return lookAnchorNo; }

	std::wstring GetPartsName();

	inline ChPtr::Shared<PartsParameters>GetPartsParameters() { return partsParameter; }

	inline char GetWeaponPaletteCounter(size_t _num, WeaponHandType _type)
	{
		if (_num >= weaponPaletteCounter[ChStd::EnumCast(_type)].size())
			return NOT_WEAPON_REGIST_NUM;

		return weaponPaletteCounter[ChStd::EnumCast(_type)][_num];
	}

private:

	ChVec3 GetCameraLookPos();

public:

	bool IsAutoRotateLookTarget();

private:

	void UpdateLookTargetDirectionVertical();

	void UpdateLookTargetDirectionHorizontal();

public:

	inline void AddWeaponPaletteCounter(char _num, WeaponHandType _type)
	{
		weaponPaletteCounter[ChStd::EnumCast(_type)].push_back(_num);
	}

public:

	void Update()override;

	void Move()override;

	void DrawBegin()override;

	void Draw3D()override;

	void DrawEnd();

protected:

	ChCpp::SphereCollider collider;
	MechaParts* baseParts;

private:

	GameFrame* frame = nullptr;

	RotateDirectionType thisRotateType = RotateDirectionType::None;
	MechaPartsObject* lookTargetBasePosParts = nullptr;
	unsigned long posPartsCount = 0;
	float rotateInverseValue = 1.0f;
	bool isRotateFlg = false;

	bool isInitRunFlg = false;

	std::vector<ChPtr::Shared<WeaponFunction>>weaponFunctions;
	std::vector<char>weaponPaletteCounter[DRAW_TYPE_COUNT];

	std::map<std::wstring,ChPtr::Shared<MechaPartsObject>> positions;

	//パーツの解除フラグ//
	bool releaseFlg = false;

	//パーツの耐久地//
	float durableValue = 100.0f;

	std::wstring partsPosName = L"";

	size_t lookAnchorNo = -1;

	BaseMecha* mecha = nullptr;

	ChPtr::Shared<PartsParameters>partsParameter = nullptr;
};
