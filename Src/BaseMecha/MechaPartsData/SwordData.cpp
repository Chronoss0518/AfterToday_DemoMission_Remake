
#include"../../BaseIncluder.h"
#include"../../AllStruct.h"

#include"../../EditFrame/PartsParameters.h"

#include"SwordData.h"

#include"../../Attack/Attack.h"

#include"../MechaPartsObjectFunction/WeaponFunction.h"

#include"../../Application/Application.h"

unsigned long SwordData::Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos)
{
	unsigned long textPos = WeaponDataBase::Deserialize(_text, _textPos);
	attackTime = ChStr::GetNumFromText<unsigned long>(_text.GetTextLine(textPos).c_str());
	return textPos + 1;
}

std::wstring SwordData::Serialize()
{
	std::wstring res = L"";

	res = WeaponDataBase::Serialize();
	res += std::to_wstring(attackTime) + L"\n";

	return res;
}

void SwordData::SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)
{
	auto& thisParts = *LookObj<MechaParts>();

	auto function = ChPtr::Make_S<SwordFunction>();

	function->SetParts(&_parts);

	function->SetBaseMecha(&_base);

	function->SetFrame(_frame);

	function->SetBaseData(this);

	function->Init(thisParts.GetMeshDrawer(), AppIns().GetDirect3D11().GetDevice());

	_parts.AddWeaponFunction(function);

	NextPosBase::SetPartsParameter(_base, _parts, _frame);
}

void SwordData::SetPartsParameter(PartsParameters& _base, MechaPartsObject& _parts)
{
	auto&& weap = ChPtr::Make_S<PartsParameterStruct::SwordData>();
	SetWeaponData(*weap, _base, _parts);
	weap->attackTime = attackTime;
	weap->damageParSpeed = damageParSpeed;
	_base.weaponData.push_back(weap);
}
