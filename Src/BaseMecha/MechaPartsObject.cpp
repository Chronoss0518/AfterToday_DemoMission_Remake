#include"../BaseIncluder.h"

#include"../AllStruct.h"
#include"../Frames/GameFrame.h"
#include"../Attack/Attack.h"
#include"../Attack/AttackObject.h"
#include"MechaPartsObject.h"
#include"MechaParts.h"

#include"MechaPartsObjectFunction/WeaponFunction.h"

void MechaPartsObject::Release()
{
	for (auto&& function : externulFunctions)
	{
		if (function == nullptr)continue;
		function->Release();
	}
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
}

void MechaPartsObject::Draw(const ChLMat& _drawMat)
{
	auto&& mesh = baseParts->GetMesh();
	ChLMat tmp;

	if (positionObject != nullptr)
	{
		tmp = positionObject->positionObject->GetDrawLHandMatrix();
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

}

void MechaPartsObject::Damage()
{

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
	if (!collider.IsHit(&_sphereCollider))return 0.0f;

	float res = 0;

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
