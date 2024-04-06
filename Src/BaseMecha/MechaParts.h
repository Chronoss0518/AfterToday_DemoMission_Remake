#ifndef _BRobParts
#define _BRobParts

#include <stdio.h>
#include <string.h>

#ifndef GET_CLASS_NAME
#define GET_CLASS_NAME(cls) #cls
#endif

#include"BaseMecha.h"
 
#ifndef JSON_PROPEATY_PARTS_NAME
#define JSON_PROPEATY_PARTS_NAME "Parts"
#endif

#ifndef JSON_PROPEATY_RIGHT_WEAPON
#define JSON_PROPEATY_RIGHT_WEAPON "RightWeapon"
#endif

#ifndef JSON_PROPEATY_LEFT_WEAPON
#define JSON_PROPEATY_LEFT_WEAPON "LeftWeapon"
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

class MechaParts :public ChCpp::BaseObject
{
public:

	static std::map<std::string,ChPtr::Shared<MechaParts>>& LoadPartsList()
	{
		static std::map<std::string,ChPtr::Shared<MechaParts>>ins;
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

	unsigned long CreateDatas(BaseMecha& _base, ChCpp::TextObject& _textObject, unsigned long _linePos);

	void CreateChild(ChPtr::Shared<MechaPartsObject> _partsObject,BaseMecha& _base, ID3D11Device* _device, ChD3D11::Shader::BaseDrawMesh11* _drawer, GameFrame* _frame, ChPtr::Shared<ChCpp::JsonObject> _jsonObject);

public://Serialize Deserialize//

	static ChPtr::Shared<MechaPartsObject> LoadParts(BaseMecha& _base, ID3D11Device* _device, ChD3D11::Shader::BaseDrawMesh11* _drawer, GameFrame* _frame, const std::string& _partsFilePath);

	static ChPtr::Shared<MechaPartsObject> LoadParts(BaseMecha& _base, ID3D11Device* _device, ChD3D11::Shader::BaseDrawMesh11* _drawer, GameFrame* _frame, ChPtr::Shared<ChCpp::JsonObject> _jsonObject);

	void Load(BaseMecha& _base, ID3D11Device* _device, const std::string& _fileName);

	void Deserialize(BaseMecha& _base,ID3D11Device* _device,const std::string& _text);

	std::string Save(const std::string& _fileName);

	std::string Serialize();

public://Set Functions//

	ChPtr::Shared<MechaPartsObject> SetParameters(BaseMecha& _base, GameFrame* _frame, ChPtr::Shared<ChCpp::JsonObject> _jsonObject);

	virtual ChPtr::Shared<MechaPartsObject>  SetPartsParameter(BaseMecha& _base);

	void SetParameters();

	virtual ChPtr::Shared<PartsParameters>  SetPartsParameter();

	void SetHardness(const unsigned long _hardness) { hardness = _hardness; }

	void SetMass(const float _mass) { mass = _mass; }

	inline void SetMeshDrawer(ChD3D11::Shader::BaseDrawMesh11* _drawer) { drawer = _drawer; }

public://Get Function//

	inline unsigned long GetHardness()const { return hardness; }

	inline float GetMass()const { return mass; }

	inline float GetGroundHeight() { return groundHeight; }

	inline ChD3D11::Mesh11& GetMesh() { return *model; }

	inline ChLMat GetDefaultFrameMat() { return defaultFrameMat; }

	inline std::string GetThisFileName() { return thisFileName; }

	inline std::string GetThisFilePath() { return thisFilePath; }

	inline ChD3D11::Shader::BaseDrawMesh11* GetMeshDrawer() { return drawer; }

public:

	inline void AddPosition(const std::string& _parameter, ChPtr::Shared<ChCpp::FrameObject> _frame)
	{
		positions[_parameter] = _frame;
	}

	void AddWeaponData(ChPtr::Shared<MechaPartsObject> _partsObject,BaseMecha& _base, ChPtr::Shared<ChCpp::JsonObject> _jsonObject);

public:

	void Draw(const ChMat_11& _mat);

private:

	//パーツの硬さ//
	unsigned long hardness = 1;

	//パーツの重さ//
	float mass = 1.0f;

	std::string thisFileName = "";
	std::string thisFilePath = "";

	float groundHeight = 0.0f;

	ChPtr::Shared<ChD3D11::Mesh11> model = ChPtr::Make_S<ChD3D11::Mesh11>();
	ChLMat defaultFrameMat;

	ChD3D11::Shader::BaseDrawMesh11* drawer = nullptr;

	static std::map<std::string, std::function<ChPtr::Shared<PartsDataBase>(MechaParts&)>>createFunctions;

	std::map<std::string, ChPtr::Shared<ChCpp::FrameObject>> positions;

	constexpr static const char* GetModelNameTag() { return "ModelName:\n"; }

	constexpr static const char* GetHardnessTag() { return "Hardness:\n"; }

	constexpr static const char* GetMassTag() { return "Mass:\n"; }

};

class PartsDataBase :public ChCpp::BaseComponent
{
public:

	virtual void RemoveParameter(BaseMecha& _base){}

public://Serialize Deserialize//

	virtual unsigned long Deserialize(const ChCpp::TextObject& _text,const unsigned long _textPos = 0) = 0;

	virtual std::string Serialize() = 0;

public://Set Functions//

	virtual void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame) = 0;

	virtual void SetPartsParameter(PartsParameters& _base) = 0;

public://Get Functions//

	virtual std::string GetPartsTypeTag() = 0;

	template<class T>
	ChPtr::Shared<T> GetComponent(ChCpp::BaseObject& _base)
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

	virtual unsigned long Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos = 0)override;

	virtual std::string Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)override;

	void SetPartsParameter(PartsParameters& _base)override;

	void SetMaxEnelgy(const unsigned long _maxEnelgy) { maxEnelgy = _maxEnelgy; }

	void SetChargeEnelgy(const unsigned long _createEnelgy) { chargeEnelgy = _createEnelgy; }

public://Get Functions//

	virtual std::string GetPartsTypeTag()override { return  GET_CLASS_NAME(EnelgyTankData); }

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

	virtual unsigned long Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos = 0)override;

	virtual std::string Serialize()override;

public://Set Functions//

	virtual void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)override;

	void SetPartsParameter(PartsParameters& _base)override {};

	void SetFovy(const float _fovy) { fovy = _fovy; }

	void SetCameraObjectName(const std::string& _cameraObject) { cameraObject = _cameraObject; }

	void SetCameraCount(const unsigned long _cameraCount) { cameraCount = _cameraCount; }

public://Get Functions//

	virtual std::string GetPartsTypeTag()override{ return GET_CLASS_NAME(CameraData); }

	float GetFovy()const { return fovy; }

protected:

	//カメラの視野角//
	float fovy = 60.0f;

	//3DModelのカメラの位置
	std::string cameraObject = "";

	//機体内に表示されるカメラの数
	unsigned long cameraCount = 1;
	
};

class ScopeData : public CameraData
{

public://Serialize Deserialize//

	unsigned long Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos = 0)override;

	std::string Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)override;

	void SetPartsParameter(PartsParameters& _base)override {};

	void SetMinFovy(const float _minFovy) { minFovy = _minFovy; }
	
	void SetMaxFovy(const float _maxFovy) { maxFovy = _maxFovy; }

	void SetFovySlideSpeed(const float _fovySlideSpeed) { fovySlideSpeed = _fovySlideSpeed; }

public://Get Functions//

	std::string GetPartsTypeTag()override { return GET_CLASS_NAME(ScopeData); }

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

	unsigned long Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos = 0)override;

	std::string Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)override;

	void SetPartsParameter(PartsParameters& _base)override {};

	void SetUpPowerParSpeed(const float _upPowerParSpeed) { upPowerParSpeed = _upPowerParSpeed; }

public://Get Functions//

	float GetGroundNearPos() { return 0.0f; };

	std::string GetPartsTypeTag()override { return GET_CLASS_NAME(Aerodynamics); }

protected:

	//速度による反下降補正//
	float  upPowerParSpeed = 0.0f;
};

class MoveAcceleration : public PartsDataBase
{
public://Serialize Deserialize//

	unsigned long Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos = 0)override;

	std::string Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)override;

	void SetPartsParameter(PartsParameters& _base)override {};

	void SetAcceleration(const float _acceleration) { acceleration = _acceleration; }

	void SetDeceleration(const float _deceleration) { deceleration = _deceleration; }

public://Get Functions//

	float GetGroundNearPos() { return 0.0f; };

	std::string GetPartsTypeTag()override { return GET_CLASS_NAME(MoveAcceleration); }

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

	virtual unsigned long Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos = 0)override;

	virtual std::string Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)override;

	void SetPartsParameter(PartsParameters& _base)override;

	void SetMovePow(const float _movePow) { movePow = _movePow; }
	
	void SetRotatePow(const float _rotatePow) { rotatePow = _rotatePow; }

	void SetJumpPow(const float _jumpPow) { jumpPow = _jumpPow; }

public://Get Functions//

	float GetGroundNearPos() { return 0.0f; };

	std::string GetPartsTypeTag()override { return GET_CLASS_NAME(WalkData); }

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

	inline virtual unsigned long Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos = 0)override
	{

		SetParamName(_text.GetTextLine(_textPos));

		return _textPos + 1;
	}

	inline virtual std::string Serialize()override
	{

		std::string res = "";
		res += nextPosName + "\n";

		return res;
	}

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)override;

	void SetPartsParameter(PartsParameters& _base) override{};

	virtual void SetObjectPos(BaseMecha& _base, MechaPartsObject& _parts, ChPtr::Shared<ChCpp::FrameObject> _targetObject) = 0;

	inline void SetParamName(const std::string& _objectName)
	{
		nextPosName = _objectName;
	}

public://Get Functions//

	inline std::string GetObjectName() { return nextPosName; }

protected:

	std::string nextPosName = "";

};

class NextPos : public NextPosBase
{
public://Serialize Deserialize//

	inline unsigned long Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos = 0)override
	{
		unsigned long textPos = NextPosBase::Deserialize(_text, _textPos);
		SetConnectionName(_text.GetTextLine(textPos));

		return textPos + 1;
	}

	inline std::string Serialize()override
	{

		std::string res = NextPosBase::Serialize();
		res += connectionName + "\n";

		return res;
	}

public://Set Functions//

	inline void SetConnectionName(const std::string& _connectionName)
	{
		connectionName = _connectionName;
	}

	void SetObjectPos(BaseMecha& _base, MechaPartsObject& _parts, ChPtr::Shared<ChCpp::FrameObject> _targetObject)override;

public://Get Functions//

	std::string GetPartsTypeTag()override { return GET_CLASS_NAME(NextPos); }

	inline std::string GetConnectionName() { return connectionName; }

protected:

	//接続部の名称//
	std::string connectionName = "";
	
	//最大重量//
	float maxWeight = 0.0f;
};

class BoostBrust :public PartsDataBase
{

public:

	void RemoveParameter(BaseMecha& _base)override;

public://Serialize Deserialize//

	unsigned long Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos = 0)override;

	std::string Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)override;

	void SetBoostData(PartsParameterStruct::BoostData& _boost);

	inline void SetParamName(const std::string& _objectName)
	{
		objectName = _objectName;
	}

	void SetUseEnelgy(const unsigned long _useEnelgy) { useEnelgy = _useEnelgy; }

	void SetBoostPower(const float _boostPower) { boostPower = _boostPower; }

	void SetAvoidUseEnelgy(const unsigned long _avoidUseEnelgy) { avoidUseEnelgy = _avoidUseEnelgy; }

	void SetAvoidPower(const float _avoidPow) { avoidPow = _avoidPow; }

public://Get Functions//

	inline std::string GetObjectNameList() { return objectName; }

	unsigned long GetBoostUseEnelgy()const { return useEnelgy; }

	float GetBoostPower()const { return boostPower; }

	unsigned long GetAvoidUseEnelgy()const { return avoidUseEnelgy; }

	float GetAvoidPower()const { return avoidPow; }

	unsigned long GetAvoidWait()const { return avoidWait; }

	ChPtr::Shared<ChCpp::FrameObject> GetFrame(BaseMecha& _base);

	virtual BaseMecha::InputName GetBoostInputName() = 0;

	virtual BaseMecha::InputName GetAvoidInputName() = 0;

protected:

	std::string objectName;

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

	std::string GetPartsTypeTag()override { return GET_CLASS_NAME(RightBoostBrust); }

};

class LeftBoostBrust :public BoostBrust
{
public:

	void SetPartsParameter(PartsParameters& _base)override;

public://Get Functions//

	inline BaseMecha::InputName GetBoostInputName() override { return BaseMecha::InputName::LeftBoost; }

	inline BaseMecha::InputName GetAvoidInputName() override { return BaseMecha::InputName::LeftAvo; }

	std::string GetPartsTypeTag()override { return GET_CLASS_NAME(LeftBoostBrust); }


};

class FrontBoostBrust :public BoostBrust
{
public:

	void SetPartsParameter(PartsParameters& _base)override;

public://Get Functions//

	inline BaseMecha::InputName GetBoostInputName() override { return BaseMecha::InputName::FrontBoost; }

	inline BaseMecha::InputName GetAvoidInputName() override { return BaseMecha::InputName::FrontAvo; }

	std::string GetPartsTypeTag()override { return GET_CLASS_NAME(FrontBoostBrust); }

};

class BackBoostBrust :public BoostBrust
{
public:

	void SetPartsParameter(PartsParameters& _base)override;

public://Get Functions//

	inline BaseMecha::InputName GetBoostInputName() override { return BaseMecha::InputName::BackBoost; }

	inline BaseMecha::InputName GetAvoidInputName() override { return BaseMecha::InputName::BackAvo; }

	std::string GetPartsTypeTag()override { return GET_CLASS_NAME(BackBoostBrust); }

};

class UpBoostBrust :public BoostBrust
{
public:

	void SetPartsParameter(PartsParameters& _base)override;

public://Get Functions//

	inline BaseMecha::InputName GetBoostInputName() override { return BaseMecha::InputName::UpBoost; }

	inline BaseMecha::InputName GetAvoidInputName() override { return BaseMecha::InputName::UpAvo; }

	std::string GetPartsTypeTag()override { return GET_CLASS_NAME(UpBoostBrust); }

};

class DownBoostBrust :public BoostBrust
{
public:

	void SetPartsParameter(PartsParameters& _base)override;

public://Get Functions//

	inline BaseMecha::InputName GetBoostInputName() override { return BaseMecha::InputName::DownBoost; }

	inline BaseMecha::InputName GetAvoidInputName() override { return BaseMecha::InputName::DownAvo; }

	std::string GetPartsTypeTag()override { return GET_CLASS_NAME(DownBoostBrust); }

};

class WeaponData : public NextPosBase
{
public://Serialize Deserialize//

	virtual unsigned long Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos = 0)override;

	virtual std::string Serialize()override;

public:

	inline void SetWeaponName(const std::string& _weaponName) { weaponName = _weaponName; }

	inline void SetSEFileName(const std::string& _seFile) { seFile = _seFile; }

	inline void SetWaitTime(const unsigned long _waitTime) { waitTime = _waitTime; }

	void SetObjectPos(BaseMecha& _base, MechaPartsObject& _parts, ChPtr::Shared<ChCpp::FrameObject> _targetObject)override;

	void SetWeaponData(PartsParameterStruct::WeaponData& _base);

public:

	inline std::string GetWeaponName() { return weaponName; }

	inline std::string GetSEFileName() { return seFile; }

	inline unsigned long GetWaitTime() const { return waitTime; }

protected:

	//武器の名称//
	std::string weaponName = "";
	//効果音のファイル//
	std::string seFile = "";
	//次の攻撃可能時間//
	unsigned long waitTime = 0;
};

class SwordData :public WeaponData
{
public://Serialize Deserialize//

	unsigned long Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos = 0)override;

	std::string Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)override;

	void SetPartsParameter(PartsParameters& _base)override;

	inline void SetAttackTime(const unsigned long _attackTime) { attackTime = _attackTime; }

public://Get Functions//

	inline std::string GetPartsTypeTag() override { return GET_CLASS_NAME(SwordData); }

	inline unsigned long GetWeatTime() { return attackTime; }

protected:
	
	unsigned long damageParSpeed = 0;

	unsigned long attackTime = 0;

};

class GunData :public WeaponData
{

public://Serialize Deserialize//

	unsigned long Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos = 0)override;

	std::string Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)override;

	void SetPartsParameter(PartsParameters& _base)override;

	inline void SetFireNum(const unsigned long _fireNum) { fireNum = _fireNum; }

	inline void SetBulletNum(const unsigned long _bulletNum) { bulletNum = _bulletNum; }

	inline void SetMagazineNum(const unsigned long _magazineNum) { magazineNum = _magazineNum; }

	inline void SetReloadTime(const unsigned long _reloadTime) { reloadTime = _reloadTime; }

	inline void SetBulletFile(const std::string& _bulletFile) { bulletFile = _bulletFile; }

public://Get Functions//

	inline std::string GetPartsTypeTag() override { return GET_CLASS_NAME(GunData); }

	inline unsigned long GetFireNum()const { return fireNum; }

	inline unsigned long GetBulletNum() const { return bulletNum; }

	inline unsigned long GetMagazineNum() const { return magazineNum; }

	inline unsigned long GetReloadTime() const { return reloadTime; }

	inline unsigned char GetRange() const { return range; }

	inline std::string GetUseBulletFile() const { return bulletFile; }

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

	std::string bulletFile = "";
};

class PostureBase : public PartsDataBase
{
public:

	unsigned long Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos = 0)override;

	std::string Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)override;

	void SetPartsParameter(PartsParameters& _base)override {};

public://Get Functions//

private:

	std::string targetPartsName = "";

};

//垂直//
class VerticalPosture : public PostureBase
{

public:

	std::string GetPartsTypeTag() { return GET_CLASS_NAME(VerticalPosture); }

};

//水平//
class HorizontalPosture : public PostureBase
{

public:

	std::string GetPartsTypeTag() { return GET_CLASS_NAME(HorizontalPosture); }

};

#endif
