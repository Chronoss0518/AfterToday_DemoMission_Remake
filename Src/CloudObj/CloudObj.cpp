
#include"../ChDXLibrary/ChGameIncludeFile.h"
#include"CloudObj.h"

const float BaseHigh = 300.0f;
const float MaxLen = 1000.0f;

void CloudList::Init()
{
	MoveDir = ChVec3_9(
		(ChStd::GetRand(3) * 2.0f - 2.0f)
		, 0.0f
		, (ChStd::GetRand(3) * 2.0f - 2.0f));

	MoveDir.Normalize();

	MoveSize = (ChStd::GetRand(9) + 1.0f) * 0.05f;


	ChTexMa.SetTexture("Cloud.png", "Cloud", "Base", 128, 128, 128, 128);

	for (unsigned char i = 0; i < StartCloudCnt; i++)
	{
		CloudBurth();
	}
}

void CloudList::Update()
{
	ChVec3_9 TmpVec, TmpPos;
	TmpVec = MoveDir * MoveSize;

	for (unsigned char i =0 ;i < Cloud_List.size();i++)
	{

		Cloud_List[i]->Move(&TmpVec);
		TmpPos = *Cloud_List[i]->GetPos();

		if (TmpPos.GetLen(&ChVec3_9(0.0f, BaseHigh, 0.0f)) < MaxLen)continue;
		Cloud_List.erase(Cloud_List.begin() + i);
		i--;

	}

	if (ChStd::GetRand(5) <= 1)CloudBurth();

	if (Cloud_List.size() >= MaxCloufdCnt)Cloud_List.pop_back();

	++ChengeCnt%=ChengeIntervalCnt;

	if (ChengeCnt != 0)return;
	if(ChStd::GetRand(100)<90)return;

	MoveDir = ChVec3_9(
		(ChStd::GetRand(3) * 2.0f - 2.0f)
		, 0.0f
		, (ChStd::GetRand(3) * 2.0f - 2.0f));

	MoveDir.Normalize();

	MoveSize = (ChStd::GetRand(9) + 1.0f) * 0.05f;



}

void CloudList::Draw()
{

	ChPoBo9.SetAlphaFlg(ChStd::True);
	for (auto Obj : Cloud_List)
	{
		Obj->Draw();
	}

	ChPoBo9.SetAlphaFlg(ChStd::False);
}

void CloudList::CloudBurth()
{
	auto TmpData = ChPtr::Make_S<CloudObj>();
	TmpData->Init();
	Cloud_List.push_back(TmpData);
}

void CloudObj::Init()
{
	Scl = (float)(ChStd::GetRand(3) + 1.0f);
	Pos = ChVec3_9(
		(ChStd::GetRand(3) * 2.0f - 2.0f)
		, 0.0f
		, (ChStd::GetRand(3) * 2.0f - 2.0f));
	Pos.Normalize();
	Pos *= MaxLen * (ChStd::GetRand(100) * 0.01f);
	Pos.y = BaseHigh;
}

void CloudObj::Move(const ChVec3_9* _Move)
{
	
	ChVec3_9 TmpVec = *_Move;
	TmpVec.y = 0;
	Pos += TmpVec;
}

void CloudObj::Draw()
{
	ChMat_9 TmpMat;
	ChVec3_9 TmpVec;
	TmpMat = TmpMat * *ChDevice.GetCamVMat();
	TmpMat.Inverse();
	TmpMat = Pos;

	ChPoBo9.SetXYPosition(&ChVec3_9(-10.0f * Scl, 10.0f * Scl, 0.0f), 20.0f * Scl, 20.0f * Scl);

	ChPoBo9.Draw(ChTexMa.GetTexture(TextureName.c_str()), &TmpMat);


}