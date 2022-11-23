#include"../BaseIncluder.h"

#include"../AllStruct.h"

#include"../Bullet/Bullet.h"
#include"WeaponObject.h"
#include"MechaParts.h"


void WeaponObject::Init()
{

}

void WeaponObject::ChangeAttackType()
{

}

void WeaponObject::Attack()
{

}


void WeaponObject::SubFunction()
{
	weaponFunc[useAttackType]->SubFunction();
}

void WeaponObject::Update()
{
	weaponFunc[useAttackType]->Update();
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