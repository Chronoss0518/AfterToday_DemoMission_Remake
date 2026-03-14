#pragma once

#include"NextPosBase.h"

class NextPosData : public NextPosBase
{
public://Serialize Deserialize//

	inline unsigned long Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos = 0)override
	{
		unsigned long textPos = NextPosBase::Deserialize(_text, _textPos);
		connectionName = _text.GetTextLine(textPos);

		connectionRotateNormal.Deserialize(_text.GetTextLine(textPos + 1));
		connectionRotateUp.Deserialize(_text.GetTextLine(textPos + 2));

		type = (MechaParts::PartsConnectionType)ChStr::GetNumFromText<int>(_text.GetTextLine(textPos + 3));

		maxWeight = ChStr::GetNumFromText<float>(_text.GetTextLine(textPos + 4));

		return textPos + 5;
	}

	inline std::wstring Serialize()override
	{

		std::wstring res = NextPosBase::Serialize();
		res += connectionName + L"\n";

		res += connectionRotateNormal.Serialize<wchar_t>() + L"\n";
		res += connectionRotateUp.Serialize<wchar_t>() + L"\n";

		res += ChStr::GetTextFromNum<wchar_t>((int)type) + L"\n";

		res += ChStr::GetTextFromNum<wchar_t>(maxWeight) + L"\n";

		return res;
	}

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

	//ç≈ëÂèdó //
	float maxWeight = 0.0f;

};
