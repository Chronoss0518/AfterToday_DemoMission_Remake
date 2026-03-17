
#include"../../BaseIncluder.h"
#include"../../AllStruct.h"

#include"../../EditFrame/PartsParameters.h"

#include "WalkData.h"
#include"../FunctionComponent/MoveComponent.h"


void WalkData::RemoveParameter(BaseMecha& _base)
{
	auto&& move = GetComponent<MoveComponent>(_base);

	move->RemoveMoveObject(this);
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
	auto&& move = GetComponent<MoveComponent>(_base);

	moveObject = ChPtr::Make_S<WalkMoveObject>();

	moveObject->SetWalkData(this);

	move->AddMoveObject(moveObject);
}

void WalkData::SetPartsParameter(PartsParameters& _base)
{
	auto&& move = ChPtr::Make_S<PartsParameterStruct::MoveData>();

	move->movePower = movePow;
	move->rotatePower = rotatePow;
	move->jumpPower = jumpPow;

	_base.moveData.push_back(move);
}
