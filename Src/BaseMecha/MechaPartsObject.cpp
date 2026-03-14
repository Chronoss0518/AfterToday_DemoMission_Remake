#include"../BaseIncluder.h"

#include"../AllStruct.h"
#include"../Frames/GameFrame.h"
#include"../Attack/Attack.h"
#include"../Attack/AttackObject.h"
#include"MechaPartsObject.h"
#include"MechaParts.h"

#include"CPU/CPULooker.h"

#include"MechaPartsObjectFunction/WeaponFunction.h"

void MechaPartsObject::CreateAnchor()
{
	SetLookAnchorNo(mecha->GetAnchorRegistNum());

	mecha->AddAnchorData(GetBaseObject()->GetMesh().GetInitAllFrameBoxSize() * 2.0f, ChLMat());
}

void MechaPartsObject::Release()
{
	TransformObject<wchar_t>::Release();

	for (auto&& function : externalFunctions)
	{
		if (function == nullptr)continue;
		function->Release();
	}

	for (auto&& function : weaponFunctions)
	{
		if (function == nullptr)continue;
		function->Release();
	}

	positions.clear();
}

void MechaPartsObject::AddChildObject(const std::wstring& _objectType, ChPtr::Shared<MechaPartsObject> _childObject)
{
	ChCpp::TransformObject<wchar_t>::Update();

	if (_childObject == nullptr)return;

	auto&& position = baseParts->GetPositionList()[_objectType];

	_childObject->SetParent(shared_from_this());
	_childObject->ChCpp::TransformObject<wchar_t>::Update();

	ChVec3 pos = ChVec3();
	ChLMat lmat = position->positionObject->GetDrawLHandMatrix();
	lmat = position->connectionRotate * lmat;

	pos = lmat.Transform(pos);

	lmat.Identity();
	lmat.SetPosition(pos);

	_childObject->SetFrameTransform(lmat);

	auto&& tmpObject = positions.find(_objectType);
	if (tmpObject == positions.end())
	{
		positions[_objectType] = _childObject;

		return;
	}

	if (tmpObject->second != nullptr)
	{
		positions.erase(tmpObject);
		positions[_objectType] = _childObject;

		return;
	}

	tmpObject->second = _childObject;

}


ChPtr::Shared<ChCpp::JsonObject<wchar_t>> MechaPartsObject::Serialize()
{
	auto&& res = ChPtr::Make_S< ChCpp::JsonObject<wchar_t>>();
	res->Set(JSON_PROPEATY_PARTS_NAME, ChCpp::JsonString<wchar_t>::CreateObject(baseParts->GetThisFilePath()));

	if (GetRWeapon())
		res->Set(JSON_PROPEATY_RIGHT_WEAPON, ChCpp::JsonBoolean<wchar_t>::CreateObject(true));

	if (GetLWeapon())
		res->Set(JSON_PROPEATY_LEFT_WEAPON, ChCpp::JsonBoolean<wchar_t>::CreateObject(true));


	for (auto&& partsObject : positions)
	{
		res->Set(partsObject.first, partsObject.second->Serialize());
	}
	return res;
}

void MechaPartsObject::SetHitSize()
{
	ChVector3 tmpHitSize = GetColliderSize();

	tmpHitSize += GetDrawLHandMatrix().GetPosition();

	mecha->SetTestHitSize(tmpHitSize);
}

std::wstring MechaPartsObject::GetPartsName()
{
	std::wstring result = baseParts->GetThisFileName();
	size_t extensionStringPos = result.find_last_of(L'.');
	size_t fileNameLength = result.length();

	for (size_t i = extensionStringPos; i < fileNameLength; i++)
	{
		result.pop_back();
	}

	return result;
}

std::wstring MechaPartsObject::GetWeaponName()
{
	std::wstring result = L"";
	if (!weaponFunctions.empty())
		if (weaponFunctions.size() > useAttackType)
			result = weaponFunctions[useAttackType]->GetWeaponName();

	return result;
}

std::wstring MechaPartsObject::GetNowBulletNum()
{
	if (weaponFunctions.empty())return WeaponFunction::GetDefaultBulletNum();
	if (weaponFunctions.size() <= useAttackType)return WeaponFunction::GetDefaultBulletNum();

	return weaponFunctions[useAttackType]->GetBulletNum();
}

std::wstring MechaPartsObject::GetReloadCount()
{
	if (weaponFunctions.empty())return WeaponFunction::GetDefaultReloadCount();
	if (weaponFunctions.size() <= useAttackType)return WeaponFunction::GetDefaultReloadCount();

	return weaponFunctions[useAttackType]->GetReloadCount();
}

void MechaPartsObject::Update()
{
	TransformObject<wchar_t>::Update();

	for (auto&& func : externalFunctions)
	{
		func->Update();
	}

	for (auto&& func : weaponFunctions)
	{
		func->Update();
	}

	for (auto&& partsObject : positions)
	{
		partsObject.second->Update();
	}
}

void  MechaPartsObject::DrawBegin()
{
	TransformObject<wchar_t>::DrawBegin();

	for (auto func : externalFunctions)
	{
		func->DrawBegin();
	}

	for (auto func : weaponFunctions)
	{
		func->DrawBegin();
	}
}

void MechaPartsObject::Draw3D()
{
	TransformObject<wchar_t>::Draw3D();

	baseParts->Draw((ChMat_11)(GetDrawLHandMatrix()));

	collider.SetMatrix(GetDrawLHandMatrix());

	mecha->UpdateAnchor(GetLookAnchorNo(), GetDrawLHandMatrix());
}

void  MechaPartsObject::DrawEnd()
{
	TransformObject<wchar_t>::DrawEnd();

	for (auto func : externalFunctions)
	{
		func->DrawEnd();
	}

	for (auto func : weaponFunctions)
	{
		func->DrawEnd();
	}
}

float MechaPartsObject::GetDamage(AttackObject& _bullet)
{
	float res = 0;

	unsigned long tmp = _bullet.GetPenetration() - baseParts->GetHardness();

	tmp = tmp < 0 ? 0 : tmp;

	_bullet.SetIsHitTrue();

	res = static_cast<float>(tmp);

	return res;
}

float MechaPartsObject::GetDamage(ChCpp::SphereCollider& _sphereCollider, AttackObject& _bullet)
{
	float res = 0.0f;

	if (!collider.IsHit(&_sphereCollider))
	{
		for (auto&& partsObject : positions)
		{
			res = partsObject.second->GetDamage(_sphereCollider, _bullet);
			if (res <= 0.0f)continue;
			return res;
		}
		return res;
	}


	unsigned long tmp = _bullet.GetPenetration() - baseParts->GetHardness();

	tmp = tmp < 0 ? 0 : tmp;

	_bullet.SetIsHitTrue();

	res = static_cast<float>(tmp);

	return res;
}

float MechaPartsObject::GetDamage(ChCpp::BoxCollider& _collider)
{
	if (!collider.IsHit(&_collider))return 0.0f;

	return 0.0f;
}

void MechaPartsObject::AttackUpdate()
{
	weaponFunctions[useAttackType]->AttackUpdate();
}

void MechaPartsObject::StartWeaponSubFunction()
{
	weaponFunctions[useAttackType]->StartSubFunction();
}
