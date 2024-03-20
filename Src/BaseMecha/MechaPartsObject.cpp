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

void MechaPartsObject::CreateFramePosture(ChCpp::FrameObject* _frameObject)
{
	if (_frameObject == nullptr)return;

}

void MechaPartsObject::Release()
{
	for (auto&& function : externulFunctions)
	{
		if (function == nullptr)continue;
		function->Release();
	}

	for (auto&& function : weaponFunctions)
	{
		if (function == nullptr)continue;
		function->Release();
	}

	for (auto&& partsObject : positions)
	{
		partsObject.second->Release();
	}
	positions.clear();
}

ChPtr::Shared<ChCpp::JsonObject> MechaPartsObject::Serialize()
{
	auto&& res = ChPtr::Make_S< ChCpp::JsonObject>();
	res->Set(JSON_PROPEATY_PARTS_NAME, ChCpp::JsonString::CreateObject(baseParts->GetThisFilePath()));

	if (GetRWeapon())
		res->Set(JSON_PROPEATY_RIGHT_WEAPON, ChCpp::JsonBoolean::CreateObject(true));

	if (GetLWeapon())
		res->Set(JSON_PROPEATY_LEFT_WEAPON, ChCpp::JsonBoolean::CreateObject(true));


	for (auto&& partsObject : positions)
	{
		res->Set(partsObject.first, partsObject.second->Serialize());
	}
	return res;
}

void MechaPartsObject::SetHitSize()
{

	ChVector3 tmpHitSize = GetColliderSize();
	auto positionObject = GetPositionObject();

	if (positionObject != nullptr)tmpHitSize += positionObject->GetDrawLHandMatrix().GetPosition();

	mecha->SetTestHitSize(tmpHitSize);


}

void MechaPartsObject::SetPositionObjectRotationYAxis(float _rot)
{

}

void MechaPartsObject::SetPositionObjectRotationXAxis(float _rot)
{

}

void MechaPartsObject::SetPositionObjectRotationZAxis(float _rot)
{

}

void MechaPartsObject::SetParentRotationYAxis(unsigned long _no, float _rot)
{

}

void MechaPartsObject::SetParentRotationXAxis(unsigned long _no, float _rot)
{

}

void MechaPartsObject::SetParentRotationZAxis(unsigned long _no, float _rot)
{

}

std::wstring MechaPartsObject::GetPartsName()
{
	std::wstring result = ChStr::UTF8ToWString(baseParts->GetThisFileName());
	unsigned long extensionStringPos = result.find_last_of('.');
	unsigned long fileNameLength = result.length();

	for (unsigned long i = extensionStringPos; i < fileNameLength; i++)
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
	for (auto&& func : externulFunctions)
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

void MechaPartsObject::UpdateFramePosture(ChCpp::FrameObject* _frameObject)
{
	if (_frameObject == nullptr)return;

}

void MechaPartsObject::Draw(const ChLMat& _drawMat)
{
	auto&& mesh = baseParts->GetMesh();

	//UpdateFramePosture(&mesh);

	ChLMat tmp;

	if (positionObject != nullptr)
	{
		tmp = positionObject->GetDrawLHandMatrix();
	}
	else
	{
		tmp = baseParts->GetDefaultFrameMat();
	}

	mesh.SetFrameTransform(tmp);

	tmp.Identity();
	tmp.SetRotation(baseRot);

	ChMat_11 drawMat;
	drawMat = lastDrawMat  = tmp * _drawMat;

	baseParts->Draw(drawMat);

	collider.SetMatrix(lastDrawMat);

	for (auto func : externulFunctions)
	{
		func->PosUpdate();
	}

	for (auto func : weaponFunctions)
	{
		func->PosUpdate();
	}

	mecha->UpdateAnchor(GetLookAnchorNo(), lastDrawMat);

	for (auto&& partsObject : positions)
	{
		if (partsObject.second == nullptr)continue;
		partsObject.second->Draw(_drawMat);
	}
}

void  MechaPartsObject::DrawEnd()
{
	for (auto&& child : positions)
	{
		child.second->DrawEnd();
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
