
#include"../../BaseIncluder.h"
#include"../../AllStruct.h"

#include"../../EditFrame/PartsParameters.h"

#include "CaterpillarData.h"
#include"../FunctionComponent/MoveComponent.h"

void CaterpillarData::RemoveParameter(BaseMecha& _base)
{
	auto&& walk = GetComponent<BaseMechaMoveComponent>(_base);

	walk->SetMovePow(0.0f);
	walk->SetRotatePow(0.0f);
	walk->SetJumpPow(0.0f);
}

unsigned long CaterpillarData::Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos)
{
	movePow = ChStr::GetNumFromText<float>(_text.GetTextLine(_textPos).c_str());
	rotatePow = ChStr::GetNumFromText<float>(_text.GetTextLine(_textPos + 1).c_str());
	jumpPow = ChStr::GetNumFromText<float>(_text.GetTextLine(_textPos + 2).c_str());
	return _textPos + 3;
}

std::wstring CaterpillarData::Serialize()
{
	std::wstring res = L"";

	res += std::to_wstring(movePow) + L"\n";
	res += std::to_wstring(rotatePow) + L"\n";
	res += std::to_wstring(jumpPow) + L"\n";

	return res;
}

void CaterpillarData::SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)
{
	auto&& walk = GetComponent<TankMoveComponent>(_base);

	walk->SetMovePow(movePow);
	walk->SetRotatePow(rotatePow);
	walk->SetJumpPow(jumpPow);

	auto&& model = GetModel(_parts);

	auto&& maxPos = model.GetInitAllFrameMaxPos();

	walk->SetSideSize(maxPos.x);
}

void CaterpillarData::SetPartsParameter(PartsParameters& _base)
{
	_base.walkData.movePower = movePow;
	_base.walkData.rotatePower = rotatePow;
	_base.walkData.jumpPower = jumpPow;
}
