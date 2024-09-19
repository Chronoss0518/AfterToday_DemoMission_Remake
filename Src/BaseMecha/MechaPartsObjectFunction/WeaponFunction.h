#pragma once


#include"../MechaPartsObject.h"


class WeaponData;
class SwordData;
class GunData;

class WeaponFunction :public ExternalFunction
{
public:

	virtual void Release()override;

	virtual void AttackUpdate();

	virtual void Update()override;

	inline void SetBaseData(WeaponData* _data)
	{
		if (_data == nullptr)return;

		data = _data;

		SetData(_data);
	}

	inline ChPtr::Shared<Attack> GetAttackData()
	{
		return attackData;
	}

public:

	virtual void StartSubFunction() {};

	virtual void AttackFunction() {};

	virtual void UpdateFunction() {};

	virtual void SetData(WeaponData* _data) = 0;

public:

	static constexpr const wchar_t* const GetDefaultBulletNum() { return L"-"; }

	static constexpr const wchar_t* const GetDefaultReloadCount() { return L"-"; }

	std::wstring GetWeaponName();

	virtual std::wstring GetBulletNum() = 0;

	virtual std::wstring GetReloadCount() = 0;

protected:

	WeaponData* data = nullptr;

	ChD3D::X3DAudioObject se;

	//���̍U���\�܂ł̎���//
	unsigned long nowWaitTime = 0;

	ChPtr::Shared<Attack>attackData = nullptr;

	bool attackFlg = false;

};

class SwordFunction : public WeaponFunction
{
public:

	void Init(ChD3D11::Shader::BaseDrawMesh11<wchar_t>* _drawer, ID3D11Device* _device)override;

	void AttackFunction()override;

	void UpdateFunction()override {};

	//�����蕔���Ɠ��g�̋�؂蕔��//
	inline void SetObjectPos(ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _hitStart) { hitObjectStart = _hitStart; }

	void SetData(WeaponData* _data)override;

public:

	inline std::wstring GetBulletNum()override { return GetDefaultBulletNum(); }

	inline std::wstring GetReloadCount()override { return GetDefaultReloadCount(); }

private:

	SwordData* swordData = nullptr;

	ChPtr::Shared<ChCpp::FrameObject<wchar_t>>hitObjectStart = nullptr;

	//�U���J�n���猻�݂܂ł̎���//
	unsigned long nowAttackTime = 0;


};

class GunFunction : public WeaponFunction
{
public:

	void Init(ChD3D11::Shader::BaseDrawMesh11<wchar_t>* _drawer, ID3D11Device* _device)override;

	void AttackFunction()override;

	void StartSubFunction();

	void UpdateFunction()override;

	void DrawBegin()override;

	void DrawEnd()override;

public:

	//�e���o�Ă���ꏊ//
	inline void SetObjectPos(ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _shotPos) { shotPos = _shotPos; }

	void SetData(WeaponData* _data)override;

public:

	std::wstring GetBulletNum()override;

	std::wstring GetReloadCount()override;

private:

	void UpdatePosture();

private:

	GunData* gunData = nullptr;

	ChPtr::Shared<ChCpp::FrameObject<wchar_t>>shotPos = nullptr;

	ChLMat lastShotPos;

	bool reloadFlg = false;

	//�c��̒e��//
	unsigned long nowBulletNum = 0;
	//�c��̃}�K�W����//
	unsigned long nowMagazineNum = 0;
	//�c��̃����[�h����//
	unsigned long nowReloadTime = 0;

};
