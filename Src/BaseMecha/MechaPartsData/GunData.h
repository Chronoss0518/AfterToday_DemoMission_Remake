#pragma once

#include"WeaponDataBase.h"

class GunData :public WeaponDataBase
{

public://Serialize Deserialize//

	unsigned long Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos = 0)override;

	std::wstring Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)override;

	void SetPartsParameter(PartsParameters& _base, MechaPartsObject& _parts)override;

	inline void SetFireNum(const unsigned long _fireNum) { fireNum = _fireNum; }

	inline void SetBulletNum(const unsigned long _bulletNum) { bulletNum = _bulletNum; }

	inline void SetMagazineNum(const unsigned long _magazineNum) { magazineNum = _magazineNum; }

	inline void SetReloadTime(const unsigned long _reloadTime) { reloadTime = _reloadTime; }

	inline void SetBulletFile(const std::wstring& _bulletFile) { bulletFile = _bulletFile; }

public://Get Functions//

	inline std::wstring GetPartsTypeTag() override { return GET_CLASS_NAME(GunData); }

	inline unsigned long GetFireNum()const { return fireNum; }

	inline unsigned long GetBulletNum() const { return bulletNum; }

	inline unsigned long GetMagazineNum() const { return magazineNum; }

	inline unsigned long GetReloadTime() const { return reloadTime; }

	inline unsigned char GetRange() const { return range; }

	inline std::wstring GetUseBulletFile() const { return bulletFile; }

protected:

	//射撃時の弾の数//
	unsigned long fireNum = 1;
	//一回のリロードで打てる球の数//
	unsigned long bulletNum = 0;
	//リロードできる回数//
	unsigned long magazineNum = 0;
	//リロードのかかる時間//
	unsigned long reloadTime = 0;

	//弾の発射方向の誤差//
	unsigned char range = 0;

	std::wstring bulletFile = L"";

};