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

//攻撃全般//
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

	//初速//
	float firstSpeed = 0.0f;

	//貫通力//
	unsigned long penetration = 0;

	//衝突判定用//
	float hitSize = 0.0f;

	//弾のモデル//
	ChPtr::Shared<ChD3D11::Mesh11> bullet = ChPtr::Make_S<ChD3D11::Mesh11>();

	ChCpp::BitBool attackType;
	ChPtr::Shared<AttackBase> externulFunction[5] = { nullptr,nullptr,nullptr,nullptr,nullptr };

	MeshDrawer* drawer = nullptr;

private:

};

//通常弾全般//
class BulletData :public Attack::AttackBase
{
public:

	unsigned long Deserialize(ID3D11Device* _device, const ChCpp::TextObject& _text, const unsigned long _nowPos = 0)override;

	std::string Serialize()override;

	void UpdateBulletObject(AttackObject& _bullet)override;
};

//ブースター付きの弾全般//
class BoostBulletData :public Attack::AttackBase
{
public:

	virtual unsigned long Deserialize(ID3D11Device* _device, const ChCpp::TextObject& _text, const unsigned long _nowPos = 0)override;

	virtual std::string Serialize()override;

public:

	void UpdateBulletObject(AttackObject& _bullet)override;


protected:

	//ブースト点火までの時間//
	unsigned long startBoostTime = 0;

	//ブーストのパワー//
	float boostPow = 0.0f;
};


//爆発弾全般//
class HighExplosiveBulletData :public Attack::AttackBase
{
public:

	virtual unsigned long Deserialize(ID3D11Device* _device, const ChCpp::TextObject& _text, const unsigned long _nowPos = 0)override;

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
		
	unsigned long Deserialize(ID3D11Device* _device, const ChCpp::TextObject& _text, const unsigned long _nowPos = 0)override;

	std::string Serialize()override;

public:

	void UpdateBulletObject(AttackObject& _bullet)override;

protected:

	//回転速度//
	float rotateSpeed = 0.0f;

	//見失う範囲//
	float lostRange = 0.0f;


};
