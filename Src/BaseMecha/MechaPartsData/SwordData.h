#pragma once

#include"WeaponDataBase.h"

class SwordData :public WeaponDataBase
{
public://Serialize Deserialize//

	unsigned long Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos = 0)override;

	std::wstring Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)override;

	void SetPartsParameter(PartsParameters& _base)override;

	inline void SetAttackTime(const unsigned long _attackTime) { attackTime = _attackTime; }

public://Get Functions//

	inline std::wstring GetPartsTypeTag() override { return GET_CLASS_NAME(SwordData); }

	inline unsigned long GetWeatTime() { return attackTime; }

protected:

	unsigned long damageParSpeed = 0;

	unsigned long attackTime = 0;

};
