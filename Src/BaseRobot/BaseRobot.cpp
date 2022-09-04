
#include"../BaseIncluder.h"

#include"../AllStruct.h"
#include"../HitPosList/HitPosList.h"
#include"../Damage/Damage.h"

#include"RobParts.h"
#include"BaseRobot.h"
#include"../PlayerObj/PlayerObj.h"
#include"../EnemyObj/EnemyObj.h"

float TestData;
ChVec3 TestPosData;
unsigned char ChTmpCnt;

BRobot::~BRobot()
{
	//TexList = nullptr;
	RobParts = nullptr;
}

ChStd::Bool BRobot::SetTargetPos()
{

	ChPtr::Shared<BaseRobotsList> SpTmpRobot = WpBRobotsList.lock();
	if (SpTmpRobot == nullptr)return false;

	//キャラクター行動//
	ChVec3 tmpVec;
	ChVec3 tmpTDir;

	float tmpLen = 1000.0f;
	float tmpObjLen;


	TargetPos.SetPosition(0.0f, 100.0f, 0.0f);

	ChStd::Bool LookEFlg = false;

	for (DWORD i = 0; i < SpTmpRobot->GetRobotsCnt(); i++)
	{
		if (SpTmpRobot->GetEnemyFlg(i) == BaseD.EnemyFlg)continue;
		tmpObjLen = 0.0f;
		tmpTDir = Mat.GetPosition();
		tmpVec = SpTmpRobot->GetMat(i);
		tmpTDir = tmpVec - tmpTDir;
		tmpTDir.Normalize();
		float TTmpLen = BaseD.Pos.GetLen(tmpVec);

		if (TTmpLen <= 0.0f)continue;

		if (tmpLen < TTmpLen)continue;



		//if (ChObjCon9.SmpXFileHitRay(
		//	ChMeMa.GetSmpXFile("Field")
		//	, nullptr
		//	, &BaseD.Pos
		//	, &tmpTDir
		//	, nullptr
		//	, &tmpObjLen))
		//{
		//	if (tmpObjLen >= 0.0f && tmpObjLen < TTmpLen)continue;
		//}

		LookEFlg = true;
		tmpLen = TTmpLen;
		TargetPos = *SpTmpRobot->GetMat(i);

	}

	return LookEFlg;
}

void BRobot::BInit(const unsigned short _PartsNum)
{

	for (char i = 0; i < KeyCount; i++)
	{
		ButtonFlg[i] = false;
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


	RobParts = ChPtr::Make_S<RobPartsList>();
	RobParts->Init(shared_from_this(), _PartsNum);

	//MaxEnelgy = Enelgy;
	MaxHp = HP;
}

void BRobot::PartsSet()
{

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

void BRobot::CreateShotData(const ChStd::DataNo _tmpNo)
{
	ChPtr::Shared<HitPosList> tmpHitPosList = WpHitPosList.lock();
	ChVec3 tmpVec;
	/*
		for (auto obj : RobPartsList)
		{
			auto tmpAtkPos = obj->GetWeapDate();
			if (tmpAtkPos == nullptr)continue;
			ChPtr::Shared<ChGame::APosData> tmpDate = ChPtr::Make_S<ChGame::APosData>();

			tmpAtkPos->WeakCnt = 0;
			tmpVec = *obj->GetBasePos();
			tmpVec.z += *tmpAtkPos->ShotPos;
			tmpDate->Base = *tmpAtkPos;
			tmpDate->Pos.MatPos(&Mat, &tmpVec);
			tmpDate->Dir.MatNormal(&Mat, &ChVec3(0.0f, 0.0f, 1.0f));
			tmpHitPosList->SetAData(tmpDate);

		}
	*/
}

void BRobot::InputKeyData()
{

	auto SpTmpRobot = WpBRobotsList.lock();
	if (SpTmpRobot == nullptr)return;

	auto tmpHit = WpHitPosList.lock();
	if (tmpHit == nullptr)return;


	if (!tmpHit->IsHaveBullet())
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
	mouse->GetMovePos(&MoveRot);

	if (ChWinAPID.IsPushKey(VK_SHIFT))
	{
		if (ChWinAPID.IsPushKey(VK_SPACE))ButtonFlg[ChStd::EnumCast(KeyName::JAvo)] = true;
		if (ChWinAPID.IsPushKey('W'))ButtonFlg[ChStd::EnumCast(KeyName::UAvo)] = true;
		if (ChWinAPID.IsPushKey('A'))ButtonFlg[ChStd::EnumCast(KeyName::LAvo)] = true;
		if (ChWinAPID.IsPushKey('D'))ButtonFlg[ChStd::EnumCast(KeyName::RAvo)] = true;
		if (ChWinAPID.IsPushKeyNoHold('S'))ButtonFlg[ChStd::EnumCast(KeyName::OverBoost)] = true;

	}

	if (ChWinAPID.IsPushKey('W'))ButtonFlg[ChStd::EnumCast(KeyName::Up)] = true;
	if (ChWinAPID.IsPushKey('S'))ButtonFlg[ChStd::EnumCast(KeyName::Down)] = true;
	if (ChWinAPID.IsPushKey('A'))ButtonFlg[ChStd::EnumCast(KeyName::Left)] = true;
	if (ChWinAPID.IsPushKey('D'))ButtonFlg[ChStd::EnumCast(KeyName::Right)] = true;
	if (ChWinAPID.IsPushKey(VK_SPACE))ButtonFlg[ChStd::EnumCast(KeyName::Jump)] = true;


	if (IsEnemy()) {
		if (!SpTmpRobot->IsMemberLive())return;
	}
	else {
		if (!SpTmpRobot->IsEnemyLive())return;
	}

	if (ChWinAPID.IsPushKey(VK_RBUTTON))ButtonFlg[ChStd::EnumCast(KeyName::MAttack)] = true;
	if (ChWinAPID.IsPushKey(VK_LBUTTON))ButtonFlg[ChStd::EnumCast(KeyName::SAttack)] = true;

}

void BRobot::InputCPUData()
{

	auto SpTmpRobot = WpBRobotsList.lock();
	if (SpTmpRobot == nullptr)return;

	auto tmpHit = WpHitPosList.lock();
	if (tmpHit == nullptr)return;


	if (!tmpHit->IsHaveBullet())
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
		ButtonFlg[ChStd::EnumCast(KeyName::Up)] = true;

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
	ChVec3 tmpVec;

	tmpVec = TargetPos;

	float tmpLen = BaseD.Pos.GetLen(&tmpVec);
	ChStd::Bool tmpFlg = ChStd::False;

	if (rand() % (100) > SpNow)
	{
		if (MoveKey[0] == 'A')ButtonFlg[ChStd::EnumCast(KeyName::LAvo)] = true;
		else if (MoveKey[0] == 'D')ButtonFlg[ChStd::EnumCast(KeyName::RAvo)] = true;
		else if (MoveKey[0] == 'W')ButtonFlg[ChStd::EnumCast(KeyName::UAvo)] = true;
		else ButtonFlg[ChStd::EnumCast(KeyName::JAvo)] = true;

		tmpFlg = true;
	}

	if (!tmpFlg)
	{
		if (tmpLen > 200.0f) {
			if (!BoostFlg)ButtonFlg[ChStd::EnumCast(KeyName::MAttack)] = true;

			MoveKey[0] = 'W';
			ButtonFlg[ChStd::EnumCast(KeyName::Up)] = true;

			switch (MoveKey[1])
			{
			case 'A':
				ButtonFlg[ChStd::EnumCast(KeyName::Left)] = true;
				break;
			case 'J':
				ButtonFlg[ChStd::EnumCast(KeyName::Jump)] = true;
				break;
			case 'S':
				//ButtonFlg[ChStd::EnumCast(KeyName::Down)] = true;
				MoveKey[1] = KeyCode[rand() % (5)];
				break;
			case 'D':
				ButtonFlg[ChStd::EnumCast(KeyName::Right)] = true;
				break;
			default:
				MoveKey[1] = KeyCode[rand() % (5)];
				break;
			}
		}
		else if (tmpLen > 100.0f) {
			if (!BoostFlg)ButtonFlg[ChStd::EnumCast(KeyName::MAttack)] = true;

			MoveKey[0] = 'W';
			ButtonFlg[ChStd::EnumCast(KeyName::Up)] = true;

			switch (MoveKey[1])
			{
			case 'A':
				ButtonFlg[ChStd::EnumCast(KeyName::Left)] = true;
				break;
			case 'J':
				ButtonFlg[ChStd::EnumCast(KeyName::Jump)] = true;
				break;
			case 'S':
				//ButtonFlg[ChStd::EnumCast(KeyName::Down)] = true;
				MoveKey[1] = KeyCode[rand() % (5)];
				break;
			case 'D':
				ButtonFlg[ChStd::EnumCast(KeyName::Right)] = true;
				break;
			default:
				MoveKey[1] = KeyCode[rand() % (5)];
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
					ButtonFlg[ChStd::EnumCast(KeyName::Left)] = true;
					break;
				case 'J':
					ButtonFlg[ChStd::EnumCast(KeyName::Jump)] = true;
					break;
				case 'S':
					ButtonFlg[ChStd::EnumCast(KeyName::Down)] = true;
					break;
				case 'D':
					ButtonFlg[ChStd::EnumCast(KeyName::Right)] = true;
					break;
				default:
					Change();
					break;
				}

			}
		}
	}

	if (rand() % (100) > SpNow)Change();

}

void BRobot::AIMoveMouInput()
{
	auto SpTmpRobot = WpBRobotsList.lock();
	if (SpTmpRobot == nullptr)return;


	ChVec3 tmpVec, tmpPos;
	ChVec3 tmpMove;
	ChLMat tmpMat;
	float tmpLen;

	tmpVec = Mat;
	Look->SetCamPos(&tmpVec);

	tmpVec.MatPos(&Mat, &ChVec3(0.0f, 0.0f, 1.0f));


	Look->SetCamLook(&tmpVec);
	Look->SetCamMat(&Mat);
	Look->SetView();

	tmpMat = TargetPos;

	ChDevice.GetOnViewPos(&tmpVec, &tmpMat);

	tmpPos = ChVec3(
		ChWinAPID.GetWindWidth() / 2.0f
		, ChWinAPID.GetWindHeight() / 2.0f
		, 0.0f);

	tmpPos.z = tmpVec.z;

	tmpMove = tmpPos - tmpVec;

	tmpMove *= (0.02f * ADS);
	CurrsolMove(std::fabsf(tmpMove.x),std::fabsf(tmpMove.y));

	TWindOnPosLen = tmpPos.GetLen(&tmpVec);

	if (tmpPos.z <= 0.0f)return;

	tmpLen = tmpVec.GetLen(&tmpPos);

	if (tmpLen > ViweLen)return;



	if (IsEnemy()) {
		if (!SpTmpRobot->IsMemberLive())return;
	}
	else {
		if (!SpTmpRobot->IsEnemyLive())return;
	}
	if (LookFlg)
	{

		ButtonFlg[ChStd::EnumCast(KeyName::MAttack)] = true;

		ButtonFlg[ChStd::EnumCast(KeyName::SAttack)] = true;
	}


}

void BRobot::CurrsolMove(const float _MoveX, const float _MoveY)
{
	if (!IsEnemy())TestData = _MoveX;

	ChVec2 tmpDir;

	ChVec3 tmpVec, UpVec;
	ChVec3 tmpCross, tmpVDir;
	ChLMat tmpMat;

	tmpVec = Mat;
	tmpVDir = TargetPos;
	tmpVec = tmpVDir - tmpVec;

	tmpVDir.MatNormal(&Mat, &ChVec3(0.0f, 0.0f, 1.0f));

	tmpVec.Normalize();

	tmpCross.CrossVec(&tmpVDir, &tmpVec);

	tmpDir.x = tmpCross.y > 0 ? _MoveX : -_MoveX;
	tmpDir.y = tmpCross.x > 0 ? _MoveY : -_MoveY;

	if (tmpVec.z < 0)tmpDir.y = -tmpDir.y;


	MoveRot.x = tmpDir.x;
	MoveRot.y = tmpDir.y;
}

void BRobot::Change()
{
	for (unsigned char i = 0; i < MoveKeyCnt; i++)
	{
		switch (rand() % (5))
		{
		case 0:
			MoveKey[i] = 'A';
			ButtonFlg[ChStd::EnumCast(KeyName::Left)] = true;
			break;
		case 1:
			MoveKey[i] = 'S';
			ButtonFlg[ChStd::EnumCast(KeyName::Down)] = true;
			break;
		case 2:
			MoveKey[i] = 'D';
			ButtonFlg[ChStd::EnumCast(KeyName::Right)] = true;
			break;
		case 3:
			MoveKey[i] = 'J';
			ButtonFlg[ChStd::EnumCast(KeyName::Jump)] = true;
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

	ChLMat tmpMat;
	tmpMat.RotYPR(0.0f, BaseD.Rot.y, 0.0f);
	CamMoveVec = ChVec3(0.0f, 0.0f, 0.0f);
	//キャラクター向き//

	if (abs(MoveRot.x) > 1.0f)BaseD.Rot.y += (MoveRot.x / MouMoveP) * Sens;
	if (abs(MoveRot.y) > 1.0f)BaseD.Rot.x += (MoveRot.y / MouMoveP) * Sens;

	if (abs(BaseD.Rot.x) > 90.0f)BaseD.Rot.x > 0.0f ? BaseD.Rot.x = 90.0f : BaseD.Rot.x = -90.0f;

	//キャラクター行動//
	ChVec3 tmpVec;
	ChVec3 UpVec;

	/*
		if (ButtonFlg[ChStd::EnumCast(KeyName::OverBoost)])
		{
			BoostFlg = !BoostFlg;
		}

	*/
	if (ButtonFlg[ChStd::EnumCast(KeyName::JAvo)] && MoveD.AvoV <= 0.0f)
	{
		CamMoveVec += ChVec3(0.0f, 1.0f, 0.0f);
		MoveD.AvoDir.MatNormal(&tmpMat, &ChVec3(0.0f, 1.0f, 0.0f));
		MoveD.AvoV = MoveD.AvoMax;
	}
	if (ButtonFlg[ChStd::EnumCast(KeyName::LAvo)] && MoveD.AvoV <= 0.0f)
	{
		CamMoveVec += ChVec3(-1.0f, 0.0f, 0.0f);
		MoveD.AvoDir.MatNormal(&tmpMat, &ChVec3(-1.0f, 0.0f, 0.0f));
		MoveD.AvoV = MoveD.AvoMax;
	}
	if (ButtonFlg[ChStd::EnumCast(KeyName::RAvo)] && MoveD.AvoV <= 0.0f)
	{
		CamMoveVec += ChVec3(1.0f, 0.0f, 0.0f);
		MoveD.AvoDir.MatNormal(&tmpMat, &ChVec3(1.0f, 0.0f, 0.0f));
		MoveD.AvoV = MoveD.AvoMax;
	}
	if (ButtonFlg[ChStd::EnumCast(KeyName::UAvo)] && MoveD.AvoV <= 0.0f)
	{
		CamMoveVec += ChVec3(0.0f, 0.0f, 1.0f);
		MoveD.AvoDir.MatNormal(&tmpMat, &ChVec3(0.0f, 0.0f, 1.0f));
		MoveD.AvoV = MoveD.AvoMax;
	}


	if (ButtonFlg[ChStd::EnumCast(KeyName::Up)])
	{
		CamMoveVec += ChVec3(0.0f, 0.0f, 1.0f);
		tmpVec.Move(0.0f, 0.0f, 1.0f);
	}
	if (ButtonFlg[ChStd::EnumCast(KeyName::Down)])
	{
		CamMoveVec += ChVec3(0.0f, 0.0f, -1.0f);
		tmpVec.Move(0.0f, 0.0f, -1.0f);
	}
	if (ButtonFlg[ChStd::EnumCast(KeyName::Left)])
	{
		CamMoveVec += ChVec3(-1.0f, 0.0f, 0.0f);
		tmpVec.Move(-1.0f, 0.0f, 0.0f);
	}
	if (ButtonFlg[ChStd::EnumCast(KeyName::Right)])
	{
		CamMoveVec += ChVec3(1.0f, 0.0f, 0.0f);
		tmpVec.Move(1.0f, 0.0f, 0.0f);
	}
	if (ButtonFlg[ChStd::EnumCast(KeyName::Jump)])
	{
		CamMoveVec += ChVec3(0.0f, 1.0f, 0.0f);
		tmpVec.Move(0.0f, 1.0f, 0.0f);
	}



	if (fabsf(tmpVec.GetLen(&D3DXVECTOR3(0.0f, 0.0f, 0.0f))) > 1.0f)tmpVec.Normalize();
	tmpVec.MatNormal(&tmpMat, &tmpVec);
	CamMoveVec.Normalize();


	MoveD.MoveV += tmpVec * MoveS;

	if (BoostFlg)MoveD.MoveV += tmpVec * BoostV;

	MoveD.MoveV += MoveD.AvoDir * MoveD.AvoV;
	MoveD.AvoV *= MoveD.AvoDen;

	if (fabsf(MoveD.AvoV) < MoveD.AvoMin)MoveD.AvoV = 0.0f;


	if (ButtonFlg[ChStd::EnumCast(KeyName::MAttack)])
	{
		Attack(RobParts->GetMWeap());
	}

	if (ButtonFlg[ChStd::EnumCast(KeyName::SAttack)])
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

	ChVec3 tmpCamP, tmpVec;

	tmpCamP.MatPos(&Mat, &BaseCamPos);

	Camera->SetCamPos(&tmpCamP);

	tmpCamP.MatPos(&Mat, &BaseCamLook);

	Camera->SetCamLook(&tmpCamP);

	Camera->SetCamMat(&Mat);

	Camera->SetLookMaxLen(4000.0f);
}

void BRobot::Move()
{
	ChVec3 tmpVec;

	auto RobotsList = WpBRobotsList.lock();
	if (RobotsList == nullptr)return;
	InputToMove();

	ChPtr::Shared<HitPosList> tmpHPos = WpHitPosList.lock();


	AvoHitRobots();
	{

		ChStd::Bool PhyFlg = ChStd::False;
		for (auto Phy : Physical)
		{
			if (Phy->UpDate(&BaseD.Pos, &MoveD.MoveV))PhyFlg = true;
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
		ChVec3 tmpPos = BaseD.Pos;
		tmpPos.y += 5.0f;
		float tmpLen;
		if (ChObjCon9.SmpXFileHitRay(
			ChMeMa.GetSmpXFile("Field")
			, nullptr
			, &BaseD.Pos
			, &ChVec3(0.0f, -1.0f, 0.0f), nullptr, &tmpLen))
		{
			if (tmpLen < 5.0f
				&& MoveD.MoveV.GetLen(&ChVec3()) < 0.1f
				&& !RobotsList->IsEnemyLive())Mat.RotYPR(0.0f, BaseD.Rot.y, 0.0f);
		}
	}

	MoveD.MoveV = 0.0f;
	float tmpLen = BaseD.Pos.GetLen();

	if (!IsEnemy())TestPosData = BaseD.Pos;
	//if (!IsEnemy())TestData = tmpLen;

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
	ChVec3 tmpVec;
	float tmpLen;

	for (DWORD i = 0; i < SpTmpRobot->GetRobotsCnt(); i++)
	{
		tmpVec = *SpTmpRobot->GetMat(i);
		if (tmpVec == BaseD.Pos)continue;
		tmpLen = BaseD.Pos.GetLen(&tmpVec);
		if (tmpLen > Len)continue;
		tmpVec = tmpVec - BaseD.Pos;
		tmpVec.Normalize();
		BaseD.Pos -= tmpVec * tmpLen;
	}
}

void BRobot::Attack(const ChPtr::Shared<ChGame::WeapData> _Weap)
{
	auto HitPos = WpHitPosList.lock();

	if (_Weap == nullptr)return;
	if (HitPos == nullptr)return;

	ChVec3 tmpVec;
	ChLMat tmpMat;

	tmpMat = Mat;

	tmpMat = BaseD.Pos + MoveD.MoveV;

	tmpVec = _Weap->ShotPos;
	auto tmpAData = ChPtr::Make_S<ChGame::APosData>();
	tmpAData->Base = *_Weap;
	tmpAData->Base.Damage += AddDamage;
	tmpVec.MatPos(&tmpMat, &tmpVec);
	tmpAData->Pos = tmpVec;
	ChDSound9.PlaySE(tmpAData->Base.SEName, "Sound", &tmpVec);
	tmpVec.MatNormal(&Mat, &ChVec3(0.0f, 0.0f, tmpAData->Base.Speed));

	tmpAData->Base.Speed += MoveD.MoveV.GetLen(&ChVec3(0.0f, 0.0f, 0.0f));


	//tmpVec = MoveD.MoveV + tmpVec;
	tmpVec.Normalize();

	tmpAData->Dir = tmpVec;

	HitPos->SetAData(tmpAData);

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

	ChDevice.LightSetting(true);

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

void BaseRobotsList::SetRobots(std::string _PlayerData, const ChVec3 *_StartPos, unsigned short _PartsNum)
{
	RobotsList.push_back(ChPtr::Make_S<PlayerObj>());
	RobotsList[RobotsList.size() - 1]->Init(0);
	RobotsList[RobotsList.size() - 1]->SetPos(_StartPos);
	RobotsList[RobotsList.size() - 1]->SetHitPosList(WpHitPosList.lock());
	RobotsList[RobotsList.size() - 1]->SetRobotsList(shared_from_this());

	auto tmpDData = ChPtr::Make_S<Damage>();
	RobotsList[RobotsList.size() - 1]->SharedDamageData(tmpDData);
	DBase->SharedDamageData(tmpDData);

	MemberCnt++;
}

void BaseRobotsList::SetRobots(const ChStd::Bool _EneFlg, ChVec3 *_StartPos, unsigned short _PartsNum)
{
	if (!_EneFlg)RobotsList.push_back(ChPtr::Make_S<EnemyObj>(ChStd::EnumCast(EnemyType, Boss), _EneFlg));
	else RobotsList.push_back(ChPtr::Make_S<EnemyObj>(ChStd::EnumCast(EnemyType, Normal), _EneFlg));
	RobotsList[RobotsList.size() - 1]->Init(_PartsNum);
	RobotsList[RobotsList.size() - 1]->SetPos(_StartPos);
	RobotsList[RobotsList.size() - 1]->SetHitPosList(WpHitPosList.lock());
	RobotsList[RobotsList.size() - 1]->SetRobotsList(shared_from_this());

	auto tmpDData = ChPtr::Make_S<Damage>();
	RobotsList[RobotsList.size() - 1]->SharedDamageData(tmpDData);
	DBase->SharedDamageData(tmpDData);

	if (_EneFlg)EnemyCnt++;
	else MemberCnt++;

}

void BaseRobotsList::SetRobots2(const ChStd::Bool _EneFlg, ChVec3 *_StartPos)
{

	if (!_EneFlg)RobotsList.push_back(std::make_shared<EnemyObj2>(_EneFlg));
	else RobotsList.push_back(std::make_shared<EnemyObj2>(_EneFlg));
	RobotsList[RobotsList.size() - 1]->Init(0);
	RobotsList[RobotsList.size() - 1]->SetPos(_StartPos);
	RobotsList[RobotsList.size() - 1]->SetHitPosList(WpHitPosList.lock());
	RobotsList[RobotsList.size() - 1]->SetRobotsList(shared_from_this());

	auto tmpDData = ChPtr::Make_S<Damage>();
	RobotsList[RobotsList.size() - 1]->SharedDamageData(tmpDData);
	DBase->SharedDamageData(tmpDData);

	if (_EneFlg)EnemyCnt++;
	else MemberCnt++;
}

const ChLMat* BaseRobotsList::GetCamPos()
{
	tmpMat.Identity();
	ChPtr::Shared<BRobot>tmpRobot = WpCamRobot.lock();
	if (tmpRobot == nullptr)return &tmpMat;
	tmpMat = *tmpRobot->GetCamPos();
	return &tmpMat;
}

const ChLMat* BaseRobotsList::GetMat(const DWORD _No)
{
	tmpMat.Identity();
	if (RobotsList.size() <= _No)return &tmpMat;
	tmpMat = *RobotsList[_No]->GetMat();
	return &tmpMat;
}

const ChLMat* BaseRobotsList::GetCamLookPos()
{

	tmpMat.Identity();
	ChPtr::Shared<BRobot>tmpRobot = WpCamRobot.lock();
	if (tmpRobot == nullptr)return &tmpMat;
	tmpMat = *tmpRobot->GetMat();
	return &tmpMat;
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
	ChVec3 tmpVec;

	ChVec3 tmpPos;

	ChLMat tmpMat, tmpRMat;
	float tmpLen;


	POINT Cnt;
	Cnt.x = 0;
	Cnt.y = 0;

	for (auto Obj : RobotsList)
	{
		if (!Obj->IsEnemy())continue;

		tmpMat = *Obj->GetMat();

		ChDevice.GetOnViewPos(&tmpVec, &tmpMat);

		tmpPos = ChVec3(
			ChWinAPID.GetWindWidth() / 2.0f
			, ChWinAPID.GetWindHeight() / 2.0f
			, 0.0f);

		tmpPos.z = tmpVec.z;

		float tmpLen = tmpVec.GetLen(&tmpPos);

		ChVec3 tmpVec1, tmpVec2;
		tmpMat = *Robot->GetMat();
		tmpVec1 = tmpMat;
		tmpVec2 = *Obj->GetMat();
		tmpMat.Inverse();
		tmpVec2.MatPos(&tmpMat, &tmpVec2);

		if (tmpVec2.z > 0.0f)
		{
			if (0.0f <= tmpVec.x && tmpVec.x <= (float)ChWinAPID.GetWindWidth())
			{

				if (0.0f <= tmpVec.y && tmpVec.y <= (float)ChWinAPID.GetWindHeight())
				{


					if (tmpLen > 300.0f)continue;

					tmpMat.Identity();
					tmpMat.ScalingMode(100.0f / 64.0f);

					//tmpVec.z = 0.0f;
					tmpMat = tmpVec;

					if (tmpPos.z <= 0.0f)continue;

					ChSp9.DrawSprite(
						ChTexMa.GetTexture("TargetMa")
						, &tmpMat
						, &ChVec3(0.0f, 0.0f, 0.0f)
						, nullptr);

					continue;
				}
			}

		}





		tmpPos = ChVec3(
			ChWinAPID.GetWindWidth() / 2.0f
			, ChWinAPID.GetWindHeight() * 1.0f / 3.0f
			, 0.0f);

		tmpMat.Identity();



		if (tmpVec2.x > 0.0f) {
			tmpPos.x = ChWinAPID.GetWindWidth() - 64.0f;
			tmpPos.y += 128.0f * Cnt.x;
			Cnt.x++;
			tmpMat = tmpPos;

			ChSp9.DrawSprite(
				ChTexMa.GetTexture("Right")
				, &tmpMat
				, &ChVec3(0.0f, 0.0f, 0.0f)
				, nullptr);
			continue;
		}

		if (tmpVec2.x < 0.0f)
		{

			tmpPos.x = 64.0f;
			tmpPos.y += 128.0f * Cnt.y;
			Cnt.y++;
			tmpMat = tmpPos;

			ChSp9.DrawSprite(
				ChTexMa.GetTexture("Left")
				, &tmpMat
				, &ChVec3(0.0f, 0.0f, 0.0f)
				, nullptr);
			continue;
		}


	}

	tmpMat.Identity();
	tmpVec = *Robot->GetMoveVec();

	tmpLen = tmpVec.GetLen();
	tmpVec.Normalize();


	tmpMat.ScalingMode(32.0f / 64.0f);

	tmpPos = ChVec3(
		ChWinAPID.GetWindWidth() / 2.0f
		, ChWinAPID.GetWindHeight() / 2.0f - 30.0f
		, 0.0f);
	tmpPos -= tmpVec * tmpLen * 70.0f;

	tmpPos.y = (ChWinAPID.GetWindHeight() / 2.0f - 50.0f)
		- ((tmpPos.y / 25.0f) * (tmpLen - tmpVec.z));

	tmpPos.z = 0.0f;

	tmpMat = tmpPos;


	ChSp9.DrawSprite(
		ChTexMa.GetTexture("ATC")
		, &tmpMat
		, &ChVec3(0.0f, 0.0f, 0.0f)
		, nullptr);


	tmpPos = ChVec3(
		ChWinAPID.GetWindWidth() / 2.0f
		, ChWinAPID.GetWindHeight() / 2.0f
		, 0.0f);

	tmpMat.ScalingMode(600.0f / 128.0f);

	tmpMat = tmpPos;

	{

		float tmpR, tmpG, tmpB;
		tmpG = Robot->GetPraHp() < 50 ? 255.0f * (2.0f * Robot->GetPraHp())
			: 255.0f - 255.0f * (2.0f * Robot->GetPraHp());
		tmpB = Robot->GetPraHp() * 255.0f;
		tmpR = 255 - Robot->GetPraHp() * 255.0f;
		ChStd::COLOR255 tmpCol;
		tmpCol.a = 255;
		tmpCol.r = (unsigned char)tmpR;
		tmpCol.g = 0;
		tmpCol.b = (unsigned char)tmpB;
		ChTexMa.SetBlendColor(&tmpCol, "Wind");
	}

	ChSp9.DrawSprite(
		ChTexMa.GetTexture("Wind")
		, &tmpMat
		, &ChVec3(0.0f, 0.0f, 0.0f)
		, nullptr);

	//char tmpFont[200];
	/*

			ChDFont9.SetFontSize(16, 8);
			sprintf(tmpFont, "%.2f", TestData);

			ChDFont9.Draw(tmpFont, 0, 0);
	*/
	/*
			sprintf(tmpFont, "%.2f\n%.2f\n%.2f", TestPosData.x, TestPosData.y,TestPosData.z);

			ChDFont9.Draw(tmpFont, 0, 90);
	*/
	/*
			sprintf(tmpFont, "%d", ChtmpCnt);

			ChDFont9.Draw(tmpFont, 0, 90 * 4);
		*/
}
