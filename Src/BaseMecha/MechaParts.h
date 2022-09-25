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

	//パーツの硬さ//
	float hardness = 1.0f;

	//パーツの重さ//
	float mass = 1.0f;

	//パーツの解除フラグ//
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

	//保持可能最大エネルギー//
	unsigned long maxEnelgy = 0;

	//エネルギーの生成量//
	unsigned long createEnelgy = 0;

	//現在のエネルギー残量//
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

	//カメラの視野角//
	float fovy = 60.0f;

	//3DModelのカメラの位置
	ChPtr::Shared<ChCpp::BaseObject>cameraObject = nullptr;

	//機体内に表示されるカメラの数
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

	//最小の視野角//
	float minFovy = 10.0f;
	
	//最大の視野角//
	float maxFovy = 90.0f;

	//視野角を変動させるスピード//
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
	
	//移動力//
	float movePow = 0.0f;
	//回転力//
	float rotatePow = 0.0f;
	//飛翔力//
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
	//Boost使用時のエネルギー消費量//
	unsigned long useEnelgy = 0.0f;
	//Boost使用時の前方方向における移動力//
	float boostPower = 0.0f;
	//回避する際の移動力//
	float avoidPow = 0.0f;
	//回避時のエネルギー消費量//
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

	//次の攻撃可能時間//
	unsigned long weatTime = 0;
	//次の攻撃可能残り時間//
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

	//射撃時の弾の数//
	unsigned long fireNum = 1;
	//一回のリロードで打てる球の数//
	unsigned long bulletNum = 0;
	//リロードできる回数//
	unsigned long magazineNum = 0;
	//リロードのかかる時間//
	unsigned long reloadTime = 0;

	//現在の弾倉内弾薬数//
	unsigned long bulletCount = 0;
	//現在の弾倉数//
	unsigned long magazineCount = 0;
	//現在のリロード経過時間//
	unsigned long reLoadCount = 0.0f;

	std::string bulletFile = "";

	//弾の種類//
	unsigned char bulletType = 0;
	//弾速//
	ChPtr::Shared<ChD3D11::Mesh11>bullet = ChPtr::Make_S<ChD3D11::Mesh11>();

};

#endif
