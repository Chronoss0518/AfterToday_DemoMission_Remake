
#include"../../BaseIncluder.h"
#include"../../AllStruct.h"

#include"../../EditFrame/PartsParameters.h"

#include"GunData.h"

#include"../../Attack/Attack.h"

#include"../MechaPartsObjectFunction/WeaponFunction.h"


unsigned long GunData::Deserialize(const ChCpp::TextObject<wchar_t>& _text, const unsigned long _textPos)
{
	unsigned long textPos = WeaponDataBase::Deserialize(_text, _textPos);
	fireNum = ChStr::GetNumFromText<unsigned long>(_text.GetTextLine(textPos).c_str());
	bulletNum = ChStr::GetNumFromText<unsigned long>(_text.GetTextLine(textPos + 1).c_str());
	magazineNum = ChStr::GetNumFromText<unsigned long>(_text.GetTextLine(textPos + 2).c_str());
	reloadTime = ChStr::GetNumFromText<unsigned long>(_text.GetTextLine(textPos + 3).c_str());
	range = ChStr::GetNumFromText<unsigned char>(_text.GetTextLine(textPos + 4).c_str());
	frontDir.Deserialize<wchar_t>(_text.GetTextLine(textPos + 5).c_str());
	bulletFile = _text.GetTextLine(textPos + 6).c_str();

	return textPos + 7;

}

std::wstring GunData::Serialize()
{
	std::wstring res = L"";

	res = WeaponDataBase::Serialize();
	res += std::to_wstring(fireNum) + L"\n";
	res += std::to_wstring(bulletNum) + L"\n";
	res += std::to_wstring(magazineNum) + L"\n";
	res += std::to_wstring(reloadTime) + L"\n";
	res += std::to_wstring(range) + L"\n";
	res += frontDir.Serialize<wchar_t>(L",", L";");
	res += bulletFile + L"\n";

	return res;
}

void GunData::SetPartsParameter(BaseMecha& _base, MechaPartsObject& _parts, GameFrame* _frame)
{
	auto& thisParts = *LookObj<MechaParts>();

	auto function = ChPtr::Make_S<GunFunction>();

	function->SetPartsObject(&_parts);

	function->SetBaseMecha(&_base);

	function->SetBaseData(this);

	function->Init(thisParts.GetMeshDrawer(), ChD3D11::D3D11Device());

	function->SetFrmae(_frame);

	_parts.AddWeaponFunction(function);

	NextPosBase::SetPartsParameter(_base, _parts, _frame);

}

void GunData::SetPartsParameter(PartsParameters& _base)
{
	auto&& weap = ChPtr::Make_S<PartsParameterStruct::GunData>();
	SetWeaponData(*weap);
	weap->fireNum = fireNum;
	weap->bulletNum = bulletNum;
	weap->magazineNum = magazineNum;
	weap->reloadTime = reloadTime;
	weap->range = range;

	auto&& attackData = Attack::CreateAttackData(nullptr, ChD3D11::D3D11Device(), bulletFile);
	attackData->SetPartameter(*weap);

	_base.weaponData.push_back(weap);
}
