#include"../../BaseIncluder.h"

#include"../../AllStruct.h"
#include"../../Frames/GameFrame.h"
#include"../../Attack/Attack.h"
#include"../../Attack/AttackObject.h"
#include"../MechaPartsObject.h"
#include"../FunctionComponent/CameraComponent.h"

#include"../Controller/ControllerBase.h"

#include"../MechaPartsData/SwordData.h"
#include"../MechaPartsData/GunData.h"

#include"WeaponFunction.h"

#include"../../Application/Application.h"

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

	ChLMat tmpMat = attackPos->GetDrawLHandMatrix() * parts->GetDrawLHandMatrix();

	ChVec3 tmpPos = tmpMat.Transform(ChVec3());

	tmpMat = parts->GetDrawLHandMatrix();
	tmpMat.SetPosition(tmpPos);

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

	auto&& mgr = AppIns().GetAudioManager();

	mgr.LoadSound(se, SOUND_DIRECTORY(+gunData->GetSEFileName()));

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

void GunFunction::SelectedUpdate()
{
	if (ChPtr::NullCheck(parts))return;
	if (mecha->GetComponent<ControllerBase>() == nullptr)return;
	if (!gunData->GetLookTargetFlg())return;

	auto tree = parts->GetParentTree();

	auto camCom = mecha->GetComponentObject<CameraComponent>();
	
	ChVec3 lookPos = camCom->GetLookPosition();

	bool useVerticalFlg = false;
	bool useHorizontalFlg = false;

	for (size_t i = 0; i < tree.size(); i++)
	{
		if (useVerticalFlg && useHorizontalFlg)break;

		auto tmpParts = tree[tree.size() - i - 1];
		
		if (tmpParts->GetThisRotateType() == RotateDirectionType::None)continue;

		if (tmpParts->GetThisRotateType() == RotateDirectionType::Vertical && !useVerticalFlg)
		{
			auto tmpMat = tmpParts->GetDrawLHandMatrix();

			ChVec3 pos = tmpMat.Transform(ChVec3());

			ChVec3 dir = lookPos - pos;

			dir.Normalize();

			auto rotate = GetRotationFromDir(dir);

			float tmpDegree = ChMath::ToDegree(rotate.xzRad);
			tmpParts->SetRotate(tmpDegree);
			useVerticalFlg = true;

			continue;
		}

		if (tmpParts->GetThisRotateType() == RotateDirectionType::Horizontal && !useHorizontalFlg)
		{
			auto tmpMat = tmpParts->GetDrawLHandMatrix();

			ChVec3 pos = tmpMat.Transform(ChVec3());

			ChVec3 dir = lookPos - pos;

			dir.Normalize();

			auto rotate = GetRotationFromDir(dir);

			float tmpDegree = ChMath::ToDegree(rotate.yRad);
			tmpParts->SetRotate(-tmpDegree);
			useHorizontalFlg = true;

			continue;
		}
	}

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
