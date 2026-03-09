#pragma once

#include"NextPosBase.h"

class WeaponDataBase : public NextPosBase
{
public://Serialize Deserialize//

	virtual unsigned long Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos = 0)override;

	virtual std::wstring Serialize()override;

public:

	inline void SetWeaponName(const std::wstring& _weaponName) { weaponName = _weaponName; }

	inline void SetSEFileName(const std::wstring& _seFile) { seFile = _seFile; }

	inline void SetWaitTime(const unsigned long _waitTime) { waitTime = _waitTime; }

	inline void SetLookTargetFlg(const bool _flg) { lookTarget = _flg; }

	void SetObjectPos(BaseMecha& _base, MechaPartsObject& _parts, ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _targetObject)override;

	void SetWeaponData(PartsParameterStruct::WeaponData& _base);

public:

	inline std::wstring GetWeaponName() { return weaponName; }

	inline std::wstring GetSEFileName() { return seFile; }

	inline unsigned long GetWaitTime() const { return waitTime; }

	inline bool GetLookTargetFlg() { return lookTarget; }

protected:

	//武器の名称//
	std::wstring weaponName = L"";
	//効果音のファイル//
	std::wstring seFile = L"";
	//次の攻撃可能時間//
	unsigned long waitTime = 0;

	//対象を追尾するかのフラグ//
	bool lookTarget = false;

};