#pragma once

#include"NextPosBase.h"

class NextPosData : public NextPosBase
{
public://Serialize Deserialize//

	inline unsigned long Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos = 0)override
	{
		unsigned long textPos = NextPosBase::Deserialize(_text, _textPos);
		SetConnectionName(_text.GetTextLine(textPos));

		return textPos + 1;
	}

	inline std::wstring Serialize()override
	{

		std::wstring res = NextPosBase::Serialize();
		res += connectionName + L"\n";

		return res;
	}

public://Set Functions//

	inline void SetConnectionName(const std::wstring& _connectionName)
	{
		connectionName = _connectionName;
	}

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

	//ç≈ëÂèdó //
	float maxWeight = 0.0f;

};
