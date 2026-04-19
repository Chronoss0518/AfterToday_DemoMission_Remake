
#include"../../BaseIncluder.h"
#include"../../AllStruct.h"
#include"../../Application/Application.h"

#include"../../EditFrame/PartsParameters.h"
#include"../../Attack/Attack.h"
#include"../MechaPartsObjectFunction/WeaponFunction.h"

#include"WeaponDataBase.h"

unsigned long WeaponDataBase::Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos)
{
	auto device = AppIns().GetDirect3D11().GetDevice();

	unsigned long textPos = NextPosBase::Deserialize(_text, _textPos);
	weaponName = _text.GetTextLine(textPos);
	weaponPaletteImageFilePath = _text.GetTextLine(textPos + 1);
	seFile = _text.GetTextLine(textPos + 2);
	waitTime = ChStr::GetNumFromText<unsigned long>(_text.GetTextLine(textPos + 3).c_str());
	lookTarget = _text.GetTextLine(textPos + 4) == L"1";

	if (weaponPaletteImageFilePath != L"")
		weaponPaletteImage.CreateTexture(weaponPaletteImageFilePath, device);
	else
	{
		ChVec4 tmpCol = ChVec4::FromColor(1.0f, 1.0f, 1.0f, 1.0f);
		weaponPaletteImage.CreateColorTexture(device, &tmpCol, 1, 1);

	}
	return textPos + 5;
}

std::wstring WeaponDataBase::Serialize()
{
	std::wstring res = NextPosBase::Serialize();

	res += weaponName + L"\n";
	res += weaponPaletteImageFilePath + L"\n";
	res += seFile + L"\n";
	res += std::to_wstring(waitTime) + L"\n";
	res += lookTarget ? L"1" : L"0";

	return res;
}

void WeaponDataBase::SetObjectPos(BaseMecha& _base, MechaPartsObject& _parts, ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _targetObject)
{
	_parts.GetWeaponFunctions()[_parts.GetWeaponFunctions().size() - 1]->SetAttackPos(_targetObject);
}

void WeaponDataBase::SetWeaponData( PartsParameterStruct::WeaponData& _data, PartsParameters& _base, MechaPartsObject& _parts)
{
	auto&& look = LookObj<MechaParts>();
	if (look != nullptr)
	{
		_data.partsName = look->GetThisFileName();
	}

	_data.weaponName = weaponName;
	_data.waitTime = waitTime;

	_data.leftWeaponPaletteNo = _parts.GetWeaponPaletteCounter(_base.weaponData.size(), WeaponHandType::Left);
	_data.rightWeaponPaletteNo = _parts.GetWeaponPaletteCounter(_base.weaponData.size(), WeaponHandType::Right);
}
