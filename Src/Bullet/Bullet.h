#pragma once

class BaseMecha;
class BulletObject;

//�ʏ�e�S��//
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

	//����//
	float firstSpeed = 0.0f;

	//�ђʗ�//
	float penetration = 0.0f;

	//�e�̃��f��//
	ChPtr::Shared<ChD3D11::Mesh11>bullet = ChPtr::Make_S<ChD3D11::Mesh11>();
	
private:

	static ChPtr::Shared<BulletData>(*CreateBulletFunction[4])();

};

//�u�[�X�^�[�t���̒e�S��//
class BoostBulletData :public BulletData
{
public:

	virtual void Deserialize(const std::string& _text)override;

	virtual std::string Serialize()override;

public:

	virtual std::string GetBulletType()override { return "1\n"; }

protected:

	//�u�[�X�g�_�΂܂ł̎���//
	unsigned long startBoostTime = 0;


	//�u�[�X�g�̃p���[//
	float boostPow = 0.0f;
};


//�����e�S��//
class HighExplosiveBulletData :public BoostBulletData
{
public:

	virtual void Deserialize(const std::string& _text)override;

	virtual std::string Serialize()override;

public:

	virtual std::string GetBulletType()override { return "2\n"; }

protected:

	//�����͈�//
	float blastRange = 0.0f;

};

//�U���e�S��//
class  MissileData :public HighExplosiveBulletData
{
public://Serialize Deserialize\\

	void Deserialize(const std::string& _text)override;

	std::string Serialize()override;

public:

	virtual std::string GetBulletType()override { return "3\n"; }

protected:

	//��]���x//
	float rotateSpeed = 0.0f;

	//�������͈�//
	float lostRange = 0.0f;


};
