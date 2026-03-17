#pragma once

class BaseMecha;
class AttackObject;
struct PartsParameters;


namespace PartsParameterStruct 
{
	struct AttackData;
	struct WeaponData;
}

enum class AttackType :unsigned char
{
	Sword,//剣//
	Bullet,//通常弾//
	BoostBullet,//ブースト付き弾//
	Explosive,//爆発//
	Missile,//追尾弾//
	BirthBullet,//内臓弾//
};

//攻撃全般//
class Attack
{

public:

	class AttackBase
	{
	public:

		virtual unsigned long Deserialize(ID3D11Device* _device, Attack& _attack,const ChCpp::TextObject<wchar_t>& _text,const unsigned long _nowPos = 0) = 0;

		virtual std::wstring Serialize() = 0;

		virtual void SetPartameter(PartsParameterStruct::AttackData& _parameter) = 0;

		virtual void InitBulletObject(const ChLMat& _startMat, AttackObject& _bullet){}

		virtual void UpdateBulletObject(AttackObject& _bullet) {}

	};

	static ChPtr::Shared<Attack> CreateAttackDataFromSword(ChD3D11::Shader::BaseDrawMesh11<wchar_t>* _drawer, ID3D11Device* _device, const std::wstring& _fileName);

	static ChPtr::Shared<Attack> CreateAttackDataFromExplosive(ChD3D11::Shader::BaseDrawMesh11<wchar_t>* _drawer, ID3D11Device* _device, const std::wstring& _fileName);

	static ChPtr::Shared<Attack> CreateAttackDataFromBullet(ChD3D11::Shader::BaseDrawMesh11<wchar_t>* _drawer, ID3D11Device* _device, const std::wstring& _fileName);

	static ChPtr::Shared<Attack> CreateAttackData(ChD3D11::Shader::BaseDrawMesh11<wchar_t>* _drawer, ID3D11Device* _device, const std::wstring& _fileName);

	static void AllRelease();

public://Serialize Deserialize\\

	virtual void Deserialize(ID3D11Device* _device, const std::wstring& _text);

	virtual std::wstring Serialize();

	virtual void SetPartameter(PartsParameterStruct::WeaponData& _partsParameter);

public: 

	inline void SetMeshDrawer(ChD3D11::Shader::BaseDrawMesh11<wchar_t>* _drawer) { drawer = _drawer; }

public:

	inline unsigned long GetPenetration() { return penetration; }

	inline float GetHitSize() { return hitSize; }

	unsigned char GetAttackType() { return ChStd::EnumCast(attackType); }

public:

	void InitBulletObject(const ChLMat& _startMat,AttackObject& _bullet);

	void UpdateBulletObject(AttackObject& _bullet);

	void MoveBulletObject(AttackObject& _bullet);

	void Draw(const ChMat_11& _mat);

public:

	static std::map<std::wstring,ChPtr::Shared<Attack>>& LoadAttackList()
	{
		static std::map<std::wstring,ChPtr::Shared<Attack>> ins;
		return ins;
	}

protected:

	std::wstring useFileName = L"";
	std::wstring objectName = L"";

	//威力//
	unsigned long penetration = 0;

	//衝突判定用//
	float hitSize = 0.0f;

	AttackType attackType = AttackType::Sword;

	//弾のモデル//
	ChPtr::Shared<ChD3D11::Mesh11<wchar_t>> bullet = ChPtr::Make_S<ChD3D11::Mesh11<wchar_t>>();
	ChLMat defaultMat;

	std::vector<ChPtr::Shared<AttackBase>> externulFunctions;

	ChD3D11::Shader::BaseDrawMesh11<wchar_t>* drawer = nullptr;

private:

};

//通常弾全般//
class BulletData :public Attack::AttackBase
{
public:

	unsigned long Deserialize(ID3D11Device* _device, Attack& _attack, const ChCpp::TextObject<wchar_t>& _text, const unsigned long _nowPos = 0)override;

	std::wstring Serialize()override;

	void SetPartameter(PartsParameterStruct::AttackData& _parameter)override;

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

	virtual unsigned long Deserialize(ID3D11Device* _device, Attack& _attack, const ChCpp::TextObject<wchar_t>& _text, const unsigned long _nowPos = 0)override;

	virtual std::wstring Serialize()override;

	void SetPartameter(PartsParameterStruct::AttackData& _parameter)override;

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
class ExplosiveBulletData :public Attack::AttackBase
{
public:

	virtual unsigned long Deserialize(ID3D11Device* _device, Attack& _attack, const ChCpp::TextObject<wchar_t>& _text, const unsigned long _nowPos = 0)override;

	virtual std::wstring Serialize()override;

	void SetPartameter(PartsParameterStruct::AttackData& _parameter)override;

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
		
	unsigned long Deserialize(ID3D11Device* _device, Attack& _attack, const ChCpp::TextObject<wchar_t>& _text, const unsigned long _nowPos = 0)override;

	std::wstring Serialize()override;

	void SetPartameter(PartsParameterStruct::AttackData& _parameter)override;

public:

	void UpdateBulletObject(AttackObject& _bullet)override;

protected:

	//回転速度//
	float rotateSpeed = 0.0f;

	//見失う範囲//
	float lostRange = 0.0f;

};
