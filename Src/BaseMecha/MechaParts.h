#ifndef _BRobParts
#define _BRobParts

#include<wchar.h>
#include <stdio.h>
#include <string.h>

#ifndef GET_CLASS_NAME
#define GET_CLASS_NAME(cls) L## #cls
#endif

#include"BaseMecha.h"
 
#ifndef JSON_PROPEATY_PARTS_NAME
#define JSON_PROPEATY_PARTS_NAME L"Parts"
#endif

#ifndef JSON_PROPEATY_RIGHT_WEAPON
#define JSON_PROPEATY_RIGHT_WEAPON L"RightWeapon"
#endif

#ifndef JSON_PROPEATY_LEFT_WEAPON
#define JSON_PROPEATY_LEFT_WEAPON L"LeftWeapon"
#endif

#ifndef PARTS_DIRECTORY
#define PARTS_DIRECTORY(current_path) TARGET_DIRECTORY(L"RobotParts/" current_path)
#endif

class MechaPartsObject;
class CameraComponent;
class PartsDataBase;

struct PartsParameters;

namespace PartsParameterStruct
{
	struct BoostData;
	struct WeaponData;
}

class MechaParts : public ChCpp::BaseObject<wchar_t>
{
public:

	static std::map<std::wstring,ChPtr::Shared<MechaParts>>& LoadPartsList()
	{
		static std::map<std::wstring,ChPtr::Shared<MechaParts>>ins;
		return ins;
	}

	inline static void ClearPartsList()
	{
		auto&& partsList = LoadPartsList();
		if (partsList.empty())return;
		partsList.clear();
	}

public:

	void RemoveParameter(BaseMecha& _base);

private:

	unsigned long CreateDatas(BaseMecha& _base, ChCpp::TextObject<wchar_t>& _textObject, unsigned long _linePos);

	void CreateChild(ChPtr::Shared<MechaPartsObject> _partsObject,BaseMecha& _base, ID3D11Device* _device, ChD3D11::Shader::BaseDrawMesh11<wchar_t>* _drawer, GameFrame* _frame, ChPtr::Shared<ChCpp::JsonObject<wchar_t>> _jsonObject);

public://Serialize Deserialize//

	static ChPtr::Shared<MechaPartsObject> LoadParts(BaseMecha& _base, ID3D11Device* _device, ChD3D11::Shader::BaseDrawMesh11<wchar_t>* _drawer, GameFrame* _frame, const std::wstring& _partsFilePath);

	static ChPtr::Shared<MechaPartsObject> LoadParts(BaseMecha& _base, ID3D11Device* _device, ChD3D11::Shader::BaseDrawMesh11<wchar_t>* _drawer, GameFrame* _frame, ChPtr::Shared<ChCpp::JsonObject<wchar_t>> _jsonObject,const std::wstring& _positionObjectType = L"", ChPtr::Shared<MechaPartsObject> _parent = nullptr);

	void Load(BaseMecha& _base, ID3D11Device* _device, const std::wstring& _fileName);

	void Deserialize(BaseMecha& _base,ID3D11Device* _device,const std::wstring& _text);

	std::wstring Save(const std::wstring& _fileName);

	std::wstring Serialize();

private:

	void LoadModel(ID3D11Device* _device, const std::wstring& _fileName);

public://Set Functions//

	ChPtr::Shared<MechaPartsObject> SetParameters(BaseMecha& _base, GameFrame* _frame, ChPtr::Shared<ChCpp::JsonObject<wchar_t>> _jsonObject);

	virtual ChPtr::Shared<MechaPartsObject>  SetPartsParameter(BaseMecha& _base);

	void SetParameters();

	virtual ChPtr::Shared<PartsParameters>  SetPartsParameter();

	void SetHardness(const unsigned long _hardness) { hardness = _hardness; }

	void SetMass(const float _mass) { mass = _mass; }

	inline void SetMeshDrawer(ChD3D11::Shader::BaseDrawMesh11<wchar_t>* _drawer) { drawer = _drawer; }

public://Get Function//

	std::vector<ChPtr::Weak<PostureController>>& GetPostureControllerList() { return postureList; };

	inline unsigned long GetHardness()const { return hardness; }

	inline float GetMass()const { return mass; }

	inline float GetGroundHeight() { return groundHeight; }

	inline ChD3D11::Mesh11<wchar_t>& GetMesh() { return *model; }

	inline ChLMat GetDefaultFrameMat() { return defaultFrameMat; }

	inline std::wstring GetThisFileName() { return thisFileName; }

	inline std::wstring GetThisFilePath() { return thisFilePath; }

	inline ChD3D11::Shader::BaseDrawMesh11<wchar_t>* GetMeshDrawer() { return drawer; }

	inline std::map<std::wstring, ChPtr::Shared<ChCpp::FrameObject<wchar_t>>>& GetPositionList() { return positions; }

public:

	inline void AddPosition(const std::wstring& _parameter, ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _frame)
	{
		positions[_parameter] = _frame;
	}

	inline void AddPosture(ChPtr::Weak<PostureController> _posture)
	{
		postureList.push_back(_posture);
	}

	void AddWeaponData(ChPtr::Shared<MechaPartsObject> _partsObject,BaseMecha& _base, ChPtr::Shared<ChCpp::JsonObject<wchar_t>> _jsonObject);

public:

	void Draw(const ChMat_11& _mat);

private:

	//パーツの硬さ//
	unsigned long hardness = 1;

	//パーツの重さ//
	float mass = 1.0f;

	std::wstring thisFileName = L"";
	std::wstring thisFilePath = L"";

	float groundHeight = 0.0f;

	ChPtr::Shared<ChD3D11::Mesh11<wchar_t>> model = ChPtr::Make_S<ChD3D11::Mesh11<wchar_t>>();
	ChLMat defaultFrameMat;

	ChD3D11::Shader::BaseDrawMesh11<wchar_t>* drawer = nullptr;

	static std::map<std::wstring, std::function<ChPtr::Shared<PartsDataBase>(MechaParts&)>>createFunctions;

	std::map<std::wstring, ChPtr::Shared<ChCpp::FrameObject<wchar_t>>> positions;


	std::vector<ChPtr::Weak<PostureController>> postureList;

	constexpr static const char* GetModelNameTag() { return "ModelName:\n"; }

	constexpr static const char* GetHardnessTag() { return "Hardness:\n"; }

	constexpr static const char* GetMassTag() { return "Mass:\n"; }

};

class PartsDataBase :public ChCpp::BaseComponent
{
public:

	virtual void RemoveParameter(BaseMecha& _base){}

public://Serialize Deserialize//

	virtual unsigned long Deserialize(const ChCpp::TextObject<wchar_t>& _text,const unsigned long _textPos = 0) = 0;

	virtual std::wstring Serialize() = 0;

public://Set Functions//

	virtual void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame) = 0;

	virtual void SetPartsParameter(PartsParameters& _base) = 0;

public://Get Functions//

	virtual std::wstring GetPartsTypeTag() = 0;

	template<class T>
	ChPtr::Shared<T> GetComponent(ChCpp::BaseObject<wchar_t>& _base)
	{
		auto&& ComList = _base.GetComponents<T>();
		if (!ComList.empty())
		{
			return ComList[0];
		}

		return _base.SetComponent<T>();
	}

public:

	//virtual void ReleaseParts(BaseMecha& _base) = 0;

};

class EnelgyTankData : public PartsDataBase
{

public:

	void RemoveParameter(BaseMecha& _base)override;

public://Serialize Deserialize//

	virtual unsigned long Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos = 0)override;

	virtual std::wstring Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)override;

	void SetPartsParameter(PartsParameters& _base)override;

	void SetMaxEnelgy(const unsigned long _maxEnelgy) { maxEnelgy = _maxEnelgy; }

	void SetChargeEnelgy(const unsigned long _createEnelgy) { chargeEnelgy = _createEnelgy; }

public://Get Functions//

	virtual std::wstring GetPartsTypeTag()override { return  GET_CLASS_NAME(EnelgyTankData); }

	unsigned long GetMaxEnelgy()const { return maxEnelgy; }

	unsigned long GetChargeEnelgy()const { return chargeEnelgy; }

protected:

	//保持可能最大エネルギー//
	unsigned long maxEnelgy = 0;

	//エネルギーの生成量//
	unsigned long chargeEnelgy = 0;

};

class CameraData : public PartsDataBase
{

public://Serialize Deserialize//

	virtual unsigned long Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos = 0)override;

	virtual std::wstring Serialize()override;

public://Set Functions//

	virtual void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)override;

	void SetPartsParameter(PartsParameters& _base)override {};

	void SetFovy(const float _fovy) { fovy = _fovy; }

	void SetCameraObjectName(const std::wstring& _cameraObject) { cameraObject = _cameraObject; }

	void SetCameraCount(const unsigned long _cameraCount) { cameraCount = _cameraCount; }

public://Get Functions//

	virtual std::wstring GetPartsTypeTag()override{ return GET_CLASS_NAME(CameraData); }

	float GetFovy()const { return fovy; }

protected:

	//カメラの視野角//
	float fovy = 60.0f;

	//3DModelのカメラの位置
	std::wstring cameraObject = L"";

	//機体内に表示されるカメラの数
	unsigned long cameraCount = 1;
	
};

class ScopeData : public CameraData
{

public://Serialize Deserialize//

	unsigned long Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos = 0)override;

	std::wstring Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)override;

	void SetPartsParameter(PartsParameters& _base)override {};

	void SetMinFovy(const float _minFovy) { minFovy = _minFovy; }
	
	void SetMaxFovy(const float _maxFovy) { maxFovy = _maxFovy; }

	void SetFovySlideSpeed(const float _fovySlideSpeed) { fovySlideSpeed = _fovySlideSpeed; }

public://Get Functions//

	std::wstring GetPartsTypeTag()override { return GET_CLASS_NAME(ScopeData); }

	float GetMaxFovy()const { return minFovy; }

	float GetMinFovy()const { return maxFovy; }

	float GetFovySlideSpeed()const { return fovySlideSpeed; }

protected:

	//最小の視野角//
	float minFovy = 10.0f;
	
	//最大の視野角//
	float maxFovy = 90.0f;

	//視野角を変動させるスピード//
	float fovySlideSpeed = 1.0f;

};

class Aerodynamics : public PartsDataBase
{
public://Serialize Deserialize//

	unsigned long Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos = 0)override;

	std::wstring Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)override;

	void SetPartsParameter(PartsParameters& _base)override {};

	void SetUpPowerParSpeed(const float _upPowerParSpeed) { upPowerParSpeed = _upPowerParSpeed; }

public://Get Functions//

	float GetGroundNearPos() { return 0.0f; };

	std::wstring GetPartsTypeTag()override { return GET_CLASS_NAME(Aerodynamics); }

protected:

	//速度による反下降補正//
	float  upPowerParSpeed = 0.0f;
};

class MoveAcceleration : public PartsDataBase
{
public://Serialize Deserialize//

	unsigned long Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos = 0)override;

	std::wstring Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)override;

	void SetPartsParameter(PartsParameters& _base)override {};

	void SetAcceleration(const float _acceleration) { acceleration = _acceleration; }

	void SetDeceleration(const float _deceleration) { deceleration = _deceleration; }

public://Get Functions//

	float GetGroundNearPos() { return 0.0f; };

	std::wstring GetPartsTypeTag()override { return GET_CLASS_NAME(MoveAcceleration); }

protected:

	//前方への加速度//
	float acceleration = 0.0f;

	//前方への減速度//
	float deceleration = 0.0f;
};

class WalkData :public PartsDataBase
{

public:

	void RemoveParameter(BaseMecha& _base)override;

public://Serialize Deserialize//

	virtual unsigned long Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos = 0)override;

	virtual std::wstring Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)override;

	void SetPartsParameter(PartsParameters& _base)override;

	void SetMovePow(const float _movePow) { movePow = _movePow; }
	
	void SetRotatePow(const float _rotatePow) { rotatePow = _rotatePow; }

	void SetJumpPow(const float _jumpPow) { jumpPow = _jumpPow; }

public://Get Functions//

	float GetGroundNearPos() { return 0.0f; };

	std::wstring GetPartsTypeTag()override { return GET_CLASS_NAME(WalkData); }

	float GetMovePower()const { return movePow; }

	float GetRotatePower()const { return rotatePow; }

	float GetJumpPower()const { return jumpPow; }

protected:
	
	//移動力//
	float movePow = 0.0f;
	//回転力//
	float rotatePow = 0.0f;
	//飛翔力//
	float jumpPow = 0.0f;

	constexpr static const char* GetSpeedTag() { return "Speed:\n"; }

};


class NextPosBase : public PartsDataBase
{
public://Serialize Deserialize//

	inline virtual unsigned long Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos = 0)override
	{

		SetParamName(_text.GetTextLine(_textPos));

		return _textPos + 1;
	}

	inline virtual std::wstring Serialize()override
	{

		std::wstring res = L"";
		res += nextPosName + L"\n";

		return res;
	}

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)override;

	void SetPartsParameter(PartsParameters& _base) override{};

	virtual void SetObjectPos(BaseMecha& _base, MechaPartsObject& _parts, ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _targetObject) = 0;

	inline void SetParamName(const std::wstring& _objectName)
	{
		nextPosName = _objectName;
	}

public://Get Functions//

	inline std::wstring GetObjectName() { return nextPosName; }

protected:

	std::wstring nextPosName = L"";

};

class NextPos : public NextPosBase
{
public://Serialize Deserialize//

	inline unsigned long Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos = 0)override
	{
		unsigned long textPos = NextPosBase::Deserialize(_text, _textPos);
		SetConnectionName(_text.GetTextLine(textPos));

		return textPos + 1;
	}

	inline std::wstring Serialize()override
	{

		std::wstring res = NextPosBase::Serialize();
		res += connectionName + L"\n";

		return res;
	}

public://Set Functions//

	inline void SetConnectionName(const std::wstring& _connectionName)
	{
		connectionName = _connectionName;
	}

	inline void SetMaxWeight(const float _weight)
	{
		maxWeight = _weight;
	}

	void SetObjectPos(BaseMecha& _base, MechaPartsObject& _parts, ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _targetObject)override;

public://Get Functions//

	std::wstring GetPartsTypeTag()override { return GET_CLASS_NAME(NextPos); }

	inline std::wstring GetConnectionName() { return connectionName; }

	inline float GetMaxWeight() { return maxWeight; }


protected:

	//接続部の名称//
	std::wstring connectionName = L"";

	//最大重量//
	float maxWeight = 0.0f;

};

class Posture : public NextPosBase
{
public://Serialize Deserialize//

	inline unsigned long Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos = 0)override
	{
		unsigned long textPos = NextPosBase::Deserialize(_text, _textPos);
		SetRotateAxis(static_cast<RotateAxis>(ChStr::GetNumFromText<int,wchar_t>(_text[textPos])));
		SetMinRotate(ChStr::GetNumFromText<float, wchar_t>(_text[textPos + 1]));
		SetMaxRotate(ChStr::GetNumFromText<float, wchar_t>(_text[textPos + 2]));

		return textPos + 3;
	}

	inline std::wstring Serialize()override
	{
		std::wstring res = NextPosBase::Serialize();
		res += std::to_wstring(static_cast<unsigned char>(GetRotateAxis())) + L"\n";
		res += std::to_wstring(GetMinRotate()) + L"\n";
		res += std::to_wstring(GetMaxRotate()) + L"\n";

		return res;
	}

public://Set Functions//

	inline void SetRotateAxis(const RotateAxis _axis)
	{
		posture.SetRotateAxis(_axis);
	}

	inline void SetMinRotate(const float& _rotate)
	{
		posture.SetMinRotate(_rotate);
	}

	inline void SetMaxRotate(const float& _rotate)
	{
		posture.SetMaxRotate(_rotate);
	}

	void SetObjectPos(BaseMecha& _base, MechaPartsObject& _parts, ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _targetObject)override;

public://Get Functions//

	std::wstring GetPartsTypeTag()override { return GET_CLASS_NAME(Posture); }


	inline RotateAxis GetRotateAxis()
	{
		return posture.GetRotateAxis();
	}

	inline float GetMinRotate()
	{
		return posture.GetMinRotate();
	}

	inline float GetMaxRotate()
	{
		return posture.GetMinRotate();
	}


protected:

	PostureController posture;

};

class BoostBrust :public PartsDataBase
{

public:

	void RemoveParameter(BaseMecha& _base)override;

public://Serialize Deserialize//

	unsigned long Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos = 0)override;

	std::wstring Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)override;

	void SetBoostData(PartsParameterStruct::BoostData& _boost);

	inline void SetParamName(const std::wstring& _objectName)
	{
		objectName = _objectName;
	}

	void SetUseEnelgy(const unsigned long _useEnelgy) { useEnelgy = _useEnelgy; }

	void SetBoostPower(const float _boostPower) { boostPower = _boostPower; }

	void SetAvoidUseEnelgy(const unsigned long _avoidUseEnelgy) { avoidUseEnelgy = _avoidUseEnelgy; }

	void SetAvoidPower(const float _avoidPow) { avoidPow = _avoidPow; }

public://Get Functions//

	inline std::wstring GetObjectNameList() { return objectName; }

	unsigned long GetBoostUseEnelgy()const { return useEnelgy; }

	float GetBoostPower()const { return boostPower; }

	unsigned long GetAvoidUseEnelgy()const { return avoidUseEnelgy; }

	float GetAvoidPower()const { return avoidPow; }

	unsigned long GetAvoidWait()const { return avoidWait; }

	ChPtr::Shared<ChCpp::FrameObject<wchar_t>> GetFrame(BaseMecha& _base);

	virtual BaseMecha::InputName GetBoostInputName() = 0;

	virtual BaseMecha::InputName GetAvoidInputName() = 0;

protected:

	std::wstring objectName;

	//Boost使用時のエネルギー消費量//
	unsigned long useEnelgy = 0;
	//Boost使用時における移動力//
	float boostPower = 0.0f;
	//回避時のエネルギー消費量//
	unsigned long avoidUseEnelgy = 0;
	//回避する際の移動力//
	float avoidPow = 0.0f;
	//回避する際の再使用時間//
	unsigned long avoidWait = 0;

};

class RightBoostBrust :public BoostBrust
{
public:

	void SetPartsParameter(PartsParameters& _base)override;

public://Get Functions//

	inline BaseMecha::InputName GetBoostInputName() override { return BaseMecha::InputName::RightBoost; }

	inline BaseMecha::InputName GetAvoidInputName() override { return BaseMecha::InputName::RightAvo; }

	std::wstring GetPartsTypeTag()override { return GET_CLASS_NAME(RightBoostBrust); }

};

class LeftBoostBrust :public BoostBrust
{
public:

	void SetPartsParameter(PartsParameters& _base)override;

public://Get Functions//

	inline BaseMecha::InputName GetBoostInputName() override { return BaseMecha::InputName::LeftBoost; }

	inline BaseMecha::InputName GetAvoidInputName() override { return BaseMecha::InputName::LeftAvo; }

	std::wstring GetPartsTypeTag()override { return GET_CLASS_NAME(LeftBoostBrust); }


};

class FrontBoostBrust :public BoostBrust
{
public:

	void SetPartsParameter(PartsParameters& _base)override;

public://Get Functions//

	inline BaseMecha::InputName GetBoostInputName() override { return BaseMecha::InputName::FrontBoost; }

	inline BaseMecha::InputName GetAvoidInputName() override { return BaseMecha::InputName::FrontAvo; }

	std::wstring GetPartsTypeTag()override { return GET_CLASS_NAME(FrontBoostBrust); }

};

class BackBoostBrust :public BoostBrust
{
public:

	void SetPartsParameter(PartsParameters& _base)override;

public://Get Functions//

	inline BaseMecha::InputName GetBoostInputName() override { return BaseMecha::InputName::BackBoost; }

	inline BaseMecha::InputName GetAvoidInputName() override { return BaseMecha::InputName::BackAvo; }

	std::wstring GetPartsTypeTag()override { return GET_CLASS_NAME(BackBoostBrust); }

};

class UpBoostBrust :public BoostBrust
{
public:

	void SetPartsParameter(PartsParameters& _base)override;

public://Get Functions//

	inline BaseMecha::InputName GetBoostInputName() override { return BaseMecha::InputName::UpBoost; }

	inline BaseMecha::InputName GetAvoidInputName() override { return BaseMecha::InputName::UpAvo; }

	std::wstring GetPartsTypeTag()override { return GET_CLASS_NAME(UpBoostBrust); }

};

class DownBoostBrust :public BoostBrust
{
public:

	void SetPartsParameter(PartsParameters& _base)override;

public://Get Functions//

	inline BaseMecha::InputName GetBoostInputName() override { return BaseMecha::InputName::DownBoost; }

	inline BaseMecha::InputName GetAvoidInputName() override { return BaseMecha::InputName::DownAvo; }

	std::wstring GetPartsTypeTag()override { return GET_CLASS_NAME(DownBoostBrust); }

};

class WeaponData : public NextPosBase
{
public://Serialize Deserialize//

	virtual unsigned long Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos = 0)override;

	virtual std::wstring Serialize()override;

public:

	inline void SetWeaponName(const std::wstring& _weaponName) { weaponName = _weaponName; }

	inline void SetSEFileName(const std::wstring& _seFile) { seFile = _seFile; }

	inline void SetWaitTime(const unsigned long _waitTime) { waitTime = _waitTime; }

	inline void SetLookTargetFlg(const bool _flg) { lookTarget = _flg; }

	void SetObjectPos(BaseMecha& _base, MechaPartsObject& _parts, ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _targetObject)override;

	void SetWeaponData(PartsParameterStruct::WeaponData& _base);

public:

	inline std::wstring GetWeaponName() { return weaponName; }

	inline std::wstring GetSEFileName() { return seFile; }

	inline unsigned long GetWaitTime() const { return waitTime; }

	inline bool GetLookTargetFlg() { return lookTarget; }

protected:

	//武器の名称//
	std::wstring weaponName = L"";
	//効果音のファイル//
	std::wstring seFile = L"";
	//次の攻撃可能時間//
	unsigned long waitTime = 0;

	//対象を追尾するかのフラグ//
	bool lookTarget = false;

};

class SwordData :public WeaponData
{
public://Serialize Deserialize//

	unsigned long Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos = 0)override;

	std::wstring Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)override;

	void SetPartsParameter(PartsParameters& _base)override;

	inline void SetAttackTime(const unsigned long _attackTime) { attackTime = _attackTime; }

public://Get Functions//

	inline std::wstring GetPartsTypeTag() override { return GET_CLASS_NAME(SwordData); }

	inline unsigned long GetWeatTime() { return attackTime; }

protected:
	
	unsigned long damageParSpeed = 0;

	unsigned long attackTime = 0;

};

class GunData :public WeaponData
{

public://Serialize Deserialize//

	unsigned long Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos = 0)override;

	std::wstring Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)override;

	void SetPartsParameter(PartsParameters& _base)override;

	inline void SetFireNum(const unsigned long _fireNum) { fireNum = _fireNum; }

	inline void SetBulletNum(const unsigned long _bulletNum) { bulletNum = _bulletNum; }

	inline void SetMagazineNum(const unsigned long _magazineNum) { magazineNum = _magazineNum; }

	inline void SetReloadTime(const unsigned long _reloadTime) { reloadTime = _reloadTime; }

	inline void SetBulletFile(const std::wstring& _bulletFile) { bulletFile = _bulletFile; }

	inline void SetFrontDirection(const ChVec3& _dir){ frontDir = _dir; }

public://Get Functions//

	inline std::wstring GetPartsTypeTag() override { return GET_CLASS_NAME(GunData); }

	inline unsigned long GetFireNum()const { return fireNum; }

	inline unsigned long GetBulletNum() const { return bulletNum; }

	inline unsigned long GetMagazineNum() const { return magazineNum; }

	inline unsigned long GetReloadTime() const { return reloadTime; }

	inline unsigned char GetRange() const { return range; }

	inline std::wstring GetUseBulletFile() const { return bulletFile; }

	inline ChVec3 GetFrontDirection()const { return frontDir; }

protected:

	//射撃時の弾の数//
	unsigned long fireNum = 1;
	//一回のリロードで打てる球の数//
	unsigned long bulletNum = 0;
	//リロードできる回数//
	unsigned long magazineNum = 0;
	//リロードのかかる時間//
	unsigned long reloadTime = 0;

	//弾の発射方向の誤差//
	unsigned char range = 0;

	std::wstring bulletFile = L"";

	//攻撃時に向ける方向//
	ChVec3 frontDir;
};


#endif
