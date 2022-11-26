#ifndef _BRobParts
#define _BRobParts

#include"BaseMecha.h"

class MechaPartsObject;
class CameraObject;
class WeaponObject;
class PartsDataBase;

class MechaParts :public ChCpp::BaseObject
{
public:

	static std::map<std::string,ChPtr::Shared<MechaParts>>& LoadPartsList()
	{
		static std::map<std::string,ChPtr::Shared<MechaParts>>ins;
		return ins;
	}

private:

	unsigned long CreateDatas(BaseMecha& _base, ChCpp::TextObject& _textObject, unsigned long _linePos);

public://Serialize Deserialize//

	static ChPtr::Shared<MechaPartsObject> LoadParts(BaseMecha& _base, ID3D11Device* _device, const std::string& _fileName);

	void Load(BaseMecha& _base, ID3D11Device* _device, const std::string& _fileName);

	void Deserialize(BaseMecha& _base,ID3D11Device* _device,const std::string& _text);

	std::string Save(const std::string& _fileName);

	std::string Serialize();

public://Set Functions//

	ChPtr::Shared<MechaPartsObject>  SetParameters(BaseMecha& _base);

	virtual ChPtr::Shared<MechaPartsObject>  SetPartsParameter(BaseMecha& _base);

	void SetHardness(const float _hardness) { hardness = _hardness; }

	void SetMass(const float _mass) { mass = _mass; }

	ChStd::Bool SetPosition(BaseMecha& _base, ChPtr::Shared<MechaPartsObject> _obj, const BaseMecha::PartsPosNames _name);

public://Get Function//

	inline float GetHardness() { return hardness; }

	inline float GetMass() { return mass; }

	inline float GetGroundHeight() { return groundHeight; }

	inline ChD3D11::Mesh11& GetMesh() { return *model; }

	inline std::string GetThisFileName() { return thisFileName; }

private:

	//パーツの硬さ//
	float hardness = 1.0f;

	//パーツの重さ//
	float mass = 1.0f;

	std::string thisFileName = "";

	float groundHeight = 0.0f;

	ChPtr::Shared<ChD3D11::Mesh11>model = ChPtr::Make_S<ChD3D11::Mesh11>();

	static std::map<std::string, std::function<ChPtr::Shared<PartsDataBase>(MechaParts&)>>createFunctions;

	constexpr static const char* GetModelNameTag() { return "ModelName:\n"; }

	constexpr static const char* GetHardnessTag() { return "Hardness:\n"; }

	constexpr static const char* GetMassTag() { return "Mass:\n"; }

};

class PartsDataBase :public ChCpp::BaseComponent
{
public://Serialize Deserialize//

	virtual unsigned long Deserialize(const ChCpp::TextObject& _text,const unsigned long _textPos = 0) = 0;

	virtual std::string Serialize() = 0;

public://Set Functions//

	virtual void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts) = 0;


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

public://Serialize Deserialize//

	virtual unsigned long Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos = 0)override;

	virtual std::string Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts)override;

	void SetMaxEnelgy(const unsigned long _maxEnelgy) { maxEnelgy = _maxEnelgy; }

	void SetCreateEnelgy(const unsigned long _createEnelgy) { createEnelgy = _createEnelgy; }

public://Get Functions//

	virtual std::string GetPartsTypeTag()override { return "EnelgyTank:"; }

protected:

	//保持可能最大エネルギー//
	unsigned long maxEnelgy = 0;

	//エネルギーの生成量//
	unsigned long createEnelgy = 0;

};

class CameraData : public PartsDataBase
{

public://Serialize Deserialize//

	virtual unsigned long Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos = 0)override;

	virtual std::string Serialize()override;

public://Set Functions//

	virtual void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts)override;

	void SetFovy(const float _fovy) { fovy = _fovy; }

	void SetCameraObjectName(const std::string& _cameraObject) { cameraObject = _cameraObject; }

	void SetCameraCount(const unsigned long _cameraCount) { cameraCount = _cameraCount; }

public://Get Functions//

	virtual std::string GetPartsTypeTag()override{ return "Camera:"; }

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

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts)override;

	void SetMinFovy(const float _minFovy) { minFovy = _minFovy; }
	
	void SetMaxFovy(const float _maxFovy) { maxFovy = _maxFovy; }

	void SetFovySlideSpeed(const float _fovySlideSpeed) { fovySlideSpeed = _fovySlideSpeed; }

public://Get Functions//

	std::string GetPartsTypeTag()override { return "Scope:"; }

protected:

	//最小の視野角//
	float minFovy = 10.0f;
	
	//最大の視野角//
	float maxFovy = 90.0f;

	//視野角を変動させるスピード//
	float fovySlideSpeed = 1.0f;

};

class WalkData :public PartsDataBase
{

public://Serialize Deserialize//

	virtual unsigned long Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos = 0)override;

	virtual std::string Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts)override;

	void SetMovePow(const float _movePow) { movePow = _movePow; }
	
	void SetRotatePow(const float _rotatePow) { rotatePow = _rotatePow; }

	void SetJumpPow(const float _jumpPow) { jumpPow = _jumpPow; }

public://Get Functions//

	float GetGroundNearPos() { return 0.0f; };

	std::string GetPartsTypeTag()override { return "Walk:"; }

protected:
	
	//移動力//
	float movePow = 0.0f;
	//回転力//
	float rotatePow = 0.0f;
	//飛翔力//
	float jumpPow = 0.0f;

	constexpr static const char* GetSpeedTag() { return "Speed:\n"; }

};

class WeaponData : public PartsDataBase
{
public://Serialize Deserialize//

	virtual unsigned long Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos = 0)override;

	virtual std::string Serialize()override;

public:

	inline void SetWaitTime(const unsigned long _weatTime) { weatTime = _weatTime; }

public:

	inline std::string GetPartsTypeTag() override { return "WeaponData:"; }

	inline unsigned long GetWeatTime() { return weatTime; }

protected:


	//次の攻撃可能時間//
	unsigned long weatTime = 0;
};

class SwordData :public WeaponData
{
public://Serialize Deserialize//

	unsigned long Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos = 0)override;

	std::string Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts)override;

	inline void SetAttackTime(const unsigned long _attackTime) { attackTime = _attackTime; }

public://Get Functions//

	inline std::string GetPartsTypeTag() override { return "Sword:"; }

	inline unsigned long GetWeatTime() { return attackTime; }

protected:

	unsigned long attackTime = 0;

};

class GunData :public WeaponData
{

public://Serialize Deserialize//

	unsigned long Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos = 0)override;

	std::string Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts)override;

	inline void SetFireNum(const unsigned long _fireNum) { fireNum = _fireNum; }

	inline void SetBulletNum(const unsigned long _bulletNum) { bulletNum = _bulletNum; }

	inline void SetMagazineNum(const unsigned long _magazineNum) { magazineNum = _magazineNum; }

	inline void SetReloadTime(const unsigned long _reloadTime) { reloadTime = _reloadTime; }

	inline void SetBulletFile(const std::string& _bulletFile) { bulletFile = _bulletFile; }

public://Get Functions//

	inline std::string GetPartsTypeTag() override { return "Gun:"; }

	inline unsigned long GetFireNum() { return fireNum; }

	inline unsigned long GetBulletNum() { return bulletNum; }

	inline unsigned long GetMagazineNum() { return magazineNum; }

	inline unsigned long GetReloadTime() { return reloadTime; }

	inline std::string GetUseBulletFile() { return bulletFile; }

protected:

	//射撃時の弾の数//
	unsigned long fireNum = 1;
	//一回のリロードで打てる球の数//
	unsigned long bulletNum = 0;
	//リロードできる回数//
	unsigned long magazineNum = 0;
	//リロードのかかる時間//
	unsigned long reloadTime = 0;

	std::string bulletFile = "";

};


class NextPosBase : public PartsDataBase
{
public://Serialize Deserialize//

	inline unsigned long Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos = 0)override
	{

		SetParamName(_text.GetTextLine(_textPos));

		return _textPos + 1;
	}

	inline std::string Serialize()override
	{

		std::string res = "";
		res += nextPosName + "\n";

		return res;
	}

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts)override;

	virtual void SetObjectPos(BaseMecha& _base, ChPtr::Shared<ChCpp::FrameObject> _targetObject) = 0;

	inline void SetParamName(const std::string& _objectName)
	{
		nextPosName = _objectName;
	}

public://Get Functions//

	inline std::string GetObjectName() { return nextPosName; }

protected:

	std::string nextPosName = "";

};

class RightArmPos:public NextPosBase
{
public:

	void SetObjectPos(BaseMecha& _base, ChPtr::Shared<ChCpp::FrameObject> _targetObject)override;

public://Get Functions//

	std::string GetPartsTypeTag()override { return "RightArmPos:"; }

};

class LeftArmPos :public NextPosBase 
{
public:

	void SetObjectPos(BaseMecha& _base, ChPtr::Shared<ChCpp::FrameObject> _targetObject)override;

public://Get Functions//

	std::string GetPartsTypeTag()override { return "LeftArmPos:"; }

};

class FootPos :public NextPosBase 
{
public:

	void SetObjectPos(BaseMecha& _base, ChPtr::Shared<ChCpp::FrameObject> _targetObject)override;

public://Get Functions//

	std::string GetPartsTypeTag()override { return "FootPos:"; }

};

class HeadPos :public NextPosBase 
{
public:

	void SetObjectPos(BaseMecha& _base, ChPtr::Shared<ChCpp::FrameObject> _targetObject)override;

public://Get Functions//

	std::string GetPartsTypeTag()override { return "HeadPos:"; }

};

class BoostPos :public NextPosBase 
{
public:

	void SetObjectPos(BaseMecha& _base, ChPtr::Shared<ChCpp::FrameObject> _targetObject)override;

public://Get Functions//

	std::string GetPartsTypeTag()override { return "BoostPos:"; }

};

class BoostBrust :public PartsDataBase
{

public://Serialize Deserialize//

	unsigned long Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos = 0)override;

	std::string Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts)override;

	inline void SetParamName(const std::string& _objectName)
	{
		objectName = _objectName;
	}

	void SetUseEnelgy(const unsigned long _useEnelgy) { useEnelgy = _useEnelgy; }

	void SetBoostPower(const float _boostPower) { boostPower = _boostPower; }

	void SetAvoidUseEnelgy(const unsigned long _avoidUseEnelgy) { avoidUseEnelgy = _avoidUseEnelgy; }

	void SetAvoidPower(const float _avoidPow) { avoidPow = _avoidPow; }

	virtual void SetPartsObject(BaseMecha& _base , ChPtr::Shared<ChCpp::FrameObject> _boostObject) = 0;

public://Get Functions//

	inline std::string GetObjectNameList() { return objectName; }

public:

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

	void SetPartsObject(BaseMecha& _base, ChPtr::Shared<ChCpp::FrameObject> _boostObject)override;

public://Get Functions//

	std::string GetPartsTypeTag()override { return "RightBoostBrust:"; }


};

class LeftBoostBrust :public BoostBrust
{

public:

	void SetPartsObject(BaseMecha& _base, ChPtr::Shared<ChCpp::FrameObject> _boostObject)override;

public://Get Functions//

	std::string GetPartsTypeTag()override { return "LeftBoostBrust:"; }


};

class FrontBoostBrust :public BoostBrust
{

public:

	void SetPartsObject(BaseMecha& _base, ChPtr::Shared<ChCpp::FrameObject> _boostObject)override;

public://Get Functions//

	std::string GetPartsTypeTag()override { return "FrontBoostBrust:"; }

};

class BackBoostBrust :public BoostBrust
{

public:

	void SetPartsObject(BaseMecha& _base, ChPtr::Shared<ChCpp::FrameObject> _boostObject)override;

public://Get Functions//

	std::string GetPartsTypeTag()override { return "BackBoostBrust:"; }

};

class UpBoostBrust :public BoostBrust
{
public:

	void SetPartsObject(BaseMecha& _base, ChPtr::Shared<ChCpp::FrameObject> _boostObject)override;

public://Get Functions//

	std::string GetPartsTypeTag()override { return "UpBoostBrust:"; }

};

class DownBoostBrust :public BoostBrust
{
public:

	void SetPartsObject(BaseMecha& _base, ChPtr::Shared<ChCpp::FrameObject> _boostObject)override;

public://Get Functions//

	std::string GetPartsTypeTag()override { return "DownBoostBrust:"; }

};

class WeaponPosBase : public PartsDataBase
{
public://Serialize Deserialize//

	inline unsigned long Deserialize(const ChCpp::TextObject& _text, const unsigned long _textPos = 0)override
	{

		SetParamName(_text.GetTextLine(_textPos));

		return _textPos + 1;
	}

	inline std::string Serialize()override
	{

		std::string res = "";
		res += nextPosName + "\n";

		return res;
	}

public://Set Functions//

	inline void SetParamName(const std::string& _objectName)
	{
		nextPosName = _objectName;
	}

public://Get Functions//

	inline std::string GetObjectName() { return nextPosName; }

protected:

	std::string nextPosName = "";

};

class HaveRightWeaponPos :public WeaponPosBase
{
public:

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts)override;

public://Get Functions//

	std::string GetPartsTypeTag()override { return "HaveRightWeaponPos:"; }

};

class HaveLeftWeaponPos :public WeaponPosBase
{
public:

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts)override;

public://Get Functions//

	std::string GetPartsTypeTag()override { return "HaveLeftWeaponPos:"; }

};

class ShotPos :public WeaponPosBase
{
public:

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts)override;

public://Get Functions//

	std::string GetPartsTypeTag()override { return "ShotPos:"; }

};

#endif
