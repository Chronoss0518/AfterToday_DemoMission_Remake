
#include"../../BaseIncluder.h"
#include"../../AllStruct.h"

#include"../../EditFrame/PartsParameters.h"

#include "AerodynamicsData.h"

unsigned long AerodynamicsData::Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos)
{
	upPowerParSpeed = ChStr::GetNumFromText<float>(_text.GetTextLine(_textPos).c_str());
	return _textPos + 1;
}

std::wstring AerodynamicsData::Serialize()
{
	std::wstring res = L"";

	res += std::to_wstring(upPowerParSpeed) + L"\n";

	return res;
}

void AerodynamicsData::SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)
{
	//auto&& moveAcceleration = GetComponent<Aerodynamics>(_base);

}
