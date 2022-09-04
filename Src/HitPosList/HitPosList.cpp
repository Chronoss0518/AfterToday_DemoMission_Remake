#include<Windows.h>
#include<ChBaseLibrary.h>
#include<ChWindowsLibrary.h>
#include<ChDirect3D11Library.h>

#include"../AllStruct.h"
#include"../Damage/Damage.h"
#include"../BaseRobot/BaseRobot.h"
#include"../BaseRobot/RobParts.h"
#include"HitPosList.h"

void Bullet::BInit()
{
	auto tmpData = ChPtr::Make_S<ChPushBack9>();


	tmpData->SetSmpXFile(ChMeMa.GetSmpXFile("Field"));
	//tmpData->SetLen(Pos.Base.HitLen);
	tmpData->SetLen(0.0f);
	Physical.push_back(tmpData);
	
}

void Bullet::Update(const 
	* _BPos, const float _ClearLen)
{
	float tmpLen;
	tmpLen = Pos.Pos.GetLen(&StartPos);
	if (tmpLen > _ClearLen)ClearFlg = ChStd::True;


	{
		ChVec3 tmpVec;
		ChVec3 tmpPos;

		tmpVec = Pos.Dir * Pos.Base.Speed;

		for (unsigned char i = 0; i < Physical.size(); i++)
		{
			if (Physical[i]->UpDate(&Pos.Pos, &tmpVec))
			{
				BreakFlg = ChStd::True;
			}
			Pos.Pos += (Pos.Dir * Pos.Base.Speed);
		}


	}

	

	if (ChObjCon9.SmpXFileHitRay(
		ChMeMa.GetSmpXFile("Field")
		, &ChLMat()
		, &Pos.Pos
		, &ChVec3(0.0f, 1.0f, 0.0f)
		, NULL
		, &tmpLen)) {

		if (tmpLen >= Pos.Base.HitLen)
		{
			BreakFlg = ChStd::True;
		}
	}
}

HitPosList::HitPosList()
{

}

HitPosList::~HitPosList()
{
	BList.clear();
	DDataList = nullptr;
	DData = nullptr;
}

void HitPosList::Init()
{
	ChLMat tmpMat;
	tmpMat.ScalingMode(4.0f);
	DData = ChPtr::Make_S<Damage>();
	DDataList = ChPtr::Make_S<DDataBase>();
	DDataList->Init();
	DDataList->SharedDamageData(DData);

	ChMeMa.SetSmpXFile("AL120mmBullet.x", "B120mm", "Bullet");
	ChMeMa.GetXFileMaterials("B120mm")[0]->Mat = tmpMat;
	ChMeMa.GetXFileMaterials("B120mm")[0]->Diffuse.a = 1.0f;
	ChMeMa.GetXFileMaterials("B120mm")[0]->Diffuse.r = 2.0f;
	ChMeMa.GetXFileMaterials("B120mm")[0]->Diffuse.g = 2.0f;
	ChMeMa.GetXFileMaterials("B120mm")[0]->Diffuse.b = 2.0f;

}

void HitPosList::SetAData(const ChPtr::Shared<ChGame::APosData>_AData)
{

	if (_AData == nullptr)return;
	ChPtr::Shared<Bullet> tmpBullet = ChPtr::Make_S<Bullet>();

	if (tmpBullet->SetAData(_AData)) {
		tmpBullet->BInit();
		BList.push_back(tmpBullet);
	}

}

void HitPosList::IsHitTest(ChPtr::Shared<RobPartsList> _RobParts)
{
	if (_RobParts == nullptr)return;

	for (auto Obj : BList)
	{
		Obj->SetHitFlg(_RobParts->IsPosHit(Obj->GetAPosData()));
	}

}

void HitPosList::UpDate()
{
	DDataList->Update();

	if (BList.empty())return;

	for (unsigned short i = 0; i < BList.size(); i++)
	{
		BList[i]->Update(BasePos,LenForClear);
		if (!BList[i]->IsArive())
		{
			DData->SetDPos(&BList[i]->GetAPosData()->Pos, ChStd::False);
			BList.erase(BList.begin() + i);
			i--;
			continue;
		}

		if (BList[i]->IsClear())
		{
			BList.erase(BList.begin() + i);
			i--;
			continue;
		}

		if (BList.size() > MaxAPos)
		{
			BList.erase(BList.begin());
			continue;
		}
	}

}

void HitPosList::Draw()
{
	ChDevice.LightSetting(ChStd::False);

	for (auto Obj : BList)
	{

		ChGame::APosData AData = *Obj->GetAPosData();
		ChLMat tmpMat;
		tmpMat.RotVec(&AData.Dir);
		tmpMat = AData.Pos;

		ChMeMa.DrawSmpXFile(&tmpMat, "B120mm");
	}
	DDataList->Draw();
	ChDevice.LightSetting(ChStd::True);
}
