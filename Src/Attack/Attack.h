#pragma once

class BaseMecha;
class AttackObject;

enum class AttackType :unsigned char
{
	Sword,//��//
	Bullet,//�ʏ�e//
	BoostBullet,//�u�[�X�g�t���e//
	HighExplosive,//�����e//
	Missile,//�ǔ��e//

	Explosive,//����//
};

//�U���S��//
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

	//�З�//
	unsigned long penetration = 0;

	//�Փ˔���p//
	float hitSize = 0.0f;

	//����//
	float displayFirstSpeed = 0.0f;

	//�u�[�X�g�_�΂܂ł̎���//
	unsigned long displayStartBoostTime = 0;

	//�u�[�X�g����//
	unsigned long displayUseBoostTime = 0;

	//�u�[�X�g�̃p���[//
	float displayBoostPow = 0.0f;

	//�����͈�//
	float displayBlastRange = 0.0f;

	//��]���x//
	float displayRotateSpeed = 0.0f;

	//�������͈�//
	float displayLostRange = 0.0f;

	AttackType attackType = AttackType::Sword;

	//�e�̃��f��//
	ChPtr::Shared<ChD3D11::Mesh11> bullet = ChPtr::Make_S<ChD3D11::Mesh11>();

	std::vector<ChPtr::Shared<AttackBase>> externulFunctions;

	ChD3D11::Shader::BaseDrawMesh11* drawer = nullptr;

private:

};

//�ʏ�e�S��//
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

	//����//
	float firstSpeed = 0.0f;

};

//�u�[�X�^�[�t���̒e�S��//
class BoostBulletData :public Attack::AttackBase
{
public:

	virtual unsigned long Deserialize(ID3D11Device* _device, Attack& _attack, const ChCpp::TextObject& _text, const unsigned long _nowPos = 0)override;

	virtual std::string Serialize()override;

public:

	void UpdateBulletObject(AttackObject& _bullet)override;


protected:

	//�u�[�X�g�_�΂܂ł̎���//
	unsigned long startBoostTime = 0;

	//�u�[�X�g����//
	unsigned long useBoostTime = 0;

	//�u�[�X�g�̃p���[//
	float boostPow = 0.0f;
};


//�����e�S��//
class HighExplosiveBulletData :public Attack::AttackBase
{
public:

	virtual unsigned long Deserialize(ID3D11Device* _device, Attack& _attack, const ChCpp::TextObject& _text, const unsigned long _nowPos = 0)override;

	virtual std::string Serialize()override;

public:
	
	void UpdateBulletObject(AttackObject& _bullet)override;

protected:

	//�����͈�//
	float blastRange = 0.0f;

};

//�U���e�S��//
class  MissileData :public Attack::AttackBase
{
public://Serialize Deserialize\\
		
	unsigned long Deserialize(ID3D11Device* _device, Attack& _attack, const ChCpp::TextObject& _text, const unsigned long _nowPos = 0)override;

	std::string Serialize()override;

public:

	void UpdateBulletObject(AttackObject& _bullet)override;

protected:

	//��]���x//
	float rotateSpeed = 0.0f;

	//�������͈�//
	float lostRange = 0.0f;


};
