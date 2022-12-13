#include"../BaseIncluder.h"

#include"../AllStruct.h"
#include"../Frames/GameFrame.h"
#include"../Bullet/Bullet.h"
#include"../Bullet/BulletObject.h"
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

void MechaPartsObject::Draw(const ChLMat& _drawMat)
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

	baseParts->Draw(drawMat);

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
	weaponFunc[useAttackType]->Attack();
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

void SwordFunction::Init(MeshDrawer* _drawer, ID3D11Device* _device)
{

}

void GunFunction::AttackFunction()
{
	//if (reloadFlg)return;
	//if (nowBulletNum <= 0)return;

	ChLMat tmpMat = obj->GetLastDrawMat();

	tmpMat = lastShotPos * tmpMat;

	for (unsigned long i = 0; i < gunData->GetFireNum(); i++)
	{

		auto bullet = ChPtr::Make_S<BulletObject>();

		bullet->SetBulletData(createBulletData.get());
		bullet->SetPosition(tmpMat.GetPosition());
		bullet->SetRotation(tmpMat.GetRotation());

		bullet->Init();
		frame->AddBullet(bullet);

		nowBulletNum--;
		if (nowBulletNum <= 0)break;
	}

	if (nowMagazineNum <= 0)return;
	if (nowBulletNum > 0)return;


}

void GunFunction::Init(MeshDrawer* _drawer, ID3D11Device* _device)
{
	nowBulletNum = gunData->GetBulletNum();

	nowMagazineNum = gunData->GetMagazineNum();

	createBulletData = BulletData::CreateBullet(_drawer, _device, gunData->GetUseBulletFile());

}

void GunFunction::SubFunction()
{
	if (!reloadFlg)return;

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
	
	lastShotPos = shotPos->GetDrawLHandMatrix();
}