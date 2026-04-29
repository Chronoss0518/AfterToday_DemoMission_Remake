#include"../BaseIncluder.h"

#include"../AllStruct.h"
#include"../Frames/GameFrame.h"
#include"../Attack/Attack.h"
#include"../Attack/AttackObject.h"
#include"MechaPartsObject.h"
#include"MechaParts.h"

#include"CPU/CPULooker.h"

#include"MechaPartsObjectFunction/WeaponFunction.h"

#include"FunctionComponent/WeaponComponent.h"
#include"FunctionComponent/CameraComponent.h"

void MechaPartsObject::CreateEnd()
{
	auto&& baseMecha = GetBaseMecha();

	auto&& positions = baseParts->GetPositionList();
	
	for (auto&& pos : positions)
	{
		baseMecha->AddSmokeCreatePos(pos.second->positionObject, this);
	}
}

void MechaPartsObject::CreateAnchor()
{
	SetLookAnchorNo(mecha->GetAnchorRegistNum());

	mecha->AddAnchorData(baseParts->GetMesh().GetInitAllFrameBoxSize() * 2.0f, ChLMat());
}

void MechaPartsObject::Release()
{
	TransformObject<wchar_t>::Release();

	auto&& baseMecha = GetBaseMecha();
	baseMecha->SubSmokeCreatePos(this);

	positions.clear();
}

void MechaPartsObject::AddChildObject(const std::wstring& _objectType, ChPtr::Shared<MechaPartsObject> _childObject)
{
	ChCpp::TransformObject<wchar_t>::UpdateDrawTransform();

	if (_childObject == nullptr)return;

	auto&& position = baseParts->GetPositionList()[_objectType];

	SetChild(_childObject);

	ChVec3 pos = ChVec3();

	ChLMat lmat = position->positionObject->GetDrawLHandMatrix();

	pos = lmat.Transform(pos);

	lmat.Identity();
	lmat = position->connectionRotate;
	lmat.SetPosition(pos);

	_childObject->SetFrameTransform(lmat);

	_childObject->UpdateDrawTransform();

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

	SerializeWeapon(res, JSON_PROPEATY_RIGHT_WEAPON, WeaponHandType::Right);

	SerializeWeapon(res, JSON_PROPEATY_LEFT_WEAPON, WeaponHandType::Left);

	for (auto&& partsObject : positions)
	{
		res->Set(partsObject.first, partsObject.second->Serialize());
	}

	return res;
}

void MechaPartsObject::SerializeWeapon(ChPtr::Shared<ChCpp::JsonObject<wchar_t>>& _obj, const std::wstring& _jsonParameterText, WeaponHandType _type)
{
	auto&& weaponCom = mecha->GetComponentObject<WeaponComponent>();

	auto weaponNumbers = ChPtr::Make_S<ChCpp::JsonArray<wchar_t>>();

	bool isRegist = false;

	for (size_t i = 0; i < weaponFunctions.size(); i++)
	{
		int val = weaponCom->GetWeaponCount(weaponFunctions[i], _type);
		if (val >= 0)isRegist = true;
		auto num = ChPtr::Make_S<ChCpp::JsonNumber<wchar_t>>();
		num->SetValue(val);
		num->SetOutputDecimalPointFlg(false);
		weaponNumbers->Add(num);
	}

	if (!isRegist)return;
	if (weaponNumbers->GetCount() <= 0)return;

	_obj->Set(_jsonParameterText, weaponNumbers);
}

void MechaPartsObject::SetHitSize()
{
	ChVector3 tmpHitSize = GetColliderSize();

	tmpHitSize += GetDrawLHandMatrix().GetPosition();

	mecha->SetTestHitSize(tmpHitSize);
}

void MechaPartsObject::SetLookTargetParts(MechaPartsObject* _weaponParts)
{
	lookTargetBasePosParts = _weaponParts;
	posPartsCount++;
}

float MechaPartsObject::GetCameraBaseHeight()
{
	auto&& mesh = baseParts->GetMesh();

	ChVec3 minPos = mesh.GetInitAllFrameMinPos();
	ChVec3 maxPos = mesh.GetInitAllFrameMaxPos();

	ChLMat beforeDrawMat = mecha->GetBeforeDrawMat();
	beforeDrawMat.Inverse();
	ChLMat tmpMat = GetDrawLHandMatrix() * beforeDrawMat;

	minPos = tmpMat.Transform(minPos);
	maxPos = tmpMat.Transform(maxPos);

	float res = minPos.y > maxPos.y ? minPos.y : maxPos.y;

	for (auto&& pos : positions)
	{
		float tmpRes = pos.second->GetGroundHeight();

		res = res > tmpRes ? res : tmpRes;
	}

	return res;
}

float MechaPartsObject::GetGroundHeight()
{
	auto&& mesh = baseParts->GetMesh();

	ChVec3 minPos = mesh.GetInitAllFrameMinPos();
	ChVec3 maxPos = mesh.GetInitAllFrameMaxPos();

	ChLMat beforeDrawMat = mecha->GetBeforeDrawMat();
	beforeDrawMat.Inverse();
	ChLMat tmpMat = GetDrawLHandMatrix() * beforeDrawMat;

	minPos = tmpMat.Transform(minPos);
	maxPos = tmpMat.Transform(maxPos);

	float res = minPos.y < maxPos.y ? minPos.y : maxPos.y;

	for (auto&& pos : positions)
	{
		float tmpRes = pos.second->GetGroundHeight();

		res = res < tmpRes ? res : tmpRes;
	}

	return res;
}

std::vector<MechaPartsObject*>MechaPartsObject::GetParentTree()
{
	std::vector<MechaPartsObject*>res;

	res.push_back(this);

	auto parent = GetParent();

	if (parent == nullptr)return res;

	auto mechaPartsParent = ChPtr::SharedSafeCast<MechaPartsObject>(parent);

	if (mechaPartsParent == nullptr)return res;

	auto&& tmpTree = mechaPartsParent->GetParentTree();

	for (size_t i = 0; i < tmpTree.size(); i++)
	{
		res.push_back(tmpTree[i]);
	}

	return res;
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

void MechaPartsObject::Update()
{
	if (GetParent() != nullptr)
	{
		ChLMat lmat = GetDrawLHandMatrix();

		ChLMat beforeDrawMat = mecha->GetBeforeDrawMat();

		beforeDrawMat.Inverse();
		lmat = lmat * beforeDrawMat;

		float x = std::abs(lmat.m[1][0]);
		float y = std::abs(lmat.m[1][1]);

		if (x > 0.1f || y > 0.1f)
		{
			if (x > y)
			{
				thisRotateType = RotateDirectionType::Horizontal;
				rotateInverseValue = lmat.m[1][0];
			}
			else
			{
				thisRotateType = RotateDirectionType::Vertical;
				rotateInverseValue = lmat.m[1][1];
			}
		}
	}

}

void MechaPartsObject::Move()
{
	UpdateLookTargetDirectionVertical();
	UpdateLookTargetDirectionHorizontal();

	lookTargetBasePosParts = nullptr;
	posPartsCount = 0;

	if (!isRotateFlg)
		SetOutSideTransform(ChLMat());

	UpdateDrawTransform();

	isRotateFlg = false;
}

void  MechaPartsObject::DrawBegin()
{
	TransformObject<wchar_t>::DrawBegin();
}

void MechaPartsObject::Draw3D()
{
	DrawBeginFunction();

	TransformObject<wchar_t>::Draw3D();

	auto lMat = GetDrawLHandMatrix();

	baseParts->Draw((ChMat_11)(lMat));

	collider.SetMatrix(lMat);

	mecha->UpdateAnchor(GetLookAnchorNo(), lMat);

	DrawEndFunction();
}

void  MechaPartsObject::DrawEnd()
{
	TransformObject<wchar_t>::DrawEnd();
}

void MechaPartsObject::UpdateLookTargetDirectionVertical()
{
	if (ChPtr::NullCheck(lookTargetBasePosParts))return;
	if (thisRotateType != RotateDirectionType::Vertical)return;

	ChVec3 lookPos = GetCameraLookPos();

	auto tmpMat = lookTargetBasePosParts->GetDrawLHandMatrix();

	if (posPartsCount > 1)tmpMat = GetDrawLHandMatrix();

	ChVec3 pos = tmpMat.Transform(ChVec3());

	ChVec3 dir = lookPos - pos;

#if true

	dir.y = 0.0f;

	if (!dir.Normalize())
		dir = ChVec3(0.0f, 0.0f, 1.0f);

	auto partsDir = tmpMat.TransformCoord(ChVec3(0.0f,0.0f,1.0f));
	partsDir.y = 0.0f;

	if (!partsDir.Normalize())
		partsDir = ChVec3(0.0f, 0.0f, 1.0f);

	ChQua tmpQua;
	tmpQua.SetRotation(partsDir, dir);

	ChVec3 useDir = tmpQua.GetMul(ChVec3(0.0f, 0.0f, 1.0f));
	auto rotate = GetRotationFromDir(useDir);

#else

	dir.Normalize();
	auto rotate = GetRotationFromDir(dir);

#endif

	float tmpDegree = ChMath::ToDegree(rotate.xzRad);

	SetRotation(rotate.xzRad);

}

void MechaPartsObject::UpdateLookTargetDirectionHorizontal()
{
	if (ChPtr::NullCheck(lookTargetBasePosParts))return;
	if (thisRotateType != RotateDirectionType::Horizontal)return;

	ChVec3 lookPos = GetCameraLookPos();

	auto tmpMat = lookTargetBasePosParts->GetDrawLHandMatrix();

	if (posPartsCount > 1)tmpMat = GetDrawLHandMatrix();

	ChVec3 pos = tmpMat.Transform(ChVec3());

	ChVec3 dir = lookPos - pos;

	dir.Normalize();

#if true

	ChVec3 partsDir = tmpMat.TransformCoord(ChVec3(0.0f, 0.0f, 1.0f));

	partsDir.Normalize();

	ChQua tmpQua;
	tmpQua.SetRotation(partsDir, dir);

	ChVec3 useDir = tmpQua.GetMul(ChVec3(0.0f, 0.0f, 1.0f));
	useDir.Normalize();

	auto rotate = GetRotationFromDir(useDir);

	if (dir.z > 0.0f)rotate.yRad = -rotate.yRad;

#else

	auto rotate = GetRotationFromDir(dir);

#endif

	float tmpDegree = -ChMath::ToDegree(rotate.yRad);

	SetRotation(rotate.yRad);

}

void MechaPartsObject::SetRotation(float _rad)
{
	ChLMat tmp;

	tmp.SetRotationYAxis(rotateInverseValue * _rad);

	tmp = tmp * GetOutSideTransformLMat();

	SetOutSideTransform(tmp);

	isRotateFlg = true;
}

ChVec3 MechaPartsObject::GetCameraLookPos()
{
	auto camCom = mecha->GetComponentObject<CameraComponent>();

	return camCom->GetLookPosition();
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
