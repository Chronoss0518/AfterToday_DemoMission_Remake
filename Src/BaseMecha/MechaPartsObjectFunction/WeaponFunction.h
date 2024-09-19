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

	//次の攻撃可能までの時間//
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

	//持ち手部分と刀身の区切り部分//
	inline void SetObjectPos(ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _hitStart) { hitObjectStart = _hitStart; }

	void SetData(WeaponData* _data)override;

public:

	inline std::wstring GetBulletNum()override { return GetDefaultBulletNum(); }

	inline std::wstring GetReloadCount()override { return GetDefaultReloadCount(); }

private:

	SwordData* swordData = nullptr;

	ChPtr::Shared<ChCpp::FrameObject<wchar_t>>hitObjectStart = nullptr;

	//攻撃開始から現在までの時間//
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

	//弾が出てくる場所//
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

	//残りの弾数//
	unsigned long nowBulletNum = 0;
	//残りのマガジン数//
	unsigned long nowMagazineNum = 0;
	//残りのリロード時間//
	unsigned long nowReloadTime = 0;

};
