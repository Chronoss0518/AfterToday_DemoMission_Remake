#pragma once

#include "../MechaParts.h"

class AerodynamicsData : public PartsDataBase
{
public://Serialize Deserialize//

	unsigned long Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos = 0)override;

	std::wstring Serialize()override;

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)override;

	void SetPartsParameter(PartsParameters& _base, MechaPartsObject& _parts)override {};

	void SetUpPowerParSpeed(const float _upPowerParSpeed) { upPowerParSpeed = _upPowerParSpeed; }

public://Get Functions//

	float GetGroundNearPos() { return 0.0f; };

	std::wstring GetPartsTypeTag()override { return GET_CLASS_NAME(Aerodynamics); }

protected:

	//ë¨ìxÇ…ÇÊÇÈîΩâ∫ç~ï‚ê≥//
	float  upPowerParSpeed = 0.0f;
};
