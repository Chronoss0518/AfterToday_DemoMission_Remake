
#include"../../BaseIncluder.h"
#include"../../AllStruct.h"

#include"../../EditFrame/PartsParameters.h"

#include"../../BaseMecha/FunctionComponent/CameraComponent.h"
#include "ScopeData.h"

unsigned long ScopeData::Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos)
{
	unsigned long textPos = CameraData::Deserialize(_text, _textPos);
	minFovy = ChStr::GetNumFromText<float>(_text.GetTextLine(textPos).c_str());
	maxFovy = ChStr::GetNumFromText<float>(_text.GetTextLine(textPos + 1).c_str());
	fovySlideSpeed = ChStr::GetNumFromText<float>(_text.GetTextLine(textPos + 2).c_str());
	return textPos + 3;
}

std::wstring ScopeData::Serialize()
{
	std::wstring res = L"";
	res = CameraData::Serialize();
	res += std::to_wstring(minFovy) + L"\n";
	res += std::to_wstring(maxFovy) + L"\n";
	res += std::to_wstring(fovySlideSpeed) + L"\n";

	return res;
}

void ScopeData::SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)
{
	//auto camera = ChPtr::Make_S<CameraObject>();
	auto camCom = _base.GetComponentObject<CameraComponent>();

	camCom->AddCameraData(this);
}
