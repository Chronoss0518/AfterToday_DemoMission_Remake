
#include"../ChDXLibrary/ChGameIncludeFile.h"
#include"../AllStruct.h"
#include"../HitPosList/HitPosList.h"
#include"../Damage/Damage.h"

#include"RobParts.h"
#include"BaseRobot.h"
#include"../PlayerObj/PlayerObj.h"
#include"../EnemyObj/EnemyObj.h"

float TestData;
ChVec3_9 TestPosData;
unsigned char ChTmpCnt;

BRobot::~BRobot()
{
	//TexList = nullptr;
	RobParts = nullptr;
	Look = nullptr;
}

ChStd::Bool BRobot::SetTargetPos()
{

	ChPtr::Shared<BaseRobotsList> SpTmpRobot = WpBRobotsList.lock();
	if (SpTmpRobot == nullptr)return ChStd::False;

	//キャラクター行動//
	ChVec3_9 TmpVec;
	ChVec3_9 TmpTDir;

	float TmpLen = 1000.0f;
	float TmpObjLen;


	TargetPos = ChVec3_9(0.0f, 100.0f, 0.0f);

	ChStd::Bool LookEFlg = ChStd::False;

	for (DWORD i = 0; i < SpTmpRobot->GetRobotsCnt(); i++)
	{
		if (SpTmpRobot->GetEnemyFlg(i) == BaseD.EnemyFlg)continue;
		TmpObjLen = 0.0f;
		TmpTDir = Mat;
		TmpVec = *SpTmpRobot->GetMat(i);
		TmpTDir = TmpVec - TmpTDir;
		TmpTDir.Normalize();
		float TTmpLen = BaseD.Pos.GetLen(&TmpVec);

		if (TTmpLen <= 0.0f)continue;

		if (TmpLen < TTmpLen)continue;

		if (ChObjCon9.SmpXFileHitRay(
			ChMeMa.GetSmpXFile("Field")
			, nullptr
			, &BaseD.Pos
			, &TmpTDir
			, nullptr
			, &TmpObjLen))
		{
			if (TmpObjLen >= 0.0f && TmpObjLen < TTmpLen)continue;
		}

		LookEFlg = ChStd::True;
		TmpLen = TTmpLen;
		TargetPos = *SpTmpRobot->GetMat(i);

	}

	return LookEFlg;
}

void BRobot::BInit(const unsigned short _PartsNum)
{

	for (char i = 0; i < KeyCount; i++)
	{
		ButtonFlg[i] = ChStd::False;
	}

	switch (_PartsNum)
	{
	case 1:
		HP += 20;
		break;
	case 2:
		MoveS += 0.4f;
		BoostV += 0.5f;
		//BoostEnelgy += 200;
		//Enelgy += 5000;
		break;
	default:
		break;
	}
	MoveD.MoveV = 0.0f;
	TargetPos.Identity();

	{

		auto TmpData = ChPtr::Make_S<ChPushBack9>();
		TmpData->SetSmpXFile(ChMeMa.GetSmpXFile("Field"));
		TmpData->SetSmpXFile(ChMeMa.GetSmpXFile("FieldHit"));
		TmpData->SetLen(0.0f);
		TmpData->SetData(1.0f);
		Physical.push_back(TmpData);
	}

	{
		auto TmpData = ChPtr::Make_S<ChGravity9>();
		TmpData->Init(
			(float)ChWinAPID.GetFPSCnt());
		TmpData->SetSmpXFile(ChMeMa.GetSmpXFile("Field"));
		TmpData->SetSmpXFile(ChMeMa.GetSmpXFile("FieldHit"));
		TmpData->SetLen(4.0f);
		//TmpData->SetData(5.0f);
		Physical.push_back(TmpData);
	}

	Look = ChPtr::Make_S<ChCamera9>(ChDevice.GetDevice(), ChWinAPID);
	RobParts = ChPtr::Make_S<RobPartsList>();
	RobParts->Init(shared_from_this(), _PartsNum);

	//MaxEnelgy = Enelgy;
	MaxHp = HP;
}

void BRobot::PartsSet()
{
	RobParts->SetColor(GetEnumCNum(RobPartsList::BodyPartsListName, Foot), &Col);
	RobParts->SetColor(GetEnumCNum(RobPartsList::BodyPartsListName, Body), &Col);
	RobParts->SetColor(GetEnumCNum(RobPartsList::BodyPartsListName, Head), &Col);
	RobParts->SetColor(GetEnumCNum(RobPartsList::BodyPartsListName, Arm), &Col);
	RobParts->SetColor(GetEnumCNum(RobPartsList::BodyPartsListName, Boost), &Col);

}

void BRobot::Draw(const ChStd::Bool _EffectFlg)
{

	if (HP <= 0)return;

	RobParts->Draw();


}

void BRobot::IsDamage(const short _D)
{
	if (HP < 1.0f)return;
	auto Obj = WpBRobotsList.lock();
	if (Obj == nullptr)return;
	if (!Obj->IsDFlg())return;

	HP -= (float)_D;

	if (HP < 0.0f)HP = 0.0f;

}

void BRobot::CreateShotData(const ChStd::DataNo _TmpNo)
{
	ChPtr::Shared<HitPosList> TmpHitPosList = WpHitPosList.lock();
	ChVec3_9 TmpVec;
	/*
		for (auto obj : RobPartsList)
		{
			auto TmpAtkPos = obj->GetWeapDate();
			if (TmpAtkPos == nullptr)continue;
			ChPtr::Shared<ChGame::APosData> TmpDate = ChPtr::Make_S<ChGame::APosData>();

			TmpAtkPos->WeakCnt = 0;
			TmpVec = *obj->GetBasePos();
			TmpVec.z += *TmpAtkPos->ShotPos;
			TmpDate->Base = *TmpAtkPos;
			TmpDate->Pos.MatPos(&Mat, &TmpVec);
			TmpDate->Dir.MatNormal(&Mat, &ChVec3(0.0f, 0.0f, 1.0f));
			TmpHitPosList->SetAData(TmpDate);

		}
	*/
}

void BRobot::InputKeyData()
{

	auto SpTmpRobot = WpBRobotsList.lock();
	if (SpTmpRobot == nullptr)return;

	auto TmpHit = WpHitPosList.lock();
	if (TmpHit == nullptr)return;


	if (!TmpHit->IsHaveBullet())
	{
		if (IsEnemy()) {
			if (!SpTmpRobot->IsMemberLive())return;
		}
		else {
			if (!SpTmpRobot->IsEnemyLive())return;
		}
	}

	SetTargetPos();

	//キャラクター向き//
	Mouse->GetMovePos(&MoveRot);

	if (ChWinAPID.IsPushKey(VK_SHIFT))
	{
		if (ChWinAPID.IsPushKey(VK_SPACE))ButtonFlg[GetEnumCNum(KeyName, JAvo)] = ChStd::True;
		if (ChWinAPID.IsPushKey('W'))ButtonFlg[GetEnumCNum(KeyName, UAvo)] = ChStd::True;
		if (ChWinAPID.IsPushKey('A'))ButtonFlg[GetEnumCNum(KeyName, LAvo)] = ChStd::True;
		if (ChWinAPID.IsPushKey('D'))ButtonFlg[GetEnumCNum(KeyName, RAvo)] = ChStd::True;
		if (ChWinAPID.IsPushKeyNoHold('S'))ButtonFlg[GetEnumCNum(KeyName, OverBoost)] = ChStd::True;

	}

	if (ChWinAPID.IsPushKey('W'))ButtonFlg[GetEnumCNum(KeyName, Up)] = ChStd::True;
	if (ChWinAPID.IsPushKey('S'))ButtonFlg[GetEnumCNum(KeyName, Down)] = ChStd::True;
	if (ChWinAPID.IsPushKey('A'))ButtonFlg[GetEnumCNum(KeyName, Left)] = ChStd::True;
	if (ChWinAPID.IsPushKey('D'))ButtonFlg[GetEnumCNum(KeyName, Right)] = ChStd::True;
	if (ChWinAPID.IsPushKey(VK_SPACE))ButtonFlg[GetEnumCNum(KeyName, Jump)] = ChStd::True;


	if (IsEnemy()) {
		if (!SpTmpRobot->IsMemberLive())return;
	}
	else {
		if (!SpTmpRobot->IsEnemyLive())return;
	}

	if (ChWinAPID.IsPushKey(VK_RBUTTON))ButtonFlg[GetEnumCNum(KeyName, MAttack)] = ChStd::True;
	if (ChWinAPID.IsPushKey(VK_LBUTTON))ButtonFlg[GetEnumCNum(KeyName, SAttack)] = ChStd::True;

}

void BRobot::InputCPUData()
{

	auto SpTmpRobot = WpBRobotsList.lock();
	if (SpTmpRobot == nullptr)return;

	auto TmpHit = WpHitPosList.lock();
	if (TmpHit == nullptr)return;


	if (!TmpHit->IsHaveBullet())
	{
		if (IsEnemy()) {
			if (!SpTmpRobot->IsMemberLive())return;
		}
		else {
			if (!SpTmpRobot->IsEnemyLive())return;
		}
	}

	LookFlg = SetTargetPos();
	if (!LookFlg)
	{

		MoveKey[0] = 'W';
		ButtonFlg[GetEnumCNum(KeyName, Up)] = ChStd::True;

		//加算計算//
		AIMoveMouInput();

		return;

	}

	//移動計算//
	AIMoveKeyInput();

	//加算計算//
	AIMoveMouInput();

}

void BRobot::AIMoveKeyInput()
{
	ChVec3_9 TmpVec;

	TmpVec = TargetPos;

	float TmpLen = BaseD.Pos.GetLen(&TmpVec);
	ChStd::Bool TmpFlg = ChStd::False;

	if (ChStd::GetRand(100) > SpNow)
	{
		if (MoveKey[0] == 'A')ButtonFlg[GetEnumCNum(KeyName, LAvo)] = ChStd::True;
		else if (MoveKey[0] == 'D')ButtonFlg[GetEnumCNum(KeyName, RAvo)] = ChStd::True;
		else if (MoveKey[0] == 'W')ButtonFlg[GetEnumCNum(KeyName, UAvo)] = ChStd::True;
		else ButtonFlg[GetEnumCNum(KeyName, JAvo)] = ChStd::True;

		TmpFlg = ChStd::True;
	}

	if (!TmpFlg)
	{
		if (TmpLen > 200.0f) {
			if (!BoostFlg)ButtonFlg[GetEnumCNum(KeyName, MAttack)] = ChStd::True;

			MoveKey[0] = 'W';
			ButtonFlg[GetEnumCNum(KeyName, Up)] = ChStd::True;

			switch (MoveKey[1])
			{
			case 'A':
				ButtonFlg[GetEnumCNum(KeyName, Left)] = ChStd::True;
				break;
			case 'J':
				ButtonFlg[GetEnumCNum(KeyName, Jump)] = ChStd::True;
				break;
			case 'S':
				//ButtonFlg[GetEnumCNum(KeyName, Down)] = ChStd::True;
				MoveKey[1] = KeyCode[ChStd::GetRand(5)];
				break;
			case 'D':
				ButtonFlg[GetEnumCNum(KeyName, Right)] = ChStd::True;
				break;
			default:
				MoveKey[1] = KeyCode[ChStd::GetRand(5)];
				break;
			}
		}
		else if (TmpLen > 100.0f) {
			if (!BoostFlg)ButtonFlg[GetEnumCNum(KeyName, MAttack)] = ChStd::True;

			MoveKey[0] = 'W';
			ButtonFlg[GetEnumCNum(KeyName, Up)] = ChStd::True;

			switch (MoveKey[1])
			{
			case 'A':
				ButtonFlg[GetEnumCNum(KeyName, Left)] = ChStd::True;
				break;
			case 'J':
				ButtonFlg[GetEnumCNum(KeyName, Jump)] = ChStd::True;
				break;
			case 'S':
				//ButtonFlg[GetEnumCNum(KeyName, Down)] = ChStd::True;
				MoveKey[1] = KeyCode[ChStd::GetRand(5)];
				break;
			case 'D':
				ButtonFlg[GetEnumCNum(KeyName, Right)] = ChStd::True;
				break;
			default:
				MoveKey[1] = KeyCode[ChStd::GetRand(5)];
				break;
			}
		}
		else
		{
			for (unsigned char i = 0; i < MoveKeyCnt; i++)
			{
				switch (MoveKey[i])
				{
				case 'A':
					ButtonFlg[GetEnumCNum(KeyName, Left)] = ChStd::True;
					break;
				case 'J':
					ButtonFlg[GetEnumCNum(KeyName, Jump)] = ChStd::True;
					break;
				case 'S':
					ButtonFlg[GetEnumCNum(KeyName, Down)] = ChStd::True;
					break;
				case 'D':
					ButtonFlg[GetEnumCNum(KeyName, Right)] = ChStd::True;
					break;
				default:
					Change();
					break;
				}

			}
		}
	}

	if (ChStd::GetRand(100) > SpNow)Change();

}

void BRobot::AIMoveMouInput()
{
	auto SpTmpRobot = WpBRobotsList.lock();
	if (SpTmpRobot == nullptr)return;


	ChVector3_9 TmpVec, TmpPos;
	ChVec3_9 TmpMove;
	ChMatrix_9 TmpMat;
	float TmpLen;

	TmpVec = Mat;
	Look->SetCamPos(&TmpVec);

	TmpVec.MatPos(&Mat, &ChVec3_9(0.0f, 0.0f, 1.0f));


	Look->SetCamLook(&TmpVec);
	Look->SetCamMat(&Mat);
	Look->SetView();

	TmpMat = TargetPos;

	ChDevice.GetOnViewPos(&TmpVec, &TmpMat);

	TmpPos = ChVec3_9(
		ChWinAPID.GetWindWidth() / 2.0f
		, ChWinAPID.GetWindHeight() / 2.0f
		, 0.0f);

	TmpPos.z = TmpVec.z;

	TmpMove = TmpPos - TmpVec;

	TmpMove *= (0.02f * ADS);
	CurrsolMove(std::fabsf(TmpMove.x),std::fabsf(TmpMove.y));

	TWindOnPosLen = TmpPos.GetLen(&TmpVec);

	if (TmpPos.z <= 0.0f)return;

	TmpLen = TmpVec.GetLen(&TmpPos);

	if (TmpLen > ViweLen)return;



	if (IsEnemy()) {
		if (!SpTmpRobot->IsMemberLive())return;
	}
	else {
		if (!SpTmpRobot->IsEnemyLive())return;
	}
	if (LookFlg)
	{

		ButtonFlg[GetEnumCNum(KeyName, MAttack)] = ChStd::True;

		ButtonFlg[GetEnumCNum(KeyName, SAttack)] = ChStd::True;
	}


}

void BRobot::CurrsolMove(const float _MoveX, const float _MoveY)
{
	if (!IsEnemy())TestData = _MoveX;

	ChStd::FPOINT TmpDir;

	ChVector3_9 TmpVec, UpVec;
	ChVector3_9 TmpCross, TmpVDir;
	ChMat_9 TmpMat;

	TmpVec = Mat;
	TmpVDir = TargetPos;
	TmpVec = TmpVDir - TmpVec;

	TmpVDir.MatNormal(&Mat, &ChVec3_9(0.0f, 0.0f, 1.0f));

	TmpVec.Normalize();

	TmpCross.CrossVec(&TmpVDir, &TmpVec);

	TmpDir.x = TmpCross.y > 0 ? _MoveX : -_MoveX;
	TmpDir.y = TmpCross.x > 0 ? _MoveY : -_MoveY;

	if (TmpVec.z < 0)TmpDir.y = -TmpDir.y;


	MoveRot.x = TmpDir.x;
	MoveRot.y = TmpDir.y;
}

void BRobot::Change()
{
	for (unsigned char i = 0; i < MoveKeyCnt; i++)
	{
		switch (ChStd::GetRand(5))
		{
		case 0:
			MoveKey[i] = 'A';
			ButtonFlg[GetEnumCNum(KeyName, Left)] = ChStd::True;
			break;
		case 1:
			MoveKey[i] = 'S';
			ButtonFlg[GetEnumCNum(KeyName, Down)] = ChStd::True;
			break;
		case 2:
			MoveKey[i] = 'D';
			ButtonFlg[GetEnumCNum(KeyName, Right)] = ChStd::True;
			break;
		case 3:
			MoveKey[i] = 'J';
			ButtonFlg[GetEnumCNum(KeyName, Jump)] = ChStd::True;
			break;
		default:
			if (i != 0)return;
			MoveKey[0] = 'T';
			return;
		}
	}
}

void BRobot::InputToMove()
{

	ChMat_9 TmpMat;
	TmpMat.RotYPR(0.0f, BaseD.Rot.y, 0.0f);
	CamMoveVec = ChVec3_9(0.0f, 0.0f, 0.0f);
	//キャラクター向き//

	if (abs(MoveRot.x) > 1.0f)BaseD.Rot.y += (MoveRot.x / MouMoveP) * Sens;
	if (abs(MoveRot.y) > 1.0f)BaseD.Rot.x += (MoveRot.y / MouMoveP) * Sens;

	if (abs(BaseD.Rot.x) > 90.0f)BaseD.Rot.x > 0.0f ? BaseD.Rot.x = 90.0f : BaseD.Rot.x = -90.0f;

	//キャラクター行動//
	ChVec3_9 TmpVec;
	ChVec3_9 UpVec;

	/*
		if (ButtonFlg[GetEnumCNum(KeyName, OverBoost)])
		{
			BoostFlg = !BoostFlg;
		}

	*/
	if (ButtonFlg[GetEnumCNum(KeyName,JAvo)] && MoveD.AvoV <= 0.0f)
	{
		CamMoveVec += ChVec3_9(0.0f, 1.0f, 0.0f);
		MoveD.AvoDir.MatNormal(&TmpMat, &ChVec3_9(0.0f, 1.0f, 0.0f));
		MoveD.AvoV = MoveD.AvoMax;
	}
	if (ButtonFlg[GetEnumCNum(KeyName, LAvo)] && MoveD.AvoV <= 0.0f)
	{
		CamMoveVec += ChVec3_9(-1.0f, 0.0f, 0.0f);
		MoveD.AvoDir.MatNormal(&TmpMat, &ChVec3_9(-1.0f, 0.0f, 0.0f));
		MoveD.AvoV = MoveD.AvoMax;
	}
	if (ButtonFlg[GetEnumCNum(KeyName, RAvo)] && MoveD.AvoV <= 0.0f)
	{
		CamMoveVec += ChVec3_9(1.0f, 0.0f, 0.0f);
		MoveD.AvoDir.MatNormal(&TmpMat, &ChVec3_9(1.0f, 0.0f, 0.0f));
		MoveD.AvoV = MoveD.AvoMax;
	}
	if (ButtonFlg[GetEnumCNum(KeyName, UAvo)] && MoveD.AvoV <= 0.0f)
	{
		CamMoveVec += ChVec3_9(0.0f, 0.0f, 1.0f);
		MoveD.AvoDir.MatNormal(&TmpMat, &ChVec3_9(0.0f, 0.0f, 1.0f));
		MoveD.AvoV = MoveD.AvoMax;
	}


	if (ButtonFlg[GetEnumCNum(KeyName, Up)])
	{
		CamMoveVec += ChVec3_9(0.0f, 0.0f, 1.0f);
		TmpVec.Move(0.0f, 0.0f, 1.0f);
	}
	if (ButtonFlg[GetEnumCNum(KeyName, Down)])
	{
		CamMoveVec += ChVec3_9(0.0f, 0.0f, -1.0f);
		TmpVec.Move(0.0f, 0.0f, -1.0f);
	}
	if (ButtonFlg[GetEnumCNum(KeyName, Left)])
	{
		CamMoveVec += ChVec3_9(-1.0f, 0.0f, 0.0f);
		TmpVec.Move(-1.0f, 0.0f, 0.0f);
	}
	if (ButtonFlg[GetEnumCNum(KeyName, Right)])
	{
		CamMoveVec += ChVec3_9(1.0f, 0.0f, 0.0f);
		TmpVec.Move(1.0f, 0.0f, 0.0f);
	}
	if (ButtonFlg[GetEnumCNum(KeyName, Jump)])
	{
		CamMoveVec += ChVec3_9(0.0f, 1.0f, 0.0f);
		TmpVec.Move(0.0f, 1.0f, 0.0f);
	}



	if (fabsf(TmpVec.GetLen(&D3DXVECTOR3(0.0f, 0.0f, 0.0f))) > 1.0f)TmpVec.Normalize();
	TmpVec.MatNormal(&TmpMat, &TmpVec);
	CamMoveVec.Normalize();


	MoveD.MoveV += TmpVec * MoveS;

	if (BoostFlg)MoveD.MoveV += TmpVec * BoostV;

	MoveD.MoveV += MoveD.AvoDir * MoveD.AvoV;
	MoveD.AvoV *= MoveD.AvoDen;

	if (fabsf(MoveD.AvoV) < MoveD.AvoMin)MoveD.AvoV = 0.0f;


	if (ButtonFlg[GetEnumCNum(KeyName, MAttack)])
	{
		Attack(RobParts->GetMWeap());
	}

	if (ButtonFlg[GetEnumCNum(KeyName, SAttack)])
	{
		Attack(RobParts->GetSWeap());
	}

	for (unsigned char i = 0; i < KeyCount; i++)
	{
		ButtonFlg[i] = ChStd::False;
	}

	MoveRot.x = 0.0f;
	MoveRot.y = 0.0f;


}

void BRobot::CamUpdate(void)
{
	auto Camera = WpLook.lock();

	if (Camera == nullptr)return;

	ChVector3_9 TmpCamP, TmpVec;

	TmpCamP.MatPos(&Mat, &BaseCamPos);

	Camera->SetCamPos(&TmpCamP);

	TmpCamP.MatPos(&Mat, &BaseCamLook);

	Camera->SetCamLook(&TmpCamP);

	Camera->SetCamMat(&Mat);

	Camera->SetLookMaxLen(4000.0f);
}

void BRobot::Move()
{
	ChVec3_9 TmpVec;

	auto RobotsList = WpBRobotsList.lock();
	if (RobotsList == nullptr)return;
	InputToMove();

	ChPtr::Shared<HitPosList> TmpHPos = WpHitPosList.lock();


	AvoHitRobots();
	{

		ChStd::Bool PhyFlg = ChStd::False;
		for (auto Phy : Physical)
		{
			if (Phy->UpDate(&BaseD.Pos, &MoveD.MoveV))PhyFlg = ChStd::True;
		}

		if (PhyFlg)
		{
			BaseD.Pos += MoveD.MoveV;
		}
	}

	RobParts->Updata();

	DrawMat.RotYPR(0.0f, BaseD.Rot.y, 0.0f);

	Mat.RotYPR(BaseD.Rot.x, BaseD.Rot.y, 0.0f);
	{
		ChVec3_9 TmpPos = BaseD.Pos;
		TmpPos.y += 5.0f;
		float TmpLen;
		if (ChObjCon9.SmpXFileHitRay(
			ChMeMa.GetSmpXFile("Field")
			, nullptr
			, &BaseD.Pos
			, &ChVec3_9(0.0f, -1.0f, 0.0f), nullptr, &TmpLen))
		{
			if (TmpLen < 5.0f
				&& MoveD.MoveV.GetLen(&ChVec3_9()) < 0.1f
				&& !RobotsList->IsEnemyLive())Mat.RotYPR(0.0f, BaseD.Rot.y, 0.0f);
		}
	}

	MoveD.MoveV = 0.0f;
	float TmpLen = BaseD.Pos.GetLen();

	if (!IsEnemy())TestPosData = BaseD.Pos;
	//if (!IsEnemy())TestData = TmpLen;

	Mat = BaseD.Pos;
	DrawMat = BaseD.Pos;

	SetCamMat();

	CamUpdate();
}

void BRobot::SetCamMat()
{

	for (unsigned char i = 0; i < CamPosMatCnt - 1; i++)
	{
		CamOffMat[i + 1] = CamOffMat[i];
	}
	CamOffMat[0] = Mat;

	CamPos = CamOffMat[CamPosMatCnt - 1];

}

void BRobot::AvoHitRobots()
{
	ChPtr::Shared<BaseRobotsList> SpTmpRobot = WpBRobotsList.lock();
	if (SpTmpRobot == nullptr)return;
	ChVec3_9 TmpVec;
	float TmpLen;

	for (DWORD i = 0; i < SpTmpRobot->GetRobotsCnt(); i++)
	{
		TmpVec = *SpTmpRobot->GetMat(i);
		if (TmpVec == BaseD.Pos)continue;
		TmpLen = BaseD.Pos.GetLen(&TmpVec);
		if (TmpLen > Len)continue;
		TmpVec = TmpVec - BaseD.Pos;
		TmpVec.Normalize();
		BaseD.Pos -= TmpVec * TmpLen;
	}
}

void BRobot::Attack(const ChPtr::Shared<ChGame::WeapData> _Weap)
{
	auto HitPos = WpHitPosList.lock();

	if (_Weap == nullptr)return;
	if (HitPos == nullptr)return;

	ChVec3_9 TmpVec;
	ChMat_9 TmpMat;

	TmpMat = Mat;

	TmpMat = BaseD.Pos + MoveD.MoveV;

	TmpVec = _Weap->ShotPos;
	auto TmpAData = ChPtr::Make_S<ChGame::APosData>();
	TmpAData->Base = *_Weap;
	TmpAData->Base.Damage += AddDamage;
	TmpVec.MatPos(&TmpMat, &TmpVec);
	TmpAData->Pos = TmpVec;
	ChDSound9.PlaySE(TmpAData->Base.SEName, "Sound", &TmpVec);
	TmpVec.MatNormal(&Mat, &ChVec3_9(0.0f, 0.0f, TmpAData->Base.Speed));

	TmpAData->Base.Speed += MoveD.MoveV.GetLen(&ChVec3_9(0.0f, 0.0f, 0.0f));


	//TmpVec = MoveD.MoveV + TmpVec;
	TmpVec.Normalize();

	TmpAData->Dir = TmpVec;

	HitPos->SetAData(TmpAData);

}

void BaseRobotsList::Init()
{
	Camera = ChPtr::Make_S<ChCamera9>(ChDevice.GetDevice(), ChWinAPID);
	DBase = ChPtr::Make_S<DDataBase>();
	DBase->Init();
	EndCamRobot = ChPtr::Make_S<BRobot>();

	ChTexMa.SetTexture("Ts.png", "TargetMa", "Base", 64, 64, 64, 64);
	ChTexMa.SetTexture("ATKCurrsol.png", "ATC", "Base", 64, 64, 64, 64);
	ChTexMa.SetTexture("Window.png", "Wind", "Base", 128, 128, 128, 128);
	ChTexMa.SetTexture("Right.png", "Right", "Base", 128, 128, 128, 128);
	ChTexMa.SetTexture("Left.png", "Left", "Base", 128, 128, 128, 128);

}

void BaseRobotsList::Draw()
{
	for (auto obj : RobotsList)
	{
		obj->Draw(ChStd::False);
	}

	ChDevice.LightSetting(ChStd::False);

	DBase->Draw();

	ChDevice.LightSetting(ChStd::True);

}

void BaseRobotsList::Update()
{
	for (auto obj : RobotsList)
	{
		obj->Input();
	}

	auto HitPos = WpHitPosList.lock();

	for (unsigned short i = 0; i < RobotsList.size(); i++)
	{
		if (!RobotsList[i]->IsDorpOutFlg())
		{
			*EndCamRobot = *RobotsList[i];

			RobotsList.erase(RobotsList.begin() + i);
			i--;

			if (EndCamRobot->IsEnemy())
			{
				EnemyCnt--;
			}
			else
			{
				MemberCnt--;
				MoveCamRobot();
			}
			continue;
		}

		RobotsList[i]->Move();
		if (HitPos != nullptr && RobotsList[i]->IsLiveFlg())HitPos->IsHitTest(RobotsList[i]->GetRobParts());

	}


	DBase->Update();
	ChTmpCnt = EnemyCnt;

}

void BaseRobotsList::SetRobots(std::string _PlayerData, const ChVec3_9 *_StartPos, unsigned short _PartsNum)
{
	RobotsList.push_back(ChPtr::Make_S<PlayerObj>());
	RobotsList[RobotsList.size() - 1]->Init(0);
	RobotsList[RobotsList.size() - 1]->SetPos(_StartPos);
	RobotsList[RobotsList.size() - 1]->SetHitPosList(WpHitPosList.lock());
	RobotsList[RobotsList.size() - 1]->SetRobotsList(shared_from_this());

	auto TmpDData = ChPtr::Make_S<Damage>();
	RobotsList[RobotsList.size() - 1]->SharedDamageData(TmpDData);
	DBase->SharedDamageData(TmpDData);

	MemberCnt++;
}

void BaseRobotsList::SetRobots(const ChStd::Bool _EneFlg, ChVec3_9 *_StartPos, unsigned short _PartsNum)
{
	if (!_EneFlg)RobotsList.push_back(ChPtr::Make_S<EnemyObj>(GetEnumCNum(EnemyType, Boss), _EneFlg));
	else RobotsList.push_back(ChPtr::Make_S<EnemyObj>(GetEnumCNum(EnemyType, Normal), _EneFlg));
	RobotsList[RobotsList.size() - 1]->Init(_PartsNum);
	RobotsList[RobotsList.size() - 1]->SetPos(_StartPos);
	RobotsList[RobotsList.size() - 1]->SetHitPosList(WpHitPosList.lock());
	RobotsList[RobotsList.size() - 1]->SetRobotsList(shared_from_this());

	auto TmpDData = ChPtr::Make_S<Damage>();
	RobotsList[RobotsList.size() - 1]->SharedDamageData(TmpDData);
	DBase->SharedDamageData(TmpDData);

	if (_EneFlg)EnemyCnt++;
	else MemberCnt++;

}

void BaseRobotsList::SetRobots2(const ChStd::Bool _EneFlg, ChVec3_9 *_StartPos)
{

	if (!_EneFlg)RobotsList.push_back(std::make_shared<EnemyObj2>(_EneFlg));
	else RobotsList.push_back(std::make_shared<EnemyObj2>(_EneFlg));
	RobotsList[RobotsList.size() - 1]->Init(0);
	RobotsList[RobotsList.size() - 1]->SetPos(_StartPos);
	RobotsList[RobotsList.size() - 1]->SetHitPosList(WpHitPosList.lock());
	RobotsList[RobotsList.size() - 1]->SetRobotsList(shared_from_this());

	auto TmpDData = ChPtr::Make_S<Damage>();
	RobotsList[RobotsList.size() - 1]->SharedDamageData(TmpDData);
	DBase->SharedDamageData(TmpDData);

	if (_EneFlg)EnemyCnt++;
	else MemberCnt++;
}

const ChMat_9* BaseRobotsList::GetCamPos()
{
	TmpMat.Identity();
	ChPtr::Shared<BRobot>TmpRobot = WpCamRobot.lock();
	if (TmpRobot == nullptr)return &TmpMat;
	TmpMat = *TmpRobot->GetCamPos();
	return &TmpMat;
}

const ChMat_9* BaseRobotsList::GetMat(const DWORD _No)
{
	TmpMat.Identity();
	if (RobotsList.size() <= _No)return &TmpMat;
	TmpMat = *RobotsList[_No]->GetMat();
	return &TmpMat;
}

const ChMat_9* BaseRobotsList::GetCamLookPos()
{

	TmpMat.Identity();
	ChPtr::Shared<BRobot>TmpRobot = WpCamRobot.lock();
	if (TmpRobot == nullptr)return &TmpMat;
	TmpMat = *TmpRobot->GetMat();
	return &TmpMat;
}

void BaseRobotsList::MoveCamRobot()
{
	if (WpCamRobot.lock() == nullptr)
	{
		if (MemberCnt <= 0)
		{

			WpCamRobot = EndCamRobot;
			return;
		}

		for (auto Obj : RobotsList)
		{
			if (!Obj->IsEnemy())
			{
				WpCamRobot = Obj;
				return;
			}
		}
		WpCamRobot = EndCamRobot;

	}
}

void BaseRobotsList::DrawMarker()
{
	auto Robot = WpCamRobot.lock();
	if (Robot == nullptr)return;

	ChDSound9.SetListenerPos(Robot->GetPos());
	ChVec3_9 TmpVec;

	ChVec3_9 TmpPos;

	ChMat_9 TmpMat, TmpRMat;
	float TmpLen;


	POINT Cnt;
	Cnt.x = 0;
	Cnt.y = 0;

	for (auto Obj : RobotsList)
	{
		if (!Obj->IsEnemy())continue;

		TmpMat = *Obj->GetMat();

		ChDevice.GetOnViewPos(&TmpVec, &TmpMat);

		TmpPos = ChVec3_9(
			ChWinAPID.GetWindWidth() / 2.0f
			, ChWinAPID.GetWindHeight() / 2.0f
			, 0.0f);

		TmpPos.z = TmpVec.z;

		float TmpLen = TmpVec.GetLen(&TmpPos);

		ChVec3_9 TmpVec1, TmpVec2;
		TmpMat = *Robot->GetMat();
		TmpVec1 = TmpMat;
		TmpVec2 = *Obj->GetMat();
		TmpMat.Inverse();
		TmpVec2.MatPos(&TmpMat, &TmpVec2);

		if (TmpVec2.z > 0.0f)
		{
			if (0.0f <= TmpVec.x && TmpVec.x <= (float)ChWinAPID.GetWindWidth())
			{

				if (0.0f <= TmpVec.y && TmpVec.y <= (float)ChWinAPID.GetWindHeight())
				{


					if (TmpLen > 300.0f)continue;

					TmpMat.Identity();
					TmpMat.ScalingMode(100.0f / 64.0f);

					//TmpVec.z = 0.0f;
					TmpMat = TmpVec;

					if (TmpPos.z <= 0.0f)continue;

					ChSp9.DrawSprite(
						ChTexMa.GetTexture("TargetMa")
						, &TmpMat
						, &ChVec3_9(0.0f, 0.0f, 0.0f)
						, nullptr);

					continue;
				}
			}

		}





		TmpPos = ChVec3_9(
			ChWinAPID.GetWindWidth() / 2.0f
			, ChWinAPID.GetWindHeight() * 1.0f / 3.0f
			, 0.0f);

		TmpMat.Identity();



		if (TmpVec2.x > 0.0f) {
			TmpPos.x = ChWinAPID.GetWindWidth() - 64.0f;
			TmpPos.y += 128.0f * Cnt.x;
			Cnt.x++;
			TmpMat = TmpPos;

			ChSp9.DrawSprite(
				ChTexMa.GetTexture("Right")
				, &TmpMat
				, &ChVec3_9(0.0f, 0.0f, 0.0f)
				, nullptr);
			continue;
		}

		if (TmpVec2.x < 0.0f)
		{

			TmpPos.x = 64.0f;
			TmpPos.y += 128.0f * Cnt.y;
			Cnt.y++;
			TmpMat = TmpPos;

			ChSp9.DrawSprite(
				ChTexMa.GetTexture("Left")
				, &TmpMat
				, &ChVec3_9(0.0f, 0.0f, 0.0f)
				, nullptr);
			continue;
		}


	}

	TmpMat.Identity();
	TmpVec = *Robot->GetMoveVec();

	TmpLen = TmpVec.GetLen();
	TmpVec.Normalize();


	TmpMat.ScalingMode(32.0f / 64.0f);

	TmpPos = ChVec3_9(
		ChWinAPID.GetWindWidth() / 2.0f
		, ChWinAPID.GetWindHeight() / 2.0f - 30.0f
		, 0.0f);
	TmpPos -= TmpVec * TmpLen * 70.0f;

	TmpPos.y = (ChWinAPID.GetWindHeight() / 2.0f - 50.0f)
		- ((TmpPos.y / 25.0f) * (TmpLen - TmpVec.z));

	TmpPos.z = 0.0f;

	TmpMat = TmpPos;


	ChSp9.DrawSprite(
		ChTexMa.GetTexture("ATC")
		, &TmpMat
		, &ChVec3_9(0.0f, 0.0f, 0.0f)
		, nullptr);


	TmpPos = ChVec3_9(
		ChWinAPID.GetWindWidth() / 2.0f
		, ChWinAPID.GetWindHeight() / 2.0f
		, 0.0f);

	TmpMat.ScalingMode(600.0f / 128.0f);

	TmpMat = TmpPos;

	{

		float TmpR, TmpG, TmpB;
		TmpG = Robot->GetPraHp() < 50 ? 255.0f * (2.0f * Robot->GetPraHp())
			: 255.0f - 255.0f * (2.0f * Robot->GetPraHp());
		TmpB = Robot->GetPraHp() * 255.0f;
		TmpR = 255 - Robot->GetPraHp() * 255.0f;
		ChStd::COLOR255 TmpCol;
		TmpCol.a = 255;
		TmpCol.r = (unsigned char)TmpR;
		TmpCol.g = 0;
		TmpCol.b = (unsigned char)TmpB;
		ChTexMa.SetBlendColor(&TmpCol, "Wind");
	}

	ChSp9.DrawSprite(
		ChTexMa.GetTexture("Wind")
		, &TmpMat
		, &ChVec3_9(0.0f, 0.0f, 0.0f)
		, nullptr);

	//char TmpFont[200];
	/*

			ChDFont9.SetFontSize(16, 8);
			sprintf(TmpFont, "%.2f", TestData);

			ChDFont9.Draw(TmpFont, 0, 0);
	*/
	/*
			sprintf(TmpFont, "%.2f\n%.2f\n%.2f", TestPosData.x, TestPosData.y,TestPosData.z);

			ChDFont9.Draw(TmpFont, 0, 90);
	*/
	/*
			sprintf(TmpFont, "%d", ChTmpCnt);

			ChDFont9.Draw(TmpFont, 0, 90 * 4);
		*/
}
