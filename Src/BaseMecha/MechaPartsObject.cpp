#include"../BaseIncluder.h"

#include"../AllStruct.h"

#include"../Bullet/Bullet.h"
#include"MechaPartsObject.h"
#include"MechaParts.h"

using PositionObject = PositionObject;

void MechaPartsObject::Update()
{
	for (auto&& weapon : weaponFunc)
	{
		weapon->Update();
	}
}

void MechaPartsObject::Draw(MeshDrawer& _meshDrawer, const ChLMat& _drawMat)
{
	auto&& mesh = baseParts->GetMesh();
	lastDrawMat.Identity();

	if (positionObject != nullptr)
	{
		positionObject->positionObject->UpdateAllDrawTransform();
		lastDrawMat = positionObject->positionObject->GetDrawLHandMatrix();
	}

	mesh.SetOutSizdTransform(lastDrawMat);

	ChMat_11 drawMat;
	ChMat_11 rot;
	rot.SetRotation(baseRot);

	drawMat = _drawMat;
	drawMat = rot * drawMat;

	_meshDrawer.drawer.Draw(_meshDrawer.dc, mesh, drawMat);

	for (auto func : weaponFunc)
	{
		func->PosUpdate();
	}

}

void MechaPartsObject::Damage()
{

}

void MechaPartsObject::Attack()
{

}

void MechaPartsObject::StartSubFunction()
{
	weaponFunc[useAttackType]->SubFunction();
}

void MechaPartsObject::SetSwordHitObjectPos(ChPtr::Shared<ChCpp::FrameObject> _targetObject)
{
	auto func = ChPtr::SharedSafeCast<SwordFunction>(weaponFunc[weaponFunc.size() - 1]);

	if (func == nullptr)return;

	func->SetHitObjectStart(_targetObject);

}

void MechaPartsObject::SetGunShotPos(ChPtr::Shared<ChCpp::FrameObject> _targetObject)
{
	auto func = ChPtr::SharedSafeCast<GunFunction>(weaponFunc[weaponFunc.size() - 1]);

	if (func == nullptr)return;

	func->SetShotPos(_targetObject);
}

void WeaponFunction::Attack()
{

	AttackFunction();
}

void SwordFunction::SetData(WeaponData* _data)
{
	swordData = ChPtr::SafeCast<SwordData>(_data);
}

void SwordFunction::AttackFunction()
{

}

void SwordFunction::Init()
{

}

void GunFunction::AttackFunction()
{
	if (reloadFlg)return;
	if (nowBulletNum <= 0)return;



}

void GunFunction::Init()
{
	nowBulletNum = gunData->GetBulletNum();

	nowMagazineNum = gunData->GetMagazineNum();

	createBulletData = BulletData::CreateBullet(gunData->GetUseBulletFile());

}

void GunFunction::SubFunction()
{

}

void GunFunction::SetData(WeaponData* _data)
{
	gunData = ChPtr::SafeCast<GunData>(_data);
}

void GunFunction::Update()
{
	if (!reloadFlg)return;

}

void GunFunction::PosUpdate()
{
	if (shotPos == nullptr)return;
	
	shotPos->UpdateAllDrawTransform();

	lastShotPos = shotPos->GetDrawLHandMatrix();
}