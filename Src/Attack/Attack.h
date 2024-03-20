#pragma once

class BaseMecha;
class AttackObject;

enum class AttackType :unsigned char
{
	Sword,//剣//
	Bullet,//通常弾//
	BoostBullet,//ブースト付き弾//
	HighExplosive,//爆発弾//
	Missile,//追尾弾//

	Explosive,//爆発//
};

//攻撃全般//
class Attack
{

public:

	class AttackBase
	{
	public:

		virtual unsigned long Deserialize(ID3D11Device* _device, Attack& _attack,const ChCpp::TextObject& _text,const unsigned long _nowPos = 0) = 0;

		virtual std::string Serialize() = 0;

		virtual void InitBulletObject(const ChLMat& _startMat, AttackObject& _bullet){}

		virtual void UpdateBulletObject(AttackObject& _bullet) {}

	};

	static ChPtr::Shared<Attack> CreateAttackData(ChD3D11::Shader::BaseDrawMesh11* _drawer, ID3D11Device* _device, const std::string& _fileName);

	static void AllRelease();

public://Serialize Deserialize\\

	virtual void Deserialize(ID3D11Device* _device, const std::string& _text);

	virtual std::string Serialize();

public: 

	inline void SetMeshDrawer(ChD3D11::Shader::BaseDrawMesh11* _drawer) { drawer = _drawer; }

public:

	inline unsigned long GetPenetration() { return penetration; }

	inline float GetHitSize() { return hitSize; }

	inline float GetDisplayFirstSpeed() { return displayFirstSpeed; }

	inline unsigned long GetDisplayStartBoostTime() { return displayStartBoostTime; }

	inline unsigned long GetDisplayUseBoostTime() { return displayUseBoostTime; }

	inline float GetDisplayBoostPow() { return displayBoostPow; }

	inline float GetDisplayBlastRange() { return displayBlastRange; }

	inline float GetDisplayRotateSpeed() { return displayRotateSpeed; }

	inline float GetDisplayLostRange() { return displayLostRange; }

	unsigned char GetAttackType() { return ChStd::EnumCast(attackType); }

public:

	inline void AddDisplayFirstSpeed(float _speed) { displayFirstSpeed += _speed; }

	inline void AddDisplayStartBoostTime(unsigned long _boostTime) { displayStartBoostTime += _boostTime; }

	inline void AddDisplayUseBoostTime(unsigned long _boostTime) { displayUseBoostTime += _boostTime; }

	inline void AddDisplayBoostPow(float _speed) { displayBoostPow += _speed; }
	
	inline void AddDisplayBlastRange(float _speed) { displayBlastRange += _speed; }

	inline void AddDisplayRotateSpeed(float _speed) { displayRotateSpeed += _speed; }

	inline void AddDisplayLostRange(float _speed) { displayLostRange += _speed; }

public:

	void InitBulletObject(const ChLMat& _startMat,AttackObject& _bullet);

	void UpdateBulletObject(AttackObject& _bullet);

	void MoveBulletObject(AttackObject& _bullet);

	void Draw(const ChMat_11& _mat);

public:

	static std::map<std::string,ChPtr::Shared<Attack>>& LoadAttackList()
	{
		static std::map<std::string,ChPtr::Shared<Attack>> ins;
		return ins;
	}

protected:

	//威力//
	unsigned long penetration = 0;

	//衝突判定用//
	float hitSize = 0.0f;

	//初速//
	float displayFirstSpeed = 0.0f;

	//ブースト点火までの時間//
	unsigned long displayStartBoostTime = 0;

	//ブースト時間//
	unsigned long displayUseBoostTime = 0;

	//ブーストのパワー//
	float displayBoostPow = 0.0f;

	//爆発範囲//
	float displayBlastRange = 0.0f;

	//回転速度//
	float displayRotateSpeed = 0.0f;

	//見失う範囲//
	float displayLostRange = 0.0f;

	AttackType attackType = AttackType::Sword;

	//弾のモデル//
	ChPtr::Shared<ChD3D11::Mesh11> bullet = ChPtr::Make_S<ChD3D11::Mesh11>();

	std::vector<ChPtr::Shared<AttackBase>> externulFunctions;

	ChD3D11::Shader::BaseDrawMesh11* drawer = nullptr;

private:

};

//通常弾全般//
class BulletData :public Attack::AttackBase
{
public:

	unsigned long Deserialize(ID3D11Device* _device, Attack& _attack, const ChCpp::TextObject& _text, const unsigned long _nowPos = 0)override;

	std::string Serialize()override;

	void InitBulletObject(const ChLMat& _startMat, AttackObject& _bullet)override;

	void UpdateBulletObject(AttackObject& _bullet)override;

public:

	float GetFirstSpeed() { return firstSpeed; }

private:

	//初速//
	float firstSpeed = 0.0f;

};

//ブースター付きの弾全般//
class BoostBulletData :public Attack::AttackBase
{
public:

	virtual unsigned long Deserialize(ID3D11Device* _device, Attack& _attack, const ChCpp::TextObject& _text, const unsigned long _nowPos = 0)override;

	virtual std::string Serialize()override;

public:

	void UpdateBulletObject(AttackObject& _bullet)override;


protected:

	//ブースト点火までの時間//
	unsigned long startBoostTime = 0;

	//ブースト時間//
	unsigned long useBoostTime = 0;

	//ブーストのパワー//
	float boostPow = 0.0f;
};


//爆発弾全般//
class HighExplosiveBulletData :public Attack::AttackBase
{
public:

	virtual unsigned long Deserialize(ID3D11Device* _device, Attack& _attack, const ChCpp::TextObject& _text, const unsigned long _nowPos = 0)override;

	virtual std::string Serialize()override;

public:
	
	void UpdateBulletObject(AttackObject& _bullet)override;

protected:

	//爆発範囲//
	float blastRange = 0.0f;

};

//誘導弾全般//
class  MissileData :public Attack::AttackBase
{
public://Serialize Deserialize\\
		
	unsigned long Deserialize(ID3D11Device* _device, Attack& _attack, const ChCpp::TextObject& _text, const unsigned long _nowPos = 0)override;

	std::string Serialize()override;

public:

	void UpdateBulletObject(AttackObject& _bullet)override;

protected:

	//回転速度//
	float rotateSpeed = 0.0f;

	//見失う範囲//
	float lostRange = 0.0f;


};
