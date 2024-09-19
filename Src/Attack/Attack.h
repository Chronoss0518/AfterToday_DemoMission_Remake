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
	Sword,//��//
	Bullet,//�ʏ�e//
	BoostBullet,//�u�[�X�g�t���e//
	HighExplosive,//�����e//
	Missile,//�ǔ��e//
	BirthBullet,//�����e//
	Explosive,//����//
};

//�U���S��//
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

	//�З�//
	unsigned long penetration = 0;

	//�Փ˔���p//
	float hitSize = 0.0f;

	AttackType attackType = AttackType::Sword;

	//�e�̃��f��//
	ChPtr::Shared<ChD3D11::Mesh11<wchar_t>> bullet = ChPtr::Make_S<ChD3D11::Mesh11<wchar_t>>();
	ChLMat defaultMat;

	std::vector<ChPtr::Shared<AttackBase>> externulFunctions;

	ChD3D11::Shader::BaseDrawMesh11<wchar_t>* drawer = nullptr;

private:

};

//�ʏ�e�S��//
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

	//����//
	float firstSpeed = 0.0f;

};

//�u�[�X�^�[�t���̒e�S��//
class BoostBulletData :public Attack::AttackBase
{
public:

	virtual unsigned long Deserialize(ID3D11Device* _device, Attack& _attack, const ChCpp::TextObject<wchar_t>& _text, const unsigned long _nowPos = 0)override;

	virtual std::wstring Serialize()override;

	void SetPartameter(PartsParameterStruct::AttackData& _parameter)override;

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

	virtual unsigned long Deserialize(ID3D11Device* _device, Attack& _attack, const ChCpp::TextObject<wchar_t>& _text, const unsigned long _nowPos = 0)override;

	virtual std::wstring Serialize()override;

	void SetPartameter(PartsParameterStruct::AttackData& _parameter)override;

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
		
	unsigned long Deserialize(ID3D11Device* _device, Attack& _attack, const ChCpp::TextObject<wchar_t>& _text, const unsigned long _nowPos = 0)override;

	std::wstring Serialize()override;

	void SetPartameter(PartsParameterStruct::AttackData& _parameter)override;

public:

	void UpdateBulletObject(AttackObject& _bullet)override;

protected:

	//��]���x//
	float rotateSpeed = 0.0f;

	//�������͈�//
	float lostRange = 0.0f;

};
