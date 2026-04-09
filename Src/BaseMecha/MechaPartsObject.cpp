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

void MechaPartsObject::CreateEnd()
{
	auto&& baseMecha = GetBaseMecha();
	auto&& baseParts = GetBaseObject();

	auto&& positions = baseParts->GetPositionList();
	
	for (auto&& pos : positions)
	{
		baseMecha->AddSmokeCreatePos(pos.second->positionObject, this);
	}
}

void MechaPartsObject::CreateAnchor()
{
	SetLookAnchorNo(mecha->GetAnchorRegistNum());

	mecha->AddAnchorData(GetBaseObject()->GetMesh().GetInitAllFrameBoxSize() * 2.0f, ChLMat());
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
	ChCpp::TransformObject<wchar_t>::Update();

	if (_childObject == nullptr)return;

	auto&& position = baseParts->GetPositionList()[_objectType];

	SetChild(_childObject);
	_childObject->ChCpp::TransformObject<wchar_t>::Update();

	ChVec3 pos = ChVec3();

	ChLMat lmat = position->positionObject->GetDrawLHandMatrix();
	//lmat = position->connectionRotate * lmat;

	pos = lmat.Transform(pos);

	lmat.Identity();
	lmat = position->connectionRotate;
	lmat.SetPosition(pos);

	_childObject->SetFrameTransform(lmat);
	_childObject->thisRotateType = position->rotateType;

	//Up•ûŒü‚Ì’l‚ð•ÛŽ//
	_childObject->rotateDirection.val.Set(position->connectionRotate.m[1]);

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
	ChLMat tmp;

	float tmpRotate = 0.0f;

	if (thisRotateType == RotateDirectionType::Vertical)
		tmpRotate = rotateDirection.x * rotate;

	if (thisRotateType == RotateDirectionType::Horizontal)
		tmpRotate = rotateDirection.y * rotate;

	tmp.SetRotationYAxis(ChMath::ToRadian(tmpRotate));

	SetOutSideTransform(tmp);

	TransformObject<wchar_t>::Update();

}

void  MechaPartsObject::DrawBegin()
{
	TransformObject<wchar_t>::DrawBegin();

}

void MechaPartsObject::Draw3D()
{
	DrawBeginFunction();

	TransformObject<wchar_t>::Draw3D();

	baseParts->Draw((ChMat_11)(GetDrawLHandMatrix()));

	collider.SetMatrix(GetDrawLHandMatrix());

	mecha->UpdateAnchor(GetLookAnchorNo(), GetDrawLHandMatrix());

	DrawEndFunction();
}

void  MechaPartsObject::DrawEnd()
{
	TransformObject<wchar_t>::DrawEnd();

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
