#include"../../BaseIncluder.h"

#include"../../AllStruct.h"
#include"../../Frames/GameFrame.h"
#include"../../Attack/Attack.h"
#include"../../Attack/AttackObject.h"
#include"../MechaPartsObject.h"

#include"../MechaPartsData/SwordData.h"
#include"../MechaPartsData/GunData.h"

#include"WeaponFunction.h"

#include<math.h>

#define SE_VOLUME_SIZE 0.1f

#define NOW_BULLET_NUM_TEXT_COUNT 5
#define NOW_MAGAZINE_TEXT_COUNT 2

void WeaponFunction::Release()
{
	se.Release();
}

void WeaponFunction::AttackUpdate()
{
	if (nowWaitTime < data->GetWaitTime())return;

	AttackFunction();

	nowWaitTime = 0;
}

void WeaponFunction::Update()
{
	nowWaitTime++;

	UpdateFunction();
}

std::wstring WeaponFunction::GetWeaponName()
{
	if (ChPtr::NullCheck(data))return L"";
	return data->GetWeaponName();
}

void SwordFunction::SetData(WeaponDataBase* _data)
{
	swordData = ChPtr::SafeCast<SwordData>(_data);
}

void SwordFunction::AttackFunction()
{

}

void SwordFunction::Init(ChD3D11::Shader::BaseDrawMesh11<wchar_t>* _drawer, ID3D11Device* _device)
{
	//attackData = Attack::CreateAttackData(_drawer, _device, swordData->GetObjectName());
}

void GunFunction::AttackFunction()
{
	if (reloadFlg)return;
	if (nowBulletNum <= 0)return;

	ChLMat tmpMat = attackPos->GetDrawLHandMatrix() * obj->GetDrawLHandMatrix();;

#if false
	//auto&& defaultMat = obj->GetParent()->GetBaseObject()->GetDefaultFrameMat();
	tmpMat = lastShotPos * obj->GetPositionLastDrawMat() * obj->GetLastDrawMat();

	//tmpMat = lastShotPos * defaultMat * tmpMat;
	
	tmpMat = tmpMat;
#endif
	frame->AddShotEffectObject(tmpMat.GetPosition());

	se.Stop();
	se.Play();

	se.InitMatrix(tmpMat);

	for (unsigned long i = 0; i < gunData->GetFireNum(); i++)
	{
		ChLMat useMat = tmpMat;

		{
			ChQua tmp;

			tmp.SetRotationYAxis(ChMath::ToRadian(static_cast<float>((rand() % (gunData->GetRange() * 2 + 1)) - gunData->GetRange() - 1) * 0.01f));
			tmp.AddRotationXAxis(ChMath::ToRadian(static_cast<float>((rand() % (gunData->GetRange() * 2 + 1)) - gunData->GetRange() - 1) * 0.01f));

			ChLMat rangeMat;
			rangeMat.SetRotation(tmp);

			useMat = rangeMat * useMat;
		}

		auto attackObject = ChPtr::Make_S<AttackObject>();

		attackObject->SetBulletData(attackData.get());
		attackObject->SetFrame(frame);
		
		attackObject->SetBaseMechaNo(mecha->GetMechaNo());
		attackObject->SetTeamNo(mecha->GetTeamNo());
		attackObject->Init(useMat);
		frame->AddBullet(attackObject);

		ChVec3 nockback = attackObject->GetMovePower() * 0.01f / -mecha->GetMass();

		mecha->AddMoveVector(nockback);

		nowBulletNum--;
		if (nowBulletNum <= 0)break;
	}

	if (nowBulletNum > 0)return;

	StartSubFunction();
}

void GunFunction::Init(ChD3D11::Shader::BaseDrawMesh11<wchar_t>* _drawer, ID3D11Device* _device)
{
	nowBulletNum = gunData->GetBulletNum();

	nowMagazineNum = gunData->GetMagazineNum();

	reloadFlg = false;

	attackData = Attack::CreateAttackDataFromBullet(_drawer, _device, gunData->GetUseBulletFile());

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

void GunFunction::SetData(WeaponDataBase* _data)
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

void GunFunction::DrawBegin()
{
	ChLMat tmpMat;
	tmpMat.SetRotationXAxis(ChMath::ToRadian(-90));
	//obj->GetParent()->GetPositionObject()->SetOutSideTransform(tmpMat);
}

void GunFunction::DrawEnd()
{
	if (attackPos != nullptr)
	{
		//auto&& defaultMat = obj->GetParent()->GetBaseObject()->GetDefaultFrameMat();
		//lastShotPos = shotPos->GetDrawLHandMatrix();
		
		//lastShotPos = defaultMat * shotPos->GetDrawLHandMatrix();

		float len = (attackPos->GetDrawLHandMatrix() * obj->GetDrawLHandMatrix()).GetZAxisDirection().GetLen();
		if (isnan(len))
		{
			ChLMat tmp;
			//tmp = obj->GetLastDrawMat();
			tmp = tmp * attackPos->GetDrawLHandMatrix();
			return;
		}
	}

	//obj->GetParent()->GetPositionObject()->SetOutSideTransform(ChLMat());
}

std::wstring GunFunction::GetBulletNum()
{
	std::wstring res = std::to_wstring(nowBulletNum);

	size_t len = res.length();
	for (size_t i = len; i < NOW_BULLET_NUM_TEXT_COUNT; i++)
	{
		res = L'0' + res;
	}

	return res;
}

std::wstring GunFunction::GetReloadCount()
{
	std::wstring res = std::to_wstring(nowMagazineNum);

	size_t len = res.length();
	for (size_t i = len; i < NOW_MAGAZINE_TEXT_COUNT; i++)
	{
		res = L'0' + res;
	}

	return res;
}
