#pragma once

#include "../MechaParts.h"


class EnergyTankData : public PartsDataBase
{

public:

	void RemoveParameter(BaseMecha& _base)override;

public://Serialize Deserialize//

	virtual unsigned long Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos = 0)override;

	virtual std::wstring Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)override;

	void SetPartsParameter(PartsParameters& _base, MechaPartsObject& _parts)override;

	void SetMaxEnergy(const unsigned long _maxEnergy) { maxEnergy = _maxEnergy; }

	void SetChargeEnergy(const unsigned long _createEnergy) { chargeEnergy = _createEnergy; }

public://Get Functions//

	virtual std::wstring GetPartsTypeTag()override { return  GET_CLASS_NAME(EnergyTankData); }

	unsigned long GetMaxEnergy()const { return maxEnergy; }

	unsigned long GetChargeEnergy()const { return chargeEnergy; }

protected:

	//保持可能最大エネルギー//
	unsigned long maxEnergy = 0;

	//エネルギーの生成量//
	unsigned long chargeEnergy = 0;

};
