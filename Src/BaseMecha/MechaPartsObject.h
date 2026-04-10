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

	inline void SetPartsParameters(ChPtr::Shared<PartsParameters> _param)
	{
		if (partsParameter != nullptr)return;
		if (_param == nullptr)return;

		partsParameter = _param;
	}

	inline void SetRotate(float _rotate) { rotate = _rotate; }

public:


	inline float GetRotate() { return rotate; }


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

	float GetDurableValue() { return durableValue; }

	unsigned char GetPartsPosName() { return partsPosName; }

	size_t GetPartsPosNo() { return partsPosNo; }

	float GetDamage(ChCpp::SphereCollider& _sphereCollider, AttackObject& _bullet);

	float GetDamage(ChCpp::BoxCollider& _collider);

	float GetDamage(AttackObject& _bullet);

	ChVec3 GetColliderSize() { return collider.GetScl(); }

	size_t GetLookAnchorNo() { return lookAnchorNo; }

	std::wstring GetPartsName();

	inline ChPtr::Shared<PartsParameters>GetPartsParameters() { return partsParameter; }

	inline char GetWeaponPaletteCounter(size_t _num, WeaponHandType _type)
	{
		if (_num >= weaponPaletteCounter[ChStd::EnumCast(_type)].size())
			return NOT_WEAPON_REGIST_NUM;

		return weaponPaletteCounter[ChStd::EnumCast(_type)][_num];
	}

public:

	inline void AddWeaponPaletteCounter(char _num, WeaponHandType _type)
	{
		weaponPaletteCounter[ChStd::EnumCast(_type)].push_back(_num);
	}

public:

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
	float rotate = 0.0f;
	ChVec3 rotateDirection = ChVec3();

	bool isInitRunFlg = false;

	std::vector<ChPtr::Shared<WeaponFunction>>weaponFunctions;
	std::vector<char>weaponPaletteCounter[DRAW_TYPE_COUNT];

	std::map<std::wstring,ChPtr::Shared<MechaPartsObject>> positions;

	//パーツの解除フラグ//
	bool releaseFlg = false;

	//パーツの耐久地//
	float durableValue = 100.0f;

	unsigned char partsPosName = -1;
	size_t partsPosNo = 0;

	size_t lookAnchorNo = -1;

	BaseMecha* mecha = nullptr;

	ChPtr::Shared<PartsParameters>partsParameter = nullptr;
};
