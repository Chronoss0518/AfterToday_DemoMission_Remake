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
	ChLMat tmp;
	tmp.Identity();

	if (positionObject != nullptr)
	{
		tmp = positionObject->positionObject->GetDrawLHandMatrix();
	}

	mesh.SetOutSizdTransform(tmp);

	tmp.Identity();
	tmp.SetRotation(baseRot);

	ChMat_11 drawMat;
	drawMat = tmp * _drawMat;
	lastDrawMat = drawMat;
	baseParts->Draw(drawMat);

	collider.SetPosition(lastDrawMat.GetPosition());
	collider.SetRotation(lastDrawMat.GetRotation());

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

float MechaPartsObject::GetDamage(ChCpp::SphereCollider& _sphereCollider, BulletObject& _bullet)
{
	if (!collider.IsHit(&_sphereCollider))return 0.0f;

	float res = 0;

	unsigned long tmp = baseParts->GetHardness() - _bullet.GetPenetration();

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

void WeaponFunction::Attack()
{
	if (nowWeatTime < data->GetWeatTime())return;;

	AttackFunction();

	nowWeatTime = 0;
}

void WeaponFunction::Update()
{
	nowWeatTime++;

	UpdateFunction();
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
	if (reloadFlg)return;
	if (nowBulletNum <= 0)return;

	ChLMat tmpMat;
	tmpMat = obj->GetLastDrawMat();

	tmpMat = lastShotPos * tmpMat;

	for (unsigned long i = 0; i < gunData->GetFireNum(); i++)
	{

		{
			ChVec3 tmp;

			tmp.x = ChMath::ToRadian(static_cast<float>((rand() % (gunData->GetRange() * 2 + 1)) - gunData->GetRange() - 1) * 0.01f);
			tmp.y = ChMath::ToRadian(static_cast<float>((rand() % (gunData->GetRange() * 2 + 1)) - gunData->GetRange() - 1) * 0.01f);

			ChLMat rangeMat;
			rangeMat.SetRotation(tmp);

			tmpMat = rangeMat * tmpMat;

		}

		auto bullet = ChPtr::Make_S<BulletObject>();

		bullet->SetBulletData(createBulletData.get());
		bullet->SetFrame(frame);
		bullet->SetBaseMecha(mecha);
		bullet->Init(tmpMat);
		frame->AddBullet(bullet);

		ChVec3 nockback = bullet->GetMovePower() * 0.01f / -mecha->GetMass();

		mecha->AddMoveVector(nockback);

		nowBulletNum--;
		if (nowBulletNum <= 0)break;
	}

	if (nowBulletNum > 0)return;

	SubFunction();
}

void GunFunction::Init(MeshDrawer* _drawer, ID3D11Device* _device)
{
	nowBulletNum = gunData->GetBulletNum();

	nowMagazineNum = gunData->GetMagazineNum();

	reloadFlg = false;

	createBulletData = BulletData::CreateBullet(_drawer, _device, gunData->GetUseBulletFile());

}

void GunFunction::SubFunction()
{
	if (nowMagazineNum <= 0)return;
	if (reloadFlg)return;

	nowReloadTime = 0;

	reloadFlg = true;
}

void GunFunction::SetData(WeaponData* _data)
{
	gunData = ChPtr::SafeCast<GunData>(_data);
}

void GunFunction::UpdateFunction()
{
	if (!reloadFlg)return;

	nowReloadTime++;

	if (gunData->GetReloadTime() > nowReloadTime)return;

	nowBulletNum = gunData->GetBulletNum();

	nowMagazineNum--;

	reloadFlg = false;


}

void GunFunction::PosUpdate()
{
	if (shotPos == nullptr)return;

	lastShotPos = shotPos->GetDrawLHandMatrix();
}
