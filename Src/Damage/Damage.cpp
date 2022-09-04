#include<Windows.h>
#include<ChBaseLibrary.h>
#include<ChWindowsLibrary.h>
#include<ChDirect3D11Library.h>

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

void Damage::SetDPos(const ChVec3 *_Pos, const ChStd::Bool _DFlg)
{
	ChPtr::Shared<DData>tmpDate = std::make_shared<DData>();
	tmpDate->Pos = *_Pos;
	tmpDate->Rot = 0.0f;
	tmpDate->Scl = 1.0f;
	tmpDate->Alpha = 2.0f;
	tmpDate->DataName = "Burn";
	ChDSound9.PlaySE(SEName, "Sound", _Pos);
	if (_DFlg)
	{
		MaxScl = 50.0f;
	}
	DEffectList.push_back(tmpDate);

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

		ChLMat tmpMat;
		ChLMat tmpRot;
		float tmpScl;

		for (auto It = Obj->GetDData(); It != Obj->GetEndDData(); It++)
		{
			tmpScl = (*It)->Scl;
			tmpMat.ScalingMode(tmpScl, tmpScl, tmpScl);
			tmpMat = ((*It)->Pos);
			tmpRot.RotYPR(0.0f, (*It)->Rot, 0.0f);
			tmpMat = tmpRot * tmpMat;

			//XList->SetAlpha(DEffectList[i]->Alpha, DEffectList[i]->DateNo);
			ChStd::COLOR1f Col(
				(*It)->Alpha
				, (*It)->Alpha
				, (*It)->Alpha
				, (*It)->Alpha);

			ChMeMa.DrawSmpXFile(&tmpMat, (*It)->DataName);
/*
			ChEffectDraw.SmpXfileNormalDraw(
				ChMeMa.GetSmpXFile((*It)->DataName)
				, &tmpMat
				, ChNoEffect);
*/
		}

		ChEffectDraw.LightCheck(TRUE);
	}
}