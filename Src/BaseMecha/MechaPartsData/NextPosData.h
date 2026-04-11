#pragma once

#include"NextPosBase.h"

class NextPosData : public NextPosBase
{
public://Serialize Deserialize//

	unsigned long Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos = 0)override;

	std::wstring Serialize()override;

public://Set Functions//

	inline void SetMaxWeight(const float _weight)
	{
		maxWeight = _weight;
	}

	void SetObjectPos(BaseMecha& _base, MechaPartsObject& _parts, ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _targetObject)override;

public://Get Functions//

	std::wstring GetPartsTypeTag()override { return GET_CLASS_NAME(NextPos); }

	inline std::wstring GetConnectionName() { return connectionName; }

	inline float GetMaxWeight() { return maxWeight; }


protected:

	//ê⁄ë±ïîÇÃñºèÃ//
	std::wstring connectionName = L"";

	ChVec3 connectionRotateNormal = ChVec3(0.0f, 0.0f, 1.0f);
	ChVec3 connectionRotateUp = ChVec3(0.0f, 1.0f, 0.0f);

	MechaParts::PartsConnectionType type = MechaParts::PartsConnectionType::None;

	RotateDirectionType rotateType = RotateDirectionType::None;

	//ç≈ëÂèdó //
	float maxWeight = 0.0f;

};
