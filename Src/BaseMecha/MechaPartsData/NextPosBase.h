#pragma once

#include "../MechaParts.h"

class NextPosBase : public PartsDataBase
{
public://Serialize Deserialize//

	inline virtual unsigned long Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos = 0)override
	{

		SetParamName(_text.GetTextLine(_textPos));

		return _textPos + 1;
	}

	inline virtual std::wstring Serialize()override
	{

		std::wstring res = L"";
		res += nextPosName + L"\n";

		return res;
	}

public://Set Functions//

	void SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)override;

	void SetPartsParameter(PartsParameters& _base) override {};

	virtual void SetObjectPos(BaseMecha& _base, MechaPartsObject& _parts, ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _targetObject) = 0;

	inline void SetParamName(const std::wstring& _objectName)
	{
		nextPosName = _objectName;
	}

public://Get Functions//

	inline std::wstring GetObjectName() { return nextPosName; }

protected:

	std::wstring nextPosName = L"";

};
