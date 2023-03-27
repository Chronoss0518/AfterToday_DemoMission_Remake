#pragma once

class BaseMecha;
class BulletObject;

enum class BulletType :unsigned char
{
	Bullet,
	BoostBullet,
	HighExplosive,
	Missile,
};

//�ʏ�e�S��//
class Bullet
{
public:

	class ExternulBulletFunctionBase
	{
	public:

		virtual void UpdateBulletObject(BulletObject& _bullet) = 0;

		virtual unsigned long Deserialize(ID3D11Device* _device, const ChCpp::TextObject& _text,const unsigned long _nowPos = 0) = 0;

		virtual std::string Serialize() = 0;

	};



	static ChPtr::Shared<Bullet> CreateBullet(MeshDrawer* _drawer, ID3D11Device* _device, const std::string& _fileName);

	static void AllRelease();

public://Serialize Deserialize\\

	virtual void Deserialize(ID3D11Device* _device, const std::string& _text);

	virtual std::string Serialize();

	void CreateBulletData();

	void CrateBoostBulletData();

	void CreateHighExplosiveBulletData();

	void CrateMissileData();

public:

	inline void SetMeshDrawer(MeshDrawer* _drawer) { drawer = _drawer; }

public:

	inline unsigned long GetPenetration() { return penetration; }

	inline float GetHitSize() { return hitSize; }

public:

	virtual void InitBulletObject(const ChLMat& _startMat,BulletObject& _bullet);

	virtual void UpdateBulletObject(BulletObject& _bullet);

	virtual void MoveBulletObject(BulletObject& _bullet);

	void Draw(const ChMat_11& _mat);

public:

	static std::map<std::string,ChPtr::Shared<Bullet>>& LoadBulletList()
	{
		static std::map<std::string,ChPtr::Shared<Bullet>> ins;
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

	unsigned char bulletType = 0;
	ChPtr::Shared<ExternulBulletFunctionBase> externulFunction[4] = { nullptr,nullptr,nullptr,nullptr };

	MeshDrawer* drawer = nullptr;

private:

};

//�ʏ�e�S��//
class BulletData :public Bullet::ExternulBulletFunctionBase
{
public:

	unsigned long Deserialize(ID3D11Device* _device, const ChCpp::TextObject& _text, const unsigned long _nowPos = 0)override;

	std::string Serialize()override;

	void UpdateBulletObject(BulletObject& _bullet)override;
};

//�u�[�X�^�[�t���̒e�S��//
class BoostBulletData :public Bullet::ExternulBulletFunctionBase
{
public:

	virtual unsigned long Deserialize(ID3D11Device* _device, const ChCpp::TextObject& _text, const unsigned long _nowPos = 0)override;

	virtual std::string Serialize()override;

public:

	void UpdateBulletObject(BulletObject& _bullet)override;


protected:

	//�u�[�X�g�_�΂܂ł̎���//
	unsigned long startBoostTime = 0;

	//�u�[�X�g�̃p���[//
	float boostPow = 0.0f;
};


//�����e�S��//
class HighExplosiveBulletData :public Bullet::ExternulBulletFunctionBase
{
public:

	virtual unsigned long Deserialize(ID3D11Device* _device, const ChCpp::TextObject& _text, const unsigned long _nowPos = 0)override;

	virtual std::string Serialize()override;

public:
	
	void UpdateBulletObject(BulletObject& _bullet)override;

protected:

	//�����͈�//
	float blastRange = 0.0f;

};

//�U���e�S��//
class  MissileData :public Bullet::ExternulBulletFunctionBase
{
public://Serialize Deserialize\\
		
	unsigned long Deserialize(ID3D11Device* _device, const ChCpp::TextObject& _text, const unsigned long _nowPos = 0)override;

	std::string Serialize()override;

public:

	void UpdateBulletObject(BulletObject& _bullet)override;

protected:

	//��]���x//
	float rotateSpeed = 0.0f;

	//�������͈�//
	float lostRange = 0.0f;


};
