#pragma once

class BaseMecha;
class BulletObject;

//通常弾全般//
class BulletData
{
	static void CreateBullet(const std::string& _fileName);

public://Serialize Deserialize\\

	virtual void Deserialize(const std::string& _text);

	virtual std::string Serialize();

public:

	virtual std::string GetBulletType() { return "0\n"; }

public:

	static std::map<std::string,ChPtr::Shared<BulletData>>LoadBulletList()
	{
		static std::map<std::string,ChPtr::Shared<BulletData>> ins;
		return ins;
	}

protected:

	//初速//
	float firstSpeed = 0.0f;

	//貫通力//
	float penetration = 0.0f;

	//弾のモデル//
	ChPtr::Shared<ChD3D11::Mesh11>bullet = ChPtr::Make_S<ChD3D11::Mesh11>();
	
private:

	static ChPtr::Shared<BulletData>(*CreateBulletFunction[4])();

};

//ブースター付きの弾全般//
class BoostBulletData :public BulletData
{
public:

	virtual void Deserialize(const std::string& _text)override;

	virtual std::string Serialize()override;

public:

	virtual std::string GetBulletType()override { return "1\n"; }

protected:

	//ブースト点火までの時間//
	unsigned long startBoostTime = 0;


	//ブーストのパワー//
	float boostPow = 0.0f;
};


//爆発弾全般//
class HighExplosiveBulletData :public BoostBulletData
{
public:

	virtual void Deserialize(const std::string& _text)override;

	virtual std::string Serialize()override;

public:

	virtual std::string GetBulletType()override { return "2\n"; }

protected:

	//爆発範囲//
	float blastRange = 0.0f;

};

//誘導弾全般//
class  MissileData :public HighExplosiveBulletData
{
public://Serialize Deserialize\\

	void Deserialize(const std::string& _text)override;

	std::string Serialize()override;

public:

	virtual std::string GetBulletType()override { return "3\n"; }

protected:

	//回転速度//
	float rotateSpeed = 0.0f;

	//見失う範囲//
	float lostRange = 0.0f;


};
