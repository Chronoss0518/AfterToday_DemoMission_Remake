#pragma once

#include"NextPosBase.h"

class PostureData : public NextPosBase
{
public://Serialize Deserialize//

	inline unsigned long Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos = 0)override
	{
		unsigned long textPos = NextPosBase::Deserialize(_text, _textPos);
		dir.Deserialize<wchar_t>(_text[textPos], 0);
		SetRotateAxis(static_cast<RotateAxis>(ChStr::GetNumFromText<int>(_text[textPos + 1])));
		SetMinRotate(ChStr::GetNumFromText<float>(_text[textPos + 2]));
		SetMaxRotate(ChStr::GetNumFromText<float>(_text[textPos + 3]));

		return textPos + 4;
	}

	inline std::wstring Serialize()override
	{
		std::wstring res = NextPosBase::Serialize();
		res += dir.Serialize<wchar_t>() + L"\n";
		res += std::to_wstring(static_cast<unsigned char>(GetRotateAxis())) + L"\n";
		res += std::to_wstring(GetMinRotate()) + L"\n";
		res += std::to_wstring(GetMaxRotate()) + L"\n";

		return res;
	}

public://Set Functions//

	inline void SetRotateAxis(const RotateAxis _axis)
	{
		posture.SetRotateAxis(_axis);
	}

	inline void SetMinRotate(const float& _rotate)
	{
		posture.SetMinRotate(_rotate);
	}

	inline void SetMaxRotate(const float& _rotate)
	{
		posture.SetMaxRotate(_rotate);
	}

	void SetObjectPos(BaseMecha& _base, MechaPartsObject& _parts, ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _targetObject)override;

public://Get Functions//

	std::wstring GetPartsTypeTag()override { return GET_CLASS_NAME(Posture); }

	inline RotateAxis GetRotateAxis()
	{
		return posture.GetRotateAxis();
	}

	inline float GetMinRotate()
	{
		return posture.GetMinRotate();
	}

	inline float GetMaxRotate()
	{
		return posture.GetMinRotate();
	}


protected:

	PostureController posture;

	ChVec3 dir = ChVec3();
};
