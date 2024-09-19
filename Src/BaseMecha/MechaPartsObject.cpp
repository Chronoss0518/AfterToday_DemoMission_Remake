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

void MechaPartsObject::CreatePostureList()
{
	if (controllerList.empty())controllerList.clear();

	MechaPartsObject* tmpObj = this;
	while (true)
	{
		ChPtr::Shared<ChCpp::FrameObject> controller = tmpObj->GetPositionObject();

		tmpObj = tmpObj->GetParent();
		if (ChPtr::NullCheck(tmpObj))break;

		while (controller != nullptr)
		{
			auto&& controllerObj = controller->GetComponent<PostureController>();
			controller = ChPtr::SharedSafeCast<ChCpp::FrameObject>(controller->GetParent().lock());

			if (controllerObj == nullptr)continue;

			auto axis = ChStd::EnumCast(controllerObj->GetRotateAxis());
			axis %= AXIS_BASE_TYPE_NUM;

			if (axis == (ChStd::EnumCast(RotateAxis::PZ) % AXIS_BASE_TYPE_NUM))continue;

			auto&& con = ChPtr::Make_S<ControllerListItem>();
			con->controller = controllerObj;
			con->partsObejct = tmpObj;
			controllerList.push_back(con);

			if (startRotatePosture != StartRotatePosture::None)continue;

			if (axis == (ChStd::EnumCast(RotateAxis::PX) || axis == (ChStd::EnumCast(RotateAxis::MX))))startRotatePosture = StartRotatePosture::XY;
			if (axis == (ChStd::EnumCast(RotateAxis::PY) || axis == (ChStd::EnumCast(RotateAxis::MY))))startRotatePosture = StartRotatePosture::YX;
		}


	}

	for (auto&& child : positions)
	{
		child.second->CreatePostureList();
	}


}

void MechaPartsObject::Release()
{
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

void  MechaPartsObject::DrawStart()
{

	for (auto&& posture : baseParts->GetPostureControllerList())
	{
		auto&& controller = posture.lock();

		auto&& frame = controller->GetLookObject();

		auto&& postureRotate = postureRotateList.find(frame);

		if (postureRotate == postureRotateList.end())continue;

		controller->SetRotate(ChMath::ToRadian(postureRotate->second->rotate));
	}
}

void MechaPartsObject::DrawStartFunction()
{
	DrawStart();

	FunctionDrawBegin();
}

void MechaPartsObject::Draw(const ChLMat& _drawMat)
{
	DrawStartFunction();

	positionLastDrawMat = ChLMat();
	if (positionObject != nullptr)
	{

		auto&& mesh = baseParts->GetMesh();

		//tmp = positionObject->GetDrawLHandMatrix() * tmp;
		positionLastDrawMat = parentObject->GetBaseObject()->GetDefaultFrameMat() * positionObject->GetDrawLHandMatrix();
		//positionLastDrawMat = positionObject->GetDrawLHandMatrix();
		//mesh.SetOutSizdTransform(positionLastDrawMat);
		mesh = baseParts->GetMesh();
	}

	lastDrawMat = _drawMat;

	//ChLMat drawMat = positionLastDrawMat * lastDrawMat;
	ChLMat drawMat = positionLastDrawMat * lastDrawMat;
	//ChLMat drawMat = lastDrawMat;

	baseParts->Draw((ChMat_11)(drawMat));

	collider.SetMatrix(drawMat);

	mecha->UpdateAnchor(GetLookAnchorNo(), drawMat);

	for (auto&& partsObject : positions)
	{
		if (partsObject.second == nullptr)continue;
		partsObject.second->Draw(drawMat);
	}

	DrawEnd();

	isInitRunFlg = true;
}

void  MechaPartsObject::DrawEnd()
{
	FunctionDrawEnd();

	for (auto&& posture : baseParts->GetPostureControllerList())
	{
		auto&& controller = posture.lock();
		if (controller == nullptr)continue;
		controller->SetRotate(0.0f);

		auto&& frame = controller->GetLookObject();
		auto&& postureRotate = postureRotateList.find(frame);
		if (postureRotate == postureRotateList.end())continue;
		if (postureRotate->second->updateFlg)continue;
		postureRotate->second->updateFlg = false;
		postureRotate->second->rotate *= 0.1f;
	}
}

void MechaPartsObject::FunctionDrawBegin()
{
	for (auto func : externalFunctions)
	{
		func->DrawBegin();
	}

	for (auto func : weaponFunctions)
	{
		func->DrawBegin();
	}
}

void MechaPartsObject::FunctionDrawEnd()
{
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
