#pragma once

class BaseMecha;
class AttackObject;

enum class AttackType :unsigned char
{
	Sword,
	Bullet,
	BoostBullet,
	HighExplosive,
	Missile,
};

//�U���S��//
class Attack
{
public:

	class AttackBase
	{
	public:

		virtual void UpdateBulletObject(AttackObject& _bullet) = 0;

		virtual unsigned long Deserialize(ID3D11Device* _device, const ChCpp::TextObject& _text,const unsigned long _nowPos = 0) = 0;

		virtual std::string Serialize() = 0;

	};



	static ChPtr::Shared<Attack> CreateAttackData(MeshDrawer* _drawer, ID3D11Device* _device, const std::string& _fileName);

	static void AllRelease();

public://Serialize Deserialize\\

	virtual void Deserialize(ID3D11Device* _device, const std::string& _text);

	virtual std::string Serialize();

	void CreateBulletData();

	void CrateBoostBulletData();

	void CreateHighExplosiveBulletData();

	void CrateMissileData();

	void CrateSwordData();

public:

	inline void SetMeshDrawer(MeshDrawer* _drawer) { drawer = _drawer; }

public:

	inline unsigned long GetPenetration() { return penetration; }

	inline float GetHitSize() { return hitSize; }

	inline ChCpp::BitBool GetAttackType() { return attackType; }

public:

	virtual void InitBulletObject(const ChLMat& _startMat,AttackObject& _bullet);

	virtual void UpdateBulletObject(AttackObject& _bullet);

	virtual void MoveBulletObject(AttackObject& _bullet);

	void Draw(const ChMat_11& _mat);

public:

	static std::map<std::string,ChPtr::Shared<Attack>>& LoadAttackList()
	{
		static std::map<std::string,ChPtr::Shared<Attack>> ins;
		return ins;
	}

protected:

	//����//
	float firstSpeed = 0.0f;

	//�ђʗ�//
	unsigned long penetration = 0;

	//�Փ˔���p//
	float hitSize = 0.0f;

	//�e�̃��f��//
	ChPtr::Shared<ChD3D11::Mesh11> bullet = ChPtr::Make_S<ChD3D11::Mesh11>();

	ChCpp::BitBool attackType;
	ChPtr::Shared<AttackBase> externulFunction[5] = { nullptr,nullptr,nullptr,nullptr,nullptr };

	MeshDrawer* drawer = nullptr;

private:

};

//�ʏ�e�S��//
class BulletData :public Attack::AttackBase
{
public:

	unsigned long Deserialize(ID3D11Device* _device, const ChCpp::TextObject& _text, const unsigned long _nowPos = 0)override;

	std::string Serialize()override;

	void UpdateBulletObject(AttackObject& _bullet)override;
};

//�u�[�X�^�[�t���̒e�S��//
class BoostBulletData :public Attack::AttackBase
{
public:

	virtual unsigned long Deserialize(ID3D11Device* _device, const ChCpp::TextObject& _text, const unsigned long _nowPos = 0)override;

	virtual std::string Serialize()override;

public:

	void UpdateBulletObject(AttackObject& _bullet)override;


protected:

	//�u�[�X�g�_�΂܂ł̎���//
	unsigned long startBoostTime = 0;

	//�u�[�X�g�̃p���[//
	float boostPow = 0.0f;
};


//�����e�S��//
class HighExplosiveBulletData :public Attack::AttackBase
{
public:

	virtual unsigned long Deserialize(ID3D11Device* _device, const ChCpp::TextObject& _text, const unsigned long _nowPos = 0)override;

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
		
	unsigned long Deserialize(ID3D11Device* _device, const ChCpp::TextObject& _text, const unsigned long _nowPos = 0)override;

	std::string Serialize()override;

public:

	void UpdateBulletObject(AttackObject& _bullet)override;

protected:

	//��]���x//
	float rotateSpeed = 0.0f;

	//�������͈�//
	float lostRange = 0.0f;


};
