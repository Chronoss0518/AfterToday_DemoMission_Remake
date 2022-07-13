
#include"../ChDXLibrary/ChGameIncludeFile.h"
#include"../AllStruct.h"
#include"../Damage/Damage.h"
#include"../BaseRobot/BaseRobot.h"
#include"../BaseRobot/RobParts.h"
#include"HitPosList.h"

void Bullet::BInit()
{
	auto TmpData = ChPtr::Make_S<ChPushBack9>();


	TmpData->SetSmpXFile(ChMeMa.GetSmpXFile("Field"));
	//TmpData->SetLen(Pos.Base.HitLen);
	TmpData->SetLen(0.0f);
	Physical.push_back(TmpData);
	
}

void Bullet::Update(const ChVec3_9* _BPos, const float _ClearLen)
{
	float TmpLen;
	TmpLen = Pos.Pos.GetLen(&StartPos);
	if (TmpLen > _ClearLen)ClearFlg = ChStd::True;


	{
		ChVec3_9 TmpVec;
		ChVec3_9 TmpPos;

		TmpVec = Pos.Dir * Pos.Base.Speed;

		for (unsigned char i = 0; i < Physical.size(); i++)
		{
			if (Physical[i]->UpDate(&Pos.Pos, &TmpVec))
			{
				BreakFlg = ChStd::True;
			}
			Pos.Pos += (Pos.Dir * Pos.Base.Speed);
		}


	}

	

	if (ChObjCon9.SmpXFileHitRay(
		ChMeMa.GetSmpXFile("Field")
		, &ChMat_9()
		, &Pos.Pos
		, &ChVec3_9(0.0f, 1.0f, 0.0f)
		, NULL
		, &TmpLen)) {

		if (TmpLen >= Pos.Base.HitLen)
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
	ChMat_9 TmpMat;
	TmpMat.ScalingMode(4.0f);
	DData = ChPtr::Make_S<Damage>();
	DDataList = ChPtr::Make_S<DDataBase>();
	DDataList->Init();
	DDataList->SharedDamageData(DData);

	ChMeMa.SetSmpXFile("AL120mmBullet.x", "B120mm", "Bullet");
	ChMeMa.GetXFileMaterials("B120mm")[0]->Mat = TmpMat;
	ChMeMa.GetXFileMaterials("B120mm")[0]->Diffuse.a = 1.0f;
	ChMeMa.GetXFileMaterials("B120mm")[0]->Diffuse.r = 2.0f;
	ChMeMa.GetXFileMaterials("B120mm")[0]->Diffuse.g = 2.0f;
	ChMeMa.GetXFileMaterials("B120mm")[0]->Diffuse.b = 2.0f;

}

void HitPosList::SetAData(const ChPtr::Shared<ChGame::APosData>_AData)
{

	if (_AData == nullptr)return;
	ChPtr::Shared<Bullet> TmpBullet = ChPtr::Make_S<Bullet>();

	if (TmpBullet->SetAData(_AData)) {
		TmpBullet->BInit();
		BList.push_back(TmpBullet);
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
		ChMat_9 TmpMat;
		TmpMat.RotVec(&AData.Dir);
		TmpMat = AData.Pos;

		ChMeMa.DrawSmpXFile(&TmpMat, "B120mm");
	}
	DDataList->Draw();
	ChDevice.LightSetting(ChStd::True);
}
