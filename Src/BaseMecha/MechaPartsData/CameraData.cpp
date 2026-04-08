
#include"../../BaseIncluder.h"
#include"../../AllStruct.h"

#include"../../EditFrame/PartsParameters.h"

#include"../../BaseMecha/FunctionComponent/CameraComponent.h"
#include "CameraData.h"

unsigned long CameraData::Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos)
{
	fovy = ChStr::GetNumFromText<float>(_text.GetTextLine(_textPos).c_str());
	cameraCount = ChStr::GetNumFromText<unsigned long>(_text.GetTextLine(_textPos + 1).c_str());
	cameraObject = _text.GetTextLine(_textPos + 2);
	return _textPos + 3;
}

std::wstring CameraData::Serialize()
{
	std::wstring res = L"";

	res += std::to_wstring(fovy) + L"\n";
	res += std::to_wstring(cameraCount) + L"\n";
	res += cameraObject + L"\n";
	return res;
}

void CameraData::SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)
{
	//auto camera = ChPtr::Make_S<CameraObject>();
	auto camCom = _base.GetComponentObject<CameraComponent>();

	camCom->AddCameraData(this);
}
