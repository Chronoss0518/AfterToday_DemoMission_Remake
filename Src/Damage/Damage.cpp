#include"../ChDXLibrary/ChGameIncludeFile.h"
#include"Damage.h"

Damage::Damage()
{

}

Damage::~Damage()
{

	DEffectList.clear();

}


void Damage::Update()
{

	for (unsigned char i = 0; i < DEffectList.size(); i++)
	{
		DEffectList[i]->Rot += 5.0f;
		DEffectList[i]->Scl += MaxScl / TimeCnt;
		DEffectList[i]->Alpha -= 2.0f / TimeCnt;

		if (DEffectList[i]->Scl > MaxScl)
		{
			DEffectList.erase(DEffectList.begin() + i);
			i--;
			continue;
		}

		if (DEffectList.size() > 3)
		{
			DEffectList.erase(DEffectList.begin());
		}
		//DEffectList[i]->Alpha = 0.0f;


	}
	//DEffectList.clear();

}

void Damage::SetDPos(const ChVec3_9 *_Pos, const ChStd::Bool _DFlg)
{
	ChPtr::Shared<DData>TmpDate = std::make_shared<DData>();
	TmpDate->Pos = *_Pos;
	TmpDate->Rot = 0.0f;
	TmpDate->Scl = 1.0f;
	TmpDate->Alpha = 2.0f;
	TmpDate->DataName = "Burn";
	ChDSound9.PlaySE(SEName, "Sound", _Pos);
	if (_DFlg)
	{
		MaxScl = 50.0f;
	}
	DEffectList.push_back(TmpDate);

}

void DDataBase::Init()
{

	ChMeMa.SetSmpXFile("Burn.x", "Burn", "Base");

}

void DDataBase::Release()
{

}

void DDataBase::Update()
{
	for (auto Obj : DList)
	{
		Obj->Update();
	}
}

void DDataBase::Draw()
{

	ChEffectDraw.LightCheck(FALSE);
	for (auto Obj : DList)
	{

		ChMat_9 TmpMat;
		ChMat_9 TmpRot;
		float TmpScl;

		for (auto It = Obj->GetDData(); It != Obj->GetEndDData(); It++)
		{
			TmpScl = (*It)->Scl;
			TmpMat.ScalingMode(TmpScl, TmpScl, TmpScl);
			TmpMat = ((*It)->Pos);
			TmpRot.RotYPR(0.0f, (*It)->Rot, 0.0f);
			TmpMat = TmpRot * TmpMat;

			//XList->SetAlpha(DEffectList[i]->Alpha, DEffectList[i]->DateNo);
			ChStd::COLOR1f Col(
				(*It)->Alpha
				, (*It)->Alpha
				, (*It)->Alpha
				, (*It)->Alpha);

			ChMeMa.DrawSmpXFile(&TmpMat, (*It)->DataName);
/*
			ChEffectDraw.SmpXfileNormalDraw(
				ChMeMa.GetSmpXFile((*It)->DataName)
				, &TmpMat
				, ChNoEffect);
*/
		}

		ChEffectDraw.LightCheck(TRUE);
	}
}