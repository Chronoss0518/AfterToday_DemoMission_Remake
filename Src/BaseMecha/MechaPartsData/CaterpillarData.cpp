
#include"../../BaseIncluder.h"
#include"../../AllStruct.h"

#include"../../EditFrame/PartsParameters.h"

#include "CaterpillarData.h"
#include"../FunctionComponent/MoveComponent.h"

void CaterpillarData::RemoveParameter(BaseMecha& _base)
{
	auto&& walk = GetComponent<MoveComponent>(_base);

	walk->RemoveMoveObject(moveObject);
}

unsigned long CaterpillarData::Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos)
{
	movePow = ChStr::GetNumFromText<float>(_text.GetTextLine(_textPos).c_str());
	jumpPow = ChStr::GetNumFromText<float>(_text.GetTextLine(_textPos + 1).c_str());
	return _textPos + 3;
}

std::wstring CaterpillarData::Serialize()
{
	std::wstring res = L"";

	res += std::to_wstring(movePow) + L"\n";
	res += std::to_wstring(jumpPow) + L"\n";

	return res;
}

void CaterpillarData::SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)
{
	auto&& move = GetComponent<MoveComponent>(_base);

	moveObject = ChPtr::Make_S<TankMoveComponent>(_base);

	moveObject->SetMovePow(movePow);
	moveObject->SetJumpPow(jumpPow);

	auto&& model = GetModel(_parts);

	auto&& maxPos = model.GetInitAllFrameMaxPos();

	moveObject->SetSideSize(maxPos.x);

	move->AddMoveObject(moveObject);
}

void CaterpillarData::SetPartsParameter(PartsParameters& _base)
{
	auto&& move = ChPtr::Make_S<PartsParameterStruct::MoveData>();
	move->movePower = movePow;
	move->jumpPower = jumpPow;

	_base.moveData.push_back(move);
}
