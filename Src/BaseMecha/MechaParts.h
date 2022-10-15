#ifndef _BRobParts
#define _BRobParts

class BaseMecha;

class PartsDataBase;
class EnelgyTankData;
class CameraData;
class ScopeData;
class NormalWorlData;
class BoostData;
class SwordData;
class GunData;

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

	void Load(BaseMecha& _base, ID3D11Device* _device, const std::string& _fileName);

	void Deserialize(BaseMecha& _base,ID3D11Device* _device,const std::string& _text);

	std::string Save(const std::string& _fileName);

	std::string Serialize();

public://Set Functions//

	void SetParameters(BaseMecha& _base);

public://Get Function//

	inline float GetHardness() { return hardness; }

	inline float GetMass() { return mass; }

	inline ChD3D11::Mesh11& GetMesh() { return *model; }

private:

	//�p�[�c�̍d��//
	float hardness = 1.0f;

	//�p�[�c�̏d��//
	float mass = 1.0f;

	//�p�[�c�̉����t���O//
	ChStd::Bool releaseFlg = false;

	ChPtr::Shared<ChD3D11::Mesh11>model = ChPtr::Make_S<ChD3D11::Mesh11>();

	static std::map<std::string, std::function<ChPtr::Shared<PartsDataBase>(MechaParts&)>>createFunctions;

	constexpr static const char* GetModelNameTag() { return "ModelName:\n"; }

	constexpr static const char* GetHardnessTag() { return "Hardness:\n"; }

	constexpr static const char* GetMassTag() { return "Mass:\n"; }

};

class PartsDataBase :public ChCpp::BaseComponent
{
public://Serialize Deserialize//

	virtual void Deserialize(const std::string& _text) = 0;

	virtual std::string Serialize() = 0;

public://Set Functions//

	virtual void SetPartsParameter(BaseMecha& _base) = 0;

public://Get Functions//

	virtual std::string GetPartsTypeTag() = 0;

	virtual unsigned long GetPartsLineCount() = 0;
};

class EnelgyTankData : public PartsDataBase
{

public://Serialize Deserialize//

	virtual void Deserialize(const std::string& _text)override;

	virtual std::string Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base)override;

	void SetMaxEnelgy(const unsigned long _maxEnelgy) { maxEnelgy = _maxEnelgy; }

	void SetCreateEnelgy(const unsigned long _createEnelgy) { createEnelgy = _createEnelgy; }

public://Get Functions//

	virtual std::string GetPartsTypeTag()override { return "EnelgyTank:"; }

	unsigned long GetPartsLineCount() override{ return 2; };

protected:

	//�ێ��\�ő�G�l���M�[//
	unsigned long maxEnelgy = 0;

	//�G�l���M�[�̐�����//
	unsigned long createEnelgy = 0;

};

class CameraData : public PartsDataBase
{

public://Serialize Deserialize//

	virtual void Deserialize(const std::string& _text)override;

	virtual std::string Serialize()override;

public://Set Functions//

	virtual void SetPartsParameter(BaseMecha& _base)override;

	void SetFovy(const float _fovy) { fovy = _fovy; }

	void SetCameraObjectName(const std::string& _cameraObject) { cameraObject = _cameraObject; }

	void SetCameraCount(const unsigned long _cameraCount) { cameraCount = _cameraCount; }

public://Get Functions//

	virtual std::string GetPartsTypeTag()override{ return "Camera:"; }

	virtual unsigned long GetPartsLineCount()override { return 3; };

protected:

	//�J�����̎���p//
	float fovy = 60.0f;

	//3DModel�̃J�����̈ʒu
	std::string cameraObject = "";

	//�@�̓��ɕ\�������J�����̐�
	unsigned long cameraCount = 1;
	
};

class ScopeData : public CameraData
{

public://Serialize Deserialize//

	void Deserialize(const std::string& _text)override;

	std::string Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base)override;

	void SetMinFovy(const float _minFovy) { minFovy = _minFovy; }
	
	void SetMaxFovy(const float _maxFovy) { maxFovy = _maxFovy; }

	void SetFovySlideSpeed(const float _fovySlideSpeed) { fovySlideSpeed = _fovySlideSpeed; }

public://Get Functions//

	std::string GetPartsTypeTag()override { return "Scope:"; }

	unsigned long GetPartsLineCount()override { return 3 + CameraData::GetPartsLineCount(); };


protected:

	//�ŏ��̎���p//
	float minFovy = 10.0f;
	
	//�ő�̎���p//
	float maxFovy = 90.0f;

	//����p��ϓ�������X�s�[�h//
	float fovySlideSpeed = 1.0f;

};

class NormalWorkData :public PartsDataBase
{

public://Serialize Deserialize//

	virtual void Deserialize(const std::string& _text)override;

	virtual std::string Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base)override;

	void SetMovePow(const float _movePow) { movePow = _movePow; }
	
	void SetRotatePow(const float _rotatePow) { rotatePow = _rotatePow; }

	void SetJumpPow(const float _jumpPow) { jumpPow = _jumpPow; }

public://Get Functions//

	std::string GetPartsTypeTag()override { return "NormalWork:"; }

	unsigned long GetPartsLineCount()override { return 3; };

protected:
	
	//�ړ���//
	float movePow = 0.0f;
	//��]��//
	float rotatePow = 0.0f;
	//���ė�//
	float jumpPow = 0.0f;

	constexpr static const char* GetSpeedTag() { return "Speed:\n"; }

};

class BoostData : public PartsDataBase
{

public://Serialize Deserialize//

	void Deserialize(const std::string& _text)override;

	std::string Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base)override;

	void SetUseEnelgy(const unsigned long _useEnelgy) { useEnelgy = _useEnelgy; }

	void SetBoostPower(const float _boostPower) { boostPower = _boostPower; }
	
	void SetAvoidUseEnelgy(const unsigned long _avoidUseEnelgy) { avoidUseEnelgy = _avoidUseEnelgy; }

	void SetAvoidPower(const float _avoidPow) { avoidPow = _avoidPow; }

public://Get Functions//

	std::string GetPartsTypeTag() override { return "Boost:"; }

	unsigned long GetPartsLineCount()override { return 4; };

protected:

	//Boost�g�p���̃G�l���M�[�����//
	unsigned long useEnelgy = 0;
	//Boost�g�p���̑O�������ɂ�����ړ���//
	float boostPower = 0.0f;
	//������̃G�l���M�[�����//
	unsigned long avoidUseEnelgy = 0;
	//�������ۂ̈ړ���//
	float avoidPow = 0.0f;

};

class WeaponData : public PartsDataBase
{
public://Serialize Deserialize//

	virtual void Deserialize(const std::string& _text)override;

	virtual std::string Serialize()override;

public:

	void SetWaitTime(const unsigned long _weatTime) { weatTime = _weatTime; }

public:

	virtual unsigned long GetPartsLineCount()override { return 1; };

	std::string GetPartsTypeTag() override { return "WeaponData:"; }

protected:


	//���̍U���\����//
	unsigned long weatTime = 0;
};

class SwordData :public WeaponData
{
public://Serialize Deserialize//

	void Deserialize(const std::string& _text)override;

	std::string Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base)override;

	void SetAttackTime(const unsigned long _attackTime) { attackTime = _attackTime; }

public://Get Functions//

	std::string GetPartsTypeTag() override { return "Sword:"; }

	unsigned long GetPartsLineCount()override { return 1 + WeaponData::GetPartsLineCount(); };

protected:

	unsigned long attackTime = 0;

};

class GunData :public WeaponData
{

public://Serialize Deserialize//

	void Deserialize(const std::string& _text)override;

	std::string Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base)override;

	void SetFireNum(const unsigned long _fireNum) { fireNum = _fireNum; }

	void SetBulletNum(const unsigned long _bulletNum) { bulletNum = _bulletNum; }

	void SetMagazineNum(const unsigned long _magazineNum) { magazineNum = _magazineNum; }

	void SetReloadTime(const unsigned long _reloadTime) { reloadTime = _reloadTime; }

	void SetBulletFile(const std::string& _bulletFile) { bulletFile = _bulletFile; }

public://Get Functions//

	std::string GetPartsTypeTag() override { return "Gun:"; }

	unsigned long GetPartsLineCount()override { return 5 + WeaponData::GetPartsLineCount(); };

protected:

	//�ˌ����̒e�̐�//
	unsigned long fireNum = 1;
	//���̃����[�h�őłĂ鋅�̐�//
	unsigned long bulletNum = 0;
	//�����[�h�ł����//
	unsigned long magazineNum = 0;
	//�����[�h�̂����鎞��//
	unsigned long reloadTime = 0;

	std::string bulletFile = "";

};

#endif
