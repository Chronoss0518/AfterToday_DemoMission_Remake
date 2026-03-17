
#include"../../BaseIncluder.h"
#include"../../AllStruct.h"

#include"../../EditFrame/PartsParameters.h"

#include"BoostBrustData.h"

#include"../FunctionComponent/BoostComponent.h"

void BoostBrust::RemoveParameter(BaseMecha& _base)
{
	auto&& obj = GetFrame(_base);

	if (obj == nullptr)return;

	auto&& com = GetComponent<BoostComponent>(_base);

	com->AddBoostData(this);
}

unsigned long BoostBrust::Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos)
{
	objectName = _text.GetTextLine(_textPos);
	directionFlgs.SetValue(ChStr::GetNumFromText<unsigned long>(_text.GetTextLine(_textPos + 1).c_str()), 0);
	useEnergy = ChStr::GetNumFromText<unsigned long>(_text.GetTextLine(_textPos + 2).c_str());
	boostPower = ChStr::GetNumFromText<float>(_text.GetTextLine(_textPos + 3).c_str());
	avoidUseEnergy = ChStr::GetNumFromText<unsigned long>(_text.GetTextLine(_textPos + 4).c_str());
	avoidPow = ChStr::GetNumFromText<float>(_text.GetTextLine(_textPos + 5).c_str());
	avoidWait = ChStr::GetNumFromText<unsigned long>(_text.GetTextLine(_textPos + 6).c_str());
	return _textPos + 6;
}

std::wstring BoostBrust::Serialize()
{

	std::wstring res = L"";

	res = objectName;
	res += std::to_wstring(directionFlgs.GetValue(0)) + L"\n";
	res += std::to_wstring(useEnergy) + L"\n";
	res += std::to_wstring(boostPower) + L"\n";
	res += std::to_wstring(avoidUseEnergy) + L"\n";
	res += std::to_wstring(avoidPow) + L"\n";

	return res;
}

void BoostBrust::SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)
{
	auto&& obj = GetFrame(_base);

	if (obj == nullptr)return;

	auto&& com = GetComponent<BoostComponent>(_base);

	com->AddBoostData(this);
}

void BoostBrust::SetBoostData(BoostDirection _direction, PartsParameterStruct::BoostData& _boost)
{
	if (!directionFlgs.GetBitFlg((int)_direction))return;
	_boost.boostPower = boostPower;
	_boost.boostUseEnergy = useEnergy;
	_boost.avoidPower = avoidPow;
	_boost.avoidUseEnergy = avoidUseEnergy;
	_boost.avoidWait = avoidWait;
}

void BoostBrust::SetPartsParameter(PartsParameters& _base)
{
	SetBoostData(BoostDirection::Up, _base.upBoostData);
	SetBoostData(BoostDirection::Down, _base.downBoostData);
	SetBoostData(BoostDirection::Left, _base.leftBoostData);
	SetBoostData(BoostDirection::Right, _base.rightBoostData);
	SetBoostData(BoostDirection::Front, _base.frontBoostData);
	SetBoostData(BoostDirection::Back, _base.backBoostData);
}

ChPtr::Shared<ChCpp::FrameObject<wchar_t>> BoostBrust::GetFrame(BaseMecha& _base)
{
	auto&& base = *LookObj<MechaParts>();
	if (ChPtr::NullCheck(&base))return nullptr;

	auto&& mesh = base.GetMesh();

	auto&& boostList = mesh.GetAllChildlenForName<ChCpp::FrameObject<wchar_t>>(objectName);

	if (boostList.empty())return nullptr;

	return boostList[0].lock();

}