#pragma once

#include"MechaParts.h"

class GameFrame;
class ExternalFunction;
class WeaponFunction;
class MechaPartsObject;

class Attack;
class AttackObject;

struct ControllerListItem
{
	ChPtr::Shared<PostureController> controller = nullptr;
	MechaPartsObject* partsObejct = nullptr;
};

enum class StartRotatePosture
{
	XY,
	YX,
	None
};

class MechaPartsObject
{
public:

	friend MechaParts;

public:

	void CreateAnchor();

	void CreatePostureList();

public:

	void Release();

public:

	ChPtr::Shared<ChCpp::JsonObject> Serialize();

public:

	void AddWeaponFunction(ChPtr::Shared<WeaponFunction> _weapon)
	{
		weaponFunctions.push_back(_weapon);
	}

	void AddExternalFunction(ChPtr::Shared<ExternalFunction> _function)
	{
		externalFunctions.push_back(_function);
	}

	inline void AddChildObject(const std::string& _objectType, ChPtr::Shared<MechaPartsObject> _partsObject)
	{
		if (_partsObject == nullptr)return;

		_partsObject->SetPositoinObject(this, baseParts->GetPositionList()[_objectType]);

		auto&& tmpObject = positions.find(_objectType);
		if (tmpObject == positions.end())
		{
			positions[_objectType] = _partsObject;

			_partsObject->CreatePostureList();
			return;
		}

		if (tmpObject->second != nullptr)
		{
			positions.erase(tmpObject);
			positions[_objectType] = _partsObject;

			_partsObject->CreatePostureList();

			return;
		}

		tmpObject->second = _partsObject;
		_partsObject->CreatePostureList();

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

	void SetPostureRotation(ChCpp::BaseObject* const _target, float _rotation)
	{
		ChPtr::Shared<PostureRotateData>rotateData = postureRotateList[_target];
		if (rotateData == nullptr)
		{
			rotateData = ChPtr::Make_S<PostureRotateData>();
			postureRotateList[_target] = rotateData;
		}
		rotateData->rotate = ChMath::ToDegree(_rotation);
		rotateData->updateFlg = true;
	}

	void SetPartsPosData(unsigned char _names, unsigned long _no) { partsPosName = _names; partsPosNo = _no; }

	void SetRWeapon(const bool _flg) { weaponType.SetBitFlg(0,_flg); }

	void SetLWeapon(const bool _flg) { weaponType.SetBitFlg(1, _flg); }

	void SetFrame(GameFrame* _frame) { frame = _frame; }

	void SetBaseMecha(BaseMecha* _mecha) { mecha = _mecha; }

	void SetLookAnchorNo(const unsigned long _no) { lookAnchorNo = _no; }

	void SetHitSize();

public:

	std::vector<ChPtr::Weak<PostureController>>& GetPostureControllerList() { return baseParts->GetPostureControllerList(); };

	MechaParts* GetBaseObject() { return baseParts; }

	BaseMecha* GetBaseMecha() { return mecha; }

	ChPtr::Shared<ChCpp::FrameObject> GetPositionObject() { return positionObject; }

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

	inline ChLMat GetLastDrawMat() { return lastDrawMat; }

	inline ChLMat GetPositionLastDrawMat() { return positionLastDrawMat; }

	float GetDurableValue() { return durableValue; }

	unsigned char GetPartsPosName() { return partsPosName; }

	unsigned long GetPartsPosNo() { return partsPosNo; }

	bool GetRWeapon() { return weaponType.GetBitFlg(0); }

	bool GetLWeapon() { return weaponType.GetBitFlg(1); }

	float GetDamage(ChCpp::SphereCollider& _sphereCollider, AttackObject& _bullet);

	float GetDamage(ChCpp::BoxCollider& _collider);

	float GetDamage(AttackObject& _bullet);

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

	inline std::vector<ChPtr::Shared<ControllerListItem>>GetControllerList() { return controllerList; }

	inline StartRotatePosture GetStartRotatePosture() { return startRotatePosture; }

public:

	void AddPostureRotation(ChCpp::BaseObject* const _target, float _rotation)
	{
		ChPtr::Shared<PostureRotateData>rotateData = postureRotateList[_target];
		if (rotateData == nullptr)
		{
			rotateData = ChPtr::Make_S<PostureRotateData>();
			postureRotateList[_target] = rotateData;
		}
		rotateData->rotate += ChMath::ToDegree(_rotation);
		rotateData->updateFlg = true;
	}

public:

	void Update();

private:

	void DrawStart();

public:

	void DrawStartFunction();

	virtual void Draw(const ChLMat& _drawMat);

	void DrawEnd();

private:

	void FunctionDrawBegin();

	void FunctionDrawEnd();

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
	ChLMat positionLastDrawMat;

private:

	struct PostureRotateData
	{
		float rotate = 0;
		bool updateFlg = false;
	};

	GameFrame* frame = nullptr;

	//Autoで敵を追尾する時用に修正//
	std::map<ChCpp::BaseObject*, ChPtr::Shared<PostureRotateData>>postureRotateList;
	std::vector<ChPtr::Shared<ControllerListItem>>controllerList;
	StartRotatePosture startRotatePosture = StartRotatePosture::None;
	bool isInitRunFlg = false;

	std::vector<ChPtr::Shared<ExternalFunction>>externalFunctions;
	std::vector<ChPtr::Shared<WeaponFunction>>weaponFunctions;

	std::map<std::string,ChPtr::Shared<MechaPartsObject>> positions;

	unsigned long useAttackType = 0;

	ChPtr::Shared<ChCpp::FrameObject>positionObject = nullptr;
	MechaPartsObject* parentObject = nullptr;

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
