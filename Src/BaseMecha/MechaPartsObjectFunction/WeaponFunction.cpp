#include"../../BaseIncluder.h"

#include"../../AllStruct.h"
#include"../../Frames/GameFrame.h"
#include"../../Attack/Attack.h"
#include"../../Attack/AttackObject.h"
#include"../MechaPartsObject.h"
#include"../MechaParts.h"

#include"WeaponFunction.h"

#define SE_VOLUME_SIZE 0.1f

#define NOW_BULLET_NUM_TEXT_COUNT 5
#define NOW_MAGAZINE_TEXT_COUNT 2

void WeaponFunction::Release()
{
	se.Release();
}

void WeaponFunction::AttackUpdate()
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

std::wstring WeaponFunction::GetWeaponName()
{
	if (ChPtr::NullCheck(data))return L"";
	return ChStr::UTF8ToWString(data->GetWeaponName());
}

void SwordFunction::SetData(WeaponData* _data)
{
	swordData = ChPtr::SafeCast<SwordData>(_data);
}

void SwordFunction::AttackFunction()
{

}

void SwordFunction::Init(ChD3D11::Shader::BaseDrawMesh11* _drawer, ID3D11Device* _device)
{

	//attackData = Attack::CreateAttackData(_drawer, _device, swordData->GetObjectName());

}

void GunFunction::AttackFunction()
{
	if (reloadFlg)return;
	if (nowBulletNum <= 0)return;

	ChLMat tmpMat;
	tmpMat = obj->GetLastDrawMat();

	tmpMat = lastShotPos * tmpMat;

	frame->AddShotEffectObject(tmpMat.GetPosition());

	se.Stop();
	se.Play();

	se.InitMatrix(tmpMat);

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

		auto attackObject = ChPtr::Make_S<AttackObject>();

		attackObject->SetBulletData(attackData.get());
		attackObject->SetFrame(frame);
		
		attackObject->SetBaseMechaNo(mecha->GetMechaNo());
		attackObject->SetTeamNo(mecha->GetTeamNo());
		attackObject->Init(tmpMat);
		frame->AddBullet(attackObject);

		ChVec3 nockback = attackObject->GetMovePower() * 0.01f / -mecha->GetMass();

		mecha->AddMoveVector(nockback);

		nowBulletNum--;
		if (nowBulletNum <= 0)break;
	}

	if (nowBulletNum > 0)return;

	StartSubFunction();
}

void GunFunction::Init(ChD3D11::Shader::BaseDrawMesh11* _drawer, ID3D11Device* _device)
{
	nowBulletNum = gunData->GetBulletNum();

	nowMagazineNum = gunData->GetMagazineNum();

	reloadFlg = false;

	attackData = Attack::CreateAttackData(_drawer, _device, gunData->GetUseBulletFile());

	ChD3D::XAudioManager().LoadSound(se, SOUND_DIRECTORY(+gunData->GetSEFileName()));

	se.SetVolume(SE_VOLUME_SIZE);
}

void GunFunction::StartSubFunction()
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
	ChLMat tmpMat;
	tmpMat.SetRotationXAxis(ChMath::ToRadian(-90));
	obj->GetParent()->GetPositionObject()->SetOutSizdTransform(tmpMat);

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

std::wstring GunFunction::GetBulletNum()
{
	std::wstring res = std::to_wstring(nowBulletNum);

	unsigned long len = res.length();
	for (unsigned long i = len; i < NOW_BULLET_NUM_TEXT_COUNT; i++)
	{
		res = L'0' + res;
	}

	return res;
}

std::wstring GunFunction::GetReloadCount()
{
	std::wstring res = std::to_wstring(nowMagazineNum);

	unsigned long len = res.length();
	for (unsigned long i = len; i < NOW_MAGAZINE_TEXT_COUNT; i++)
	{
		res = L'0' + res;
	}

	return res;
}
