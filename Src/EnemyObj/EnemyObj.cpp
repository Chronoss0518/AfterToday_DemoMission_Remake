#include"../ChDXLibrary/ChGameIncludeFile.h"
#include"../AllStruct.h"
#include"../HitPosList/HitPosList.h"
#include"../Damage/Damage.h"
#include"../BaseRobot/RobParts.h"
#include"../BaseRobot/BaseRobot.h"
#include"EnemyObj.h"


EnemyObj::EnemyObj(const char _Type, ChStd::Bool _Enemy)
{
	BaseD.EnemyFlg = _Enemy;
	EType = _Type;
}

void EnemyObj::Init(unsigned short _PartsNum)
{

	EInit(EType);

	MoveRot.x = 0.0f;
	MoveRot.y = 0.0f;

	BaseD.Rot.x = 0.0f;
	BaseD.Rot.y = 0.0f;

	Col = ChStd::COLOR1f(1.0f, 1.0f, 0.0f, 0.0f);
	if (!BaseD.EnemyFlg) {
		Col = ChStd::COLOR1f(1.0f, 0.0f, 1.0f, 1.0f);
	}

	BInit(_PartsNum);
	PartsSet();
}

void EnemyObj::Input()
{

	if (HP < 1)return;

	InputCPUData();

}

void EnemyObj::EInit(const char _Type)
{

	//float TmpFps = (float)ChWinAPID.GetFPSCnt();

	switch (_Type)
	{
	case GetEnumCNum(EnemyType, Normal):
		AAS = 50.0f;
		ADS = 15.0f;
		BLen = 5.0f;
		ViweLen = 300.0f;
		SpNow = 98;
		return;
	case GetEnumCNum(EnemyType, Captain):
		AAS = 75.0f;
		ADS = 10.0f;
		BLen = 7.0f;
		ViweLen = 150.0f;
		SpNow = 90;
		return;
	case GetEnumCNum(EnemyType, Boss):
		AAS = 100.0f;
		ADS = 5.0f;
		BLen = 9.0f;
		ViweLen = 100.0f;
		SpNow = 85;
		return;
	default:
		EType = GetEnumCNum(EnemyType, Normal);
		AAS = 50.0f;
		ADS = 20.0f;
		BLen = 5.0f;
		ViweLen = 300.0f;
		SpNow = 98;
		return;
	}
}

EnemyObj2::EnemyObj2( ChStd::Bool _Enemy)
{
	BaseD.EnemyFlg = _Enemy;
}

void EnemyObj2::Init(const unsigned short _PartsNum)
{

	MoveRot.x = 0.0f;
	MoveRot.y = 0.0f;

	BaseD.Rot.x = 0.0f;
	BaseD.Rot.y = 0.0f;

	MoveD.AvoMax = 0.0f;
	BoostV = 0.0f;
	MoveS = 0.0f;
	HP = 30;

	Col = ChStd::COLOR1f(1.0f, 1.0f, 0.0f, 0.0f);
	if (!BaseD.EnemyFlg) {
		Col = ChStd::COLOR1f(1.0f, 0.0f, 1.0f, 1.0f);
	}

	BInit(_PartsNum);
}

void EnemyObj2::Input()
{

	if (HP < 1)return;

	InputCPUData();

}

void EnemyObj2::BInit(unsigned short _PartsNum)
{

	for (char i = 0; i < KeyCount; i++)
	{
		ButtonFlg[i] = ChStd::False;
	}

	TargetPos.Identity();

	{
		auto TmpData = ChPtr::Make_S<ChGravity9>();
		TmpData->Init(
			(float)ChWinAPID.GetFPSCnt());
		TmpData->SetSmpXFile(ChMeMa.GetSmpXFile("Field"));
		TmpData->SetLen(2.0f);
		Physical.push_back(TmpData);
	}

	Look = ChPtr::Make_S<ChCamera9>(ChDevice.GetDevice(), ChWinAPID);
	RobParts = ChPtr::Make_S<RobPartsList>();
	RobParts->Init(shared_from_this());


}
