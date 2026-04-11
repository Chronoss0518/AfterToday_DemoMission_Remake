
#include"../../BaseIncluder.h"
#include"../../AllStruct.h"

#include"../../EditFrame/PartsParameters.h"
#include"../MechaPartsObject.h"

#include"BoostBrustData.h"

#include"../FunctionComponent/BoostComponent.h"
#include"../MechaPartsObjectFunction/BoostFunction.h"

void BoostBrust::RemoveParameter(BaseMecha& _base)
{
	auto&& com = GetComponent<BoostComponent>(_base);

	com->SubBoostData(this);
}

unsigned long BoostBrust::Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos)
{
	unsigned long textPos = NextPosBase::Deserialize(_text, _textPos);
	directionFlgs.SetValue(ChStr::GetNumFromText<unsigned long>(_text.GetTextLine(textPos).c_str()), 0);
	useEnergy = ChStr::GetNumFromText<unsigned long>(_text.GetTextLine(textPos + 1).c_str());
	boostPower = ChStr::GetNumFromText<float>(_text.GetTextLine(textPos + 2).c_str());
	avoidUseEnergy = ChStr::GetNumFromText<unsigned long>(_text.GetTextLine(textPos + 3).c_str());
	avoidPow = ChStr::GetNumFromText<float>(_text.GetTextLine(textPos + 4).c_str());
	avoidWait = ChStr::GetNumFromText<unsigned long>(_text.GetTextLine(textPos + 5).c_str());
	return textPos + 6;
}

std::wstring BoostBrust::Serialize()
{

	std::wstring res = NextPosBase::Serialize();
	res += std::to_wstring(directionFlgs.GetValue(0)) + L"\n";
	res += std::to_wstring(useEnergy) + L"\n";
	res += std::to_wstring(boostPower) + L"\n";
	res += std::to_wstring(avoidUseEnergy) + L"\n";
	res += std::to_wstring(avoidPow) + L"\n";
	res += std::to_wstring(avoidWait) + L"\n";

	return res;
}

void BoostBrust::SetObjectPos(BaseMecha& _base, MechaPartsObject& _parts, ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _targetObject)
{
	if (_targetObject == nullptr)return;

	auto&& com = GetComponent<BoostComponent>(_base);
	auto&& func = _parts.SetComponent<BoostFunction>();
	func->SetTargetBoostObject(_targetObject);

	com->AddBoostData(this, func.get());
}

void BoostBrust::SetBoostData(Direction _direction, PartsParameterStruct::BoostData& _boost)
{
	if (!directionFlgs.GetBitFlg((int)_direction))return;
	_boost.boostPower = boostPower;
	_boost.boostUseEnergy = useEnergy;
	_boost.avoidPower = avoidPow;
	_boost.avoidUseEnergy = avoidUseEnergy;
	_boost.avoidWait = avoidWait;
}

void BoostBrust::SetPartsParameter(PartsParameters& _base, MechaPartsObject& _parts)
{
	SetBoostData(Direction::Up, _base.upBoostData);
	SetBoostData(Direction::Down, _base.downBoostData);
	SetBoostData(Direction::Left, _base.leftBoostData);
	SetBoostData(Direction::Right, _base.rightBoostData);
	SetBoostData(Direction::Front, _base.frontBoostData);
	SetBoostData(Direction::Back, _base.backBoostData);
}
