#ifndef _BRobParts
#define _BRobParts

class EnelgyTank;
class Camera;
class Scope;
class NormalWorl;
class Boost;
class Sword;
class Gun;

class MechaParts :public ChCpp::BaseObject
{
public://Override Functions//

	void Update()override;

	void Move()override;
	
private:

	unsigned long CreateEnelgyTank(ChCpp::TextObject& _textObject,unsigned long _linePos);
	unsigned long CreateCamera(ChCpp::TextObject& _textObject, unsigned long _linePos);
	unsigned long CreateScope(ChCpp::TextObject& _textObject, unsigned long _linePos);
	unsigned long CreateNormalWork(ChCpp::TextObject& _textObject, unsigned long _linePos);
	unsigned long CreateBoost(ChCpp::TextObject& _textObject, unsigned long _linePos);
	unsigned long CreateSword(ChCpp::TextObject& _textObject, unsigned long _linePos);
	unsigned long CreateGun(ChCpp::TextObject& _textObject, unsigned long _linePos);

public://Serialize Deserialize//

	void Deserialize(const std::string& _text);

	std::string Serialize();

public://Set Function//

	inline void SetDrawer(ChD3D11::Shader::BaseDrawMesh11*) { meshDrawer = meshDrawer; };

public://Get Function//

	inline float GetHardness() { return hardness; }

	inline float GetMass() { return mass; }

private:

	//�p�[�c�̍d��//
	float hardness = 1.0f;

	//�p�[�c�̏d��//
	float mass = 1.0f;

	//�p�[�c�̉����t���O//
	ChStd::Bool releaseFlg = false;

	ChD3D11::Shader::BaseDrawMesh11* meshDrawer = nullptr;

	ChPtr::Shared<ChD3D11::Mesh11>model = ChPtr::Make_S<ChD3D11::Mesh11>();

	constexpr static const char* GetModelNameTag() { return "ModelName:\n"; }

	constexpr static const char* GetHardnessTag() { return "Hardness:\n"; }

	constexpr static const char* GetMassTag() { return "Mass:\n"; }

};

class PartsFunctionBase :public ChCpp::BaseComponent
{
public://Serialize Deserialize//

	virtual void Deserialize(const std::string& _text) = 0;

	virtual std::string Serialize() = 0;

public://Get Functions//

	virtual std::string GetPartsTypeTag() = 0;

};

class EnelgyTank : public PartsFunctionBase
{
public://Override Functions//

	void Update()override;

	void Move()override;

public://Serialize Deserialize//

	virtual void Deserialize(const std::string& _text)override;

	virtual std::string Serialize()override;

public://Get Functions//

	virtual std::string GetPartsTypeTag()override { return "EnelgyTank:"; }

protected:

	//�ێ��\�ő�G�l���M�[//
	unsigned long maxEnelgy = 0;

	//�G�l���M�[�̐�����//
	unsigned long createEnelgy = 0;

	//���݂̃G�l���M�[�c��//
	unsigned long enelgy = 0;

};

class Camera : public PartsFunctionBase
{
public://Override Functions//

	void Update()override;

	void Move()override;

public://Serialize Deserialize//

	virtual void Deserialize(const std::string& _text)override;

	virtual std::string Serialize()override;

public://Get Functions//

	virtual std::string GetPartsTypeTag()override{ return "Camera:"; }

protected:

	//�J�����̎���p//
	float fovy = 60.0f;

	//3DModel�̃J�����̈ʒu
	ChPtr::Shared<ChCpp::BaseObject>cameraObject = nullptr;

	//�@�̓��ɕ\�������J�����̐�
	long cameraCount = 1;
	
};

class Scope : public PartsFunctionBase
{
public://Override Functions//

	void Update()override;

	void Move()override;

public://Serialize Deserialize//

	void Deserialize(const std::string& _text)override;

	std::string Serialize()override;

public://Get Functions//

	std::string GetPartsTypeTag()override { return "Scope:"; }


protected:

	//�ŏ��̎���p//
	float minFovy = 10.0f;
	
	//�ő�̎���p//
	float maxFovy = 90.0f;

	//����p��ϓ�������X�s�[�h//
	float fovySlideSpeed = 1.0f;

};

class NormalWork :public PartsFunctionBase
{
public://Override Functions//

	void Update()override;

	void Move()override;

public://Serialize Deserialize//

	virtual void Deserialize(const std::string& _text)override;

	virtual std::string Serialize()override;

public://Get Functions//

	std::string GetPartsTypeTag()override { return "NormalWork:"; }

protected:
	
	//�ړ���//
	float movePow = 0.0f;
	//��]��//
	float rotatePow = 0.0f;
	//���ė�//
	float jumpPow = 0.0f;

	constexpr static const char* GetSpeedTag() { return "Speed:\n"; }

};

class Boost : public PartsFunctionBase
{
public://Override Functions//

	void Update()override;

	void Move()override;

public://Serialize Deserialize//

	void Deserialize(const std::string& _text)override;

	std::string Serialize()override;

public://Get Functions//

	std::string GetPartsTypeTag() override { return "Boost:"; }

protected:

	ChStd::Bool useFlg = false;
	//Boost�g�p���̃G�l���M�[�����//
	unsigned long useEnelgy = 0.0f;
	//Boost�g�p���̑O�������ɂ�����ړ���//
	float boostPower = 0.0f;
	//�������ۂ̈ړ���//
	float avoidPow = 0.0f;
	//������̃G�l���M�[�����//
	unsigned long avoidUseEnelgy = 0.0f;

};

class Weapon : public PartsFunctionBase
{
public://Override Functions//

	void Update()override;

	void Move()override;

public:

	void Attack();

protected:


	ChStd::Bool attackFlg = false;

	//���̍U���\����//
	unsigned long weatTime = 0;
	//���̍U���\�c�莞��//
	unsigned long weatCount = 0;
};

class Sword :public Weapon
{
public://Override Functions//

	void Update()override;

	void Move()override;

public://Serialize Deserialize//

	void Deserialize(const std::string& _text)override;

	std::string Serialize()override;

public://Get Functions//

	std::string GetPartsTypeTag() override { return "Sword:"; }

public:

	void Update()override;

	void Move()override;

protected:

	unsigned long attackTime = 0.0f;

};

class Gun :public Weapon
{
public://Override Functions//

	void Update()override;

	void Move()override;

public://Serialize Deserialize//

	void Deserialize(const std::string& _text)override;

	std::string Serialize()override;

public://Get Functions//

	std::string GetPartsTypeTag() override { return "Gun:"; }

public:

	void Update()override;

	void Move()override;

protected:

	//�ˌ����̒e�̐�//
	unsigned long fireNum = 1;
	//���̃����[�h�őłĂ鋅�̐�//
	unsigned long bulletNum = 0;
	//�����[�h�ł����//
	unsigned long magazineNum = 0;
	//�����[�h�̂����鎞��//
	unsigned long reloadTime = 0;

	//���݂̒e�q���e��//
	unsigned long bulletCount = 0;
	//���݂̒e�q��//
	unsigned long magazineCount = 0;
	//���݂̃����[�h�o�ߎ���//
	unsigned long reLoadCount = 0.0f;

	std::string bulletFile = "";

	//�e�̎��//
	unsigned char bulletType = 0;
	//�e��//
	ChPtr::Shared<ChD3D11::Mesh11>bullet = ChPtr::Make_S<ChD3D11::Mesh11>();

};

#endif
