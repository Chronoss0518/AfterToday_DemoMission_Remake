
#include"../../BaseIncluder.h"
#include"../../AllStruct.h"

#include"../../EditFrame/PartsParameters.h"
#include"../../Attack/Attack.h"
#include"../MechaPartsObjectFunction/WeaponFunction.h"

#include"WeaponDataBase.h"

unsigned long WeaponDataBase::Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos)
{
	unsigned long textPos = NextPosBase::Deserialize(_text, _textPos);
	weaponName = _text.GetTextLine(textPos);
	seFile = _text.GetTextLine(textPos + 1);
	waitTime = ChStr::GetNumFromText<unsigned long>(_text.GetTextLine(textPos + 2).c_str());
	lookTarget = _text.GetTextLine(textPos + 3) == L"1";
	return textPos + 4;
}

std::wstring WeaponDataBase::Serialize()
{
	std::wstring res = NextPosBase::Serialize();

	res += weaponName + L"\n";
	res += seFile + L"\n";
	res += std::to_wstring(waitTime) + L"\n";
	res += lookTarget ? L"1" : L"0";

	return res;
}

void WeaponDataBase::SetObjectPos(BaseMecha& _base, MechaPartsObject& _parts, ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _targetObject)
{
	_parts.GetWeaponFunctions()[_parts.GetWeaponFunctions().size() - 1]->SetAttackPos(_targetObject);
}

void WeaponDataBase::SetWeaponData(PartsParameterStruct::WeaponData& _base)
{
	auto&& look = LookObj<MechaParts>();
	if (look != nullptr)
	{
		_base.partsName = look->GetThisFileName();
	}

	_base.weaponName = weaponName;
	_base.waitTime = waitTime;
}
