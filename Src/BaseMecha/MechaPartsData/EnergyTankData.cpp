
#include"../../BaseIncluder.h"
#include"../../AllStruct.h"

#include"../../EditFrame/PartsParameters.h"

#include "EnergyTankData.h"
#include"../FunctionComponent/EnergyComponent.h"

void EnergyTankData::RemoveParameter(BaseMecha& _base)
{
	auto&& energy = GetComponent<EnergyComponent>(_base);

	energy->SubMaxEnergy(maxEnergy);
	energy->SubChargeEnergy(chargeEnergy);
}

unsigned long EnergyTankData::Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos)
{
	maxEnergy = ChStr::GetNumFromText<unsigned long>(_text.GetTextLine(_textPos).c_str());
	chargeEnergy = ChStr::GetNumFromText<unsigned long>(_text.GetTextLine(_textPos + 1).c_str());

	return _textPos + 2;
}

std::wstring EnergyTankData::Serialize()
{
	std::wstring res = L"";

	res += std::to_wstring(maxEnergy) + L"\n";
	res += std::to_wstring(chargeEnergy) + L"\n";

	return res;
}

void EnergyTankData::SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)
{
	auto&& energy = GetComponent<EnergyComponent>(_base);

	energy->AddMaxEnergy(maxEnergy);
	energy->AddChargeEnergy(chargeEnergy);
}

void EnergyTankData::SetPartsParameter(PartsParameters& _base, MechaPartsObject& _parts)
{
	_base.energyTankData.maxEnergy += maxEnergy;
	_base.energyTankData.chargeEnergy += chargeEnergy;
}
