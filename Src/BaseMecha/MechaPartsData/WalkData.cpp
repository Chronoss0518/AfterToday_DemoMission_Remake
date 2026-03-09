
#include"../../BaseIncluder.h"
#include"../../AllStruct.h"

#include"../../EditFrame/PartsParameters.h"

#include "WalkData.h"
#include"../FunctionComponent/MoveComponent.h"


void WalkData::RemoveParameter(BaseMecha& _base)
{
	auto&& walk = GetComponent<BaseMechaMoveComponent>(_base);

	walk->SetMovePow(0.0f);
	walk->SetRotatePow(0.0f);
	walk->SetJumpPow(0.0f);
}

unsigned long WalkData::Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos)
{
	movePow = ChStr::GetNumFromText<float>(_text.GetTextLine(_textPos).c_str());
	rotatePow = ChStr::GetNumFromText<float>(_text.GetTextLine(_textPos + 1).c_str());
	jumpPow = ChStr::GetNumFromText<float>(_text.GetTextLine(_textPos + 2).c_str());
	return _textPos + 3;
}

std::wstring WalkData::Serialize()
{
	std::wstring res = L"";

	res += std::to_wstring(movePow) + L"\n";
	res += std::to_wstring(rotatePow) + L"\n";
	res += std::to_wstring(jumpPow) + L"\n";

	return res;
}

void WalkData::SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)
{
	auto&& walk = GetComponent<BaseMechaMoveComponent>(_base);

	walk->SetMovePow(movePow);
	walk->SetRotatePow(rotatePow);
	walk->SetJumpPow(jumpPow);
}

void WalkData::SetPartsParameter(PartsParameters& _base)
{
	_base.walkData.movePower = movePow;
	_base.walkData.rotatePower = rotatePow;
	_base.walkData.jumpPower = jumpPow;
}
