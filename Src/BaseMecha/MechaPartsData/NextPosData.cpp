
#include"../../BaseIncluder.h"
#include"../../AllStruct.h"

#include"../../EditFrame/PartsParameters.h"

#include "NextPosData.h"

unsigned long NextPosData::Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos)
{
	unsigned long textPos = NextPosBase::Deserialize(_text, _textPos);
	connectionName = _text.GetTextLine(textPos);

	connectionRotateNormal.Deserialize(_text.GetTextLine(textPos + 1));
	connectionRotateUp.Deserialize(_text.GetTextLine(textPos + 2));

	type = (MechaParts::PartsConnectionType)ChStr::GetNumFromText<int>(_text.GetTextLine(textPos + 3));

	maxWeight = ChStr::GetNumFromText<float>(_text.GetTextLine(textPos + 4));
	
	rotateType = static_cast<RotateDirectionType>(ChStr::GetNumFromText<int>(_text.GetTextLine(textPos + 5)));

	return textPos + 6;
}

std::wstring NextPosData::Serialize()
{

	std::wstring res = NextPosBase::Serialize();
	res += connectionName + L"\n";

	res += connectionRotateNormal.Serialize<wchar_t>() + L"\n";
	res += connectionRotateUp.Serialize<wchar_t>() + L"\n";

	res += ChStr::GetTextFromNum<wchar_t>((int)type) + L"\n";

	res += ChStr::GetTextFromNum<wchar_t>(maxWeight) + L"\n";

	res += ChStr::GetTextFromNum<wchar_t>((int)rotateType) + L"\n";

	return res;
}

void NextPosData::SetObjectPos(BaseMecha& _base, MechaPartsObject& _parts, ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _targetObject)
{
	auto&& mechaParts = LookObj<MechaParts>();

	ChVec4 tmpUp = connectionRotateUp;
	tmpUp.w = 0.0f;
	tmpUp.Normalize();
	ChVec4 tmpNormal = connectionRotateNormal;
	tmpNormal.w = 0.0f;
	tmpNormal.Normalize();

	ChVec4 cross = ChVec3::GetCross(tmpUp, tmpNormal);
	cross.w = 0.0f;
	ChLMat lmat;
	lmat.m[0].Set(cross.val);
	lmat.m[1].Set(tmpUp.val);
	lmat.m[2].Set(tmpNormal.val);

	mechaParts->AddPosition(connectionName, _targetObject, lmat, type, maxWeight);
}
