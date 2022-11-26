#include"../BaseIncluder.h"

#include"../AllStruct.h"

#include"../Bullet/Bullet.h"
#include"MechaPartsObject.h"
#include"MechaParts.h"

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
		positionObject->UpdateAllDrawTransform();
		lastDrawMat = positionObject->GetDrawLHandMatrix();
	}

	mesh.SetOutSizdTransform(lastDrawMat);

	ChMat_11 drawMat;
	drawMat = _drawMat;

	_meshDrawer.drawer.Draw(_meshDrawer.dc, mesh, drawMat);

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

void WeaponFunction::Attack()
{

	AttackFunction();
}

void WeaponFunction::SetBaseData(WeaponData* _data)
{
	data = _data;

	SetData(_data);
	Init();
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

}

void GunFunction::Init()
{
	nowBulletNum = gunData->GetBulletNum();

	nowMagazineNum = gunData->GetMagazineNum();
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