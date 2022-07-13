
#include"../ChDXLibrary/ChGameIncludeFile.h"
#include"../AllStruct.h"
#include"../HitPosList/HitPosList.h"
#include"../Damage/Damage.h"
#include"BaseRobot.h"
#include"RobParts.h"

RobParts::~RobParts()
{
	PartsList.clear();
}

void RobParts::Init(
	ChPtr::Shared<RobPartsList> _RobPartsList
	, std::string _PartsName)
{
	if (_RobPartsList == nullptr)return;
	if (WpRobParts.lock() != nullptr)return;
	WpRobParts = _RobPartsList;

	auto TmpXList = WpXList.lock();
	if (TmpXList == nullptr)return;

	

	SetParts(_PartsName);

}

void RobParts::SetParts(std::string _PartsName)
{
	ChPtr::Shared<ChSmpXList9> TmpXList = WpXList.lock();

	if (TmpXList == nullptr)return;

	std::string TmpString = "Normal.x";

	if (_PartsName.length() <= 0)
	{
		TmpString = XFilePath + TmpString;

		TmpXList->SetSmpXFile(
			TmpString
			, PType
		);

		BBox.SetBBoxToXFile(TmpXList->GetSmpXFile(PType));

		SetDitailsData(TmpString);

		return;
	}

	TmpString = XFilePath + _PartsName;

	TmpXList->SetSmpXFile(
		TmpString
		, PType
	);

	BBox.SetBBoxToXFile(TmpXList->GetSmpXFile(PType));

	SetDitailsData(TmpString);

}

void RobParts::SetPColor(const ChStd::COLOR1f* _Color)
{
	ChPtr::Shared<ChSmpXList9>TmpXList = WpXList.lock();

	if (TmpXList == nullptr)return;

	for (auto Mate : TmpXList->GetXFileMaterials(PType))
	{
		if (Mate->Name != "Color")continue;
		Mate->Diffuse.a = _Color->a;
		Mate->Diffuse.r = _Color->r;
		Mate->Diffuse.g = _Color->g;
		Mate->Diffuse.b = _Color->b;
	}

}

void RobParts::Draw(const ChMat_9 *_Mat)
{
	ChPtr::Shared<ChSmpXList9>TmpXList = WpXList.lock();

	if (TmpXList == nullptr)return;

	ChMat_9 TmpMat = *_Mat;
	TmpMat = BaseMat * TmpMat;

	TmpXList->DrawSmpXFile(&TmpMat,PType);
/*
	ChEffectDraw.SmpXfileNormalDraw(
		TmpXList->GetSmpXFile(PType)
		, &TmpMat
		, ChNoEffect);
*/
}

RPFoot::RPFoot()
{
	HP = 100;
	XFilePath = "Foot/";
	PType = GetEnumCNum(RobPartsList::BodyPartsListName, Foot);
	BaseMat.Trans(&ChVec3_9(0.0f, 0.0f, 0.0f));
}

RPFoot::~RPFoot()
{

}

void RPFoot::SetPColor(const ChStd::COLOR1f* _Color)
{

	ChPtr::Shared<ChSmpXList9>TmpXList = WpXList.lock();

	if (TmpXList == nullptr)return;

	for (auto Mate : TmpXList->GetXFileMaterials(PType))
	{
		if (Mate->Name != "Color")continue;
		Mate->Diffuse.a = _Color->a;
		Mate->Diffuse.r = _Color->r;
		Mate->Diffuse.g = _Color->g;
		Mate->Diffuse.b = _Color->b;
	}


}

void RPFoot::SetStandardParts()
{

	PartsList.push_back(ChPtr::Make_S<PartsData>());

	PartsList.push_back(ChPtr::Make_S<PartsData>());

	PartsList.push_back(ChPtr::Make_S<PartsData>());

}

RPBody::RPBody()
{
	HP = 100;
	XFilePath = "Body/";
	PType = GetEnumCNum(RobPartsList::BodyPartsListName, Body);
	BaseMat.Trans(&ChVec3_9(0.0f, 0.0f, 0.0f));
}

RPBody::~RPBody()
{

}

void RPBody::SetStandardParts()
{

	PartsList.push_back(ChPtr::Make_S<PartsData>());

}

RPHead::RPHead(const ChStd::Bool _WeapFlg)
{
	HP = 100;
	if (_WeapFlg)Weap = ChPtr::Make_U<ChGame::WeapData>();
	XFilePath = "Head/";
	PType = GetEnumCNum(RobPartsList::BodyPartsListName, Head);
	BaseMat.Trans(&ChVec3_9(0.0f, 2.1f, 0.0f));
}

RPHead::~RPHead()
{
	if (Weap != nullptr)Weap = nullptr;

}

void RPHead::Update()
{
	WeakCnt++;
	if (WeakCnt >= WeakFreshCnt)WeakCnt = WeakFreshCnt;
}

void RPHead::SetStandardParts()
{

	PartsList.push_back(ChPtr::Make_S<PartsData>());
}

RPArm::RPArm()
{
	HP = 100;
	XFilePath = "Arm/";
	PType = GetEnumCNum(RobPartsList::BodyPartsListName, Arm);
	BaseMat.Trans(&ChVec3_9(0.0f, 1.6f, 0.0f));
}

RPArm::~RPArm()
{

}

void RPArm::SetPColor(const ChStd::COLOR1f* _Color)
{

	ChPtr::Shared<ChSmpXList9>TmpXList = WpXList.lock();

	if (TmpXList == nullptr)return;

	for (auto Mate : TmpXList->GetXFileMaterials(PType))
	{
		if (Mate->Name != "Color")continue;
		Mate->Diffuse.a = _Color->a;
		Mate->Diffuse.r = _Color->r;
		Mate->Diffuse.g = _Color->g;
		Mate->Diffuse.b = _Color->b;
	}

}

void RPArm::SetStandardParts()
{

	PartsList.push_back(ChPtr::Make_S<PartsData>());

	PartsList.push_back(ChPtr::Make_S<PartsData>());

}

RPBoost::RPBoost()
{
	HP = 100;
	XFilePath = "Boost/";
	PType = GetEnumCNum(RobPartsList::BodyPartsListName, Boost);
	BaseMat.Trans(&ChVec3_9(0.0f, 1.2f, 0.0f));
}

RPBoost::~RPBoost()
{

}

void RPBoost::Draw(const ChMat_9 *_Mat)
{

	ChPtr::Shared<ChSmpXList9>TmpXList = WpXList.lock();

	if (TmpXList == nullptr)return;

	for (auto Mate : TmpXList->GetXFileMaterials(PType))
	{
		for (auto Boost : BoostPowList)
		{
			if (Mate->Name != Boost.first)continue;
			Mate->Mat.ScalingMode(Boost.second);
		}
	}

	ChMat_9 TmpMat = *_Mat;
	TmpMat = BaseMat * TmpMat;

	TmpXList->DrawSmpXFile(&TmpMat, PType);

/*
	for (int i = 0; i < TmpXList->GetXFileMaterials(PType).size(); i++)
	{
		if (TmpXList->GetXFileMaterials(PType)[i]->Name != "Boost"
			&& TmpXList->GetXFileMaterials(PType)[i]->Name != "Color")
		{
			ChDevice.AddBlendSetting();
		}
		else
		{

			ChDevice.AlphaBlendSetting();
		}
		
		TmpXList->DrawSmpXFileToSubSet(&TmpMat, PType, i);

	}
*/
	ChDevice.AlphaBlendSetting();
/*
	ChEffectDraw.SmpXfileNormalDraw(
		TmpXList->GetSmpXFile(PType)
		, &TmpMat
		, ChNoEffect);
*/
	ChDevice.LightSetting(ChStd::False);



	ChDevice.LightSetting(ChStd::True);
}


void RPBoost::SetStandardParts()
{

	PartsList.push_back(ChPtr::Make_S<PartsData>());

	

}

void RPBoost::SetParts(std::string _PartsName)
{
	ChPtr::Shared<ChSmpXList9> TmpXList = WpXList.lock();

	if (TmpXList == nullptr)return;

	std::string TmpString = "Normal.x";

	if (_PartsName.length() <= 0)
	{
		TmpString = XFilePath + TmpString;

		TmpXList->SetSmpXFile(
			TmpString
			, PType
		);

		BBox.SetBBoxToXFile(TmpXList->GetSmpXFile(PType));

		SetDitailsData(TmpString);

		SetBoost();

		return;
	}

	TmpString = XFilePath + _PartsName;

	TmpXList->SetSmpXFile(
		TmpString
		, PType
	);

	BBox.SetBBoxToXFile(TmpXList->GetSmpXFile(PType));

	SetDitailsData(TmpString);

	SetBoost();
}

void RPBoost::SetBoost()
{
	ChPtr::Shared<ChSmpXList9>TmpXList = WpXList.lock();

	if (TmpXList == nullptr)return;

	for (auto Mate : TmpXList->GetXFileMaterials(PType))
	{
		if (Mate->Name == "Boost" || Mate->Name == "Color")continue;
		Mate->Mat.ScalingMode(0.0f);
		Mate->Diffuse.a = 0.0f;
		Mate->Diffuse.r = 0.0f;
		Mate->Diffuse.g = 0.0f;
		Mate->Diffuse.b = 0.0f;
	}
}

void RPBoost::IsBoost()
{
	if (WpRobParts.lock() == nullptr)return;
	auto TmpPartsList = WpRobParts.lock();

	if (TmpPartsList->GetBRobot() == nullptr)return;
	auto Rob = TmpPartsList->GetBRobot();

	if (WpXList.lock() == nullptr)return;
	auto TmpXList = WpXList.lock();

	ChVec3_9 TmpVec;
	float TmpMovePow;
	ChMat_9 TmpMat;


	TmpMovePow = Rob->GetMoveData().AvoV;
	
	TmpVec = Rob->GetMoveData().AvoDir;

	TmpMat = *Rob->GetMat();
	TmpMat.Inverse();

	TmpVec.MatNormal(&TmpMat, &TmpVec);

}

RPWeapon::RPWeapon()
{
	HP = 100;
	Weap = ChPtr::Make_U<ChGame::WeapData>();
	XFilePath = "Weapon/";
	PType = GetEnumCNum(RobPartsList::BodyPartsListName, MWeap);

	BaseMat.RotYPR(-90.0f, 0.0f, 0.0f);

	BaseMat = ChVec3_9(0.0f, -0.09f, 0.0f);
}

RPWeapon::~RPWeapon()
{
	Weap = nullptr;
	APos = nullptr;
}

void RPWeapon::Update()
{
	WeakCnt++;
	if (WeakCnt >= WeakFreshCnt)WeakCnt = WeakFreshCnt;
}

void RPWeapon::SetParts(std::string _PartsName)
{

	ChPtr::Shared<ChSmpXList9> TmpXList = WpXList.lock();

	if (TmpXList == nullptr)return;

	std::string TmpString = "GunSample.x";

	if (_PartsName.length() <= 0)
	{
		TmpString = XFilePath + TmpString;

		TmpXList->SetSmpXFile(
			TmpString
			, PType
		);
		BBox.SetBBoxToXFile(TmpXList->GetSmpXFile(PType));

		SetDitailsData(TmpString);
		SetStandardParts();
		Weap->SEName = "SE_Shot.wav";

		return;
	}

	TmpString = XFilePath + _PartsName;

	TmpXList->SetSmpXFile(
		TmpString
		, PType
	);

	BBox.SetBBoxToXFile(TmpXList->GetSmpXFile(PType));
	SetStandardParts();
	SetDitailsData(TmpString);
	Weap->SEName = "SE_Shot2.wav";

	Weap->Damage = 10;
	Weap->HitLen = 2.0f;
	Weap->ShotPos.MatPos(&BaseMat, &ChVec3_9(0.0f, -10.0f, 0.0f));
	Weap->Speed = 3.0f;

	WeakCnt = 0;
	WeakFreshCnt = 20;
}

void RPWeapon::SetStandardParts()
{

	PartsList.push_back(ChPtr::Make_S<PartsData>());

	Weap = ChPtr::Make_S<ChGame::WeapData>();
	APos = ChPtr::Make_S<ChGame::APosData>();
	Weap->Damage = 5;
	Weap->HitLen = 2.0f;
	Weap->ShotPos.MatPos(&BaseMat, &ChVec3_9(0.0f, -10.0f, 0.0f));
	Weap->Speed = 5.0f;

	WeakCnt = 0;
	WeakFreshCnt = 5;
}

void RobPartsList::Init(ChPtr::Shared<BRobot>_BRobot)
{
	if (_BRobot == nullptr)return;
	if (WpSetRobot.lock() != nullptr)return;

	WpSetRobot = _BRobot;

	XList = ChPtr::Make_S<ChSmpXList9>(ChDevice.GetDevice(), "../é¿çs/data/XFile/RobotsParts");

	/*
		std::string TmpStr;
		TmpStr = TmpStr.substr(0, 3);
		std::string::iterator TmpIt;
		TmpIt = TmpStr.begin();
		for (unsigned short i = 0 ; TmpIt != TmpStr.end(); i++)
		{
			if (TmpStr[i] == ' ')
			{
				TmpStr.erase(TmpIt);
				continue;
			}

			i++;
			TmpIt++;
		}
	*/

	SetMWeapParts("TmpObj.x");

	return;

}

void RobPartsList::Init(ChPtr::Shared<BRobot>_BRobot, unsigned short _RobotNum)
{
	if (_BRobot == nullptr)return;
	if (WpSetRobot.lock() != nullptr)return;

	WpSetRobot = _BRobot;

	XList = ChPtr::Make_S<ChSmpXList9>(ChDevice.GetDevice(), "../é¿çs/data/XFile/RobotsParts");
	
	TestDate.push_back(ChPtr::Make_S <std::string>());
	TestDate.push_back(ChPtr::Make_S <std::string>());
	TestDate.push_back(ChPtr::Make_S <std::string>());
	*TestDate[0] = "Normal.x";
	*TestDate[1] = "Guardian.x";
	*TestDate[2] = "Air.x";
	unsigned short TmpNum = _RobotNum % 3;
/*
	std::string TmpStr;
	TmpStr = TmpStr.substr(0, 3);
	std::string::iterator TmpIt;
	TmpIt = TmpStr.begin();
	for (unsigned short i = 0 ; TmpIt != TmpStr.end(); i++)
	{
		if (TmpStr[i] == ' ')
		{
			TmpStr.erase(TmpIt);
			continue;
		}

		i++;
		TmpIt++;
	}
*/

	std::string TmpString = *TestDate[TmpNum];

	SetFootParts(TmpString);

	SetBodyParts(TmpString);

	SetHeadParts(TmpString);

	SetArmParts(TmpString);

	SetBoostParts(TmpString);

	SetMWeapParts("\0");

	SetSWeapParts("\0");

	return;

}

void RobPartsList::Draw()
{
	auto Obj = WpSetRobot.lock();
	if (Obj == nullptr)return;

	for (ChStd::DataNo i = 0; i < PartsCnt; i++)
	{
		if (PartsList.find(i) == PartsList.end())continue;
		PartsList[i]->Draw(Obj->GetDrawMat());
	}
}

void RobPartsList::SetColor(
	const ChStd::DataNo _PartsNo
	, const ChStd::COLOR1f* _Color)
{
	if (_PartsNo > PartsList.size())return;

	PartsList[_PartsNo]->SetPColor(_Color);

}

float RobPartsList::SetFootParts(std::string _PartsName)
{
	ChStd::DataNo TmpNum = GetEnumCNum(BodyPartsListName, Foot);

	PartsList.insert(
		std::pair
		<ChStd::DataNo,ChPtr::Shared<RobParts>>
		(TmpNum,ChPtr::Make_S<RPFoot>()));

	PartsList[TmpNum]->SetXList(XList);
	PartsList[TmpNum]->Init(shared_from_this(), _PartsName);
	return 0.0f;
}

float RobPartsList::SetBodyParts(std::string _PartsName)
{
	ChStd::DataNo TmpNum = GetEnumCNum(BodyPartsListName, Body);

	PartsList.insert(
		std::pair
		<ChStd::DataNo, ChPtr::Shared<RobParts>>
		(TmpNum, ChPtr::Make_S<RPBody>()));

	PartsList[TmpNum]->SetXList(XList);
	PartsList[TmpNum]->Init(shared_from_this(), _PartsName);

	return 0.0f;
}

float RobPartsList::SetHeadParts(std::string _PartsName)
{
	ChStd::DataNo TmpNum = GetEnumCNum(BodyPartsListName, Head);

	auto TmpHead = ChPtr::Make_S<RPHead>(ChStd::False);

	PartsList.insert(
		std::pair
		<ChStd::DataNo, ChPtr::Shared<RobParts>>
		(TmpNum, TmpHead));

	PartsList[TmpNum]->SetXList(XList);
	PartsList[TmpNum]->Init(shared_from_this(), _PartsName);

	if (ChStd::False)WpHeadWeapon = TmpHead;
	return 0.0f;
}

void RobPartsList::SetArmParts(std::string _PartsName)
{
	ChStd::DataNo TmpNum = GetEnumCNum(BodyPartsListName, Arm);

	PartsList.insert(
		std::pair
		<ChStd::DataNo, ChPtr::Shared<RobParts>>
		(GetEnumCNum(BodyPartsListName, Arm), ChPtr::Make_S<RPArm>()));

	PartsList[TmpNum]->SetXList(XList);
	PartsList[TmpNum]->Init(shared_from_this(), _PartsName);

}

float RobPartsList::SetBoostParts(std::string _PartsName)
{
	ChStd::DataNo TmpNum = GetEnumCNum(BodyPartsListName, Boost);

	PartsList.insert(
		std::pair
		<ChStd::DataNo, ChPtr::Shared<RobParts>>
		(TmpNum, ChPtr::Make_S<RPBoost>()));
	PartsList[TmpNum]->SetXList(XList);
	PartsList[TmpNum]->Init(shared_from_this(), _PartsName);
	return 0.0f;
}

void RobPartsList::SetMWeapParts(std::string _PartsName)
{
	
	//if (PartsList.find(GetEnumCNum(BodyPartsListName, Arm)) == PartsList.end())return;
	//if (WpHeadWeapon.lock() != nullptr)return;

	ChStd::DataNo TmpNum = GetEnumCNum(BodyPartsListName, MWeap);

	ChPtr::Shared<RPWeapon>TmpWeap = ChPtr::Make_S<RPWeapon>();
	PartsList.insert(
		std::pair
		<ChStd::DataNo, ChPtr::Shared<RobParts>>
		(TmpNum, TmpWeap));

	PartsList[TmpNum]->SetXList(XList);
	PartsList[TmpNum]->Init(shared_from_this(), _PartsName);
	WpSWeapons = TmpWeap;
}

void RobPartsList::SetSWeapParts(std::string _PartsName)
{

	//if (WpHeadWeapon.lock() != nullptr)return;
	ChStd::DataNo TmpNum = GetEnumCNum(BodyPartsListName, SWeap);

	ChPtr::Shared<RPWeapon>TmpWeap = ChPtr::Make_S<RPWeapon>();

	PartsList.insert(
		std::pair
		<ChStd::DataNo, ChPtr::Shared<RobParts>>
		(TmpNum, TmpWeap));

	PartsList[TmpNum]->SetXList(XList);
	PartsList[TmpNum]->Init(shared_from_this(), _PartsName);
	WpSWeapons = TmpWeap;

}

ChStd::Bool RobPartsList::IsPosHit(
	const ChGame::APosData* _AData)
{
	if (ChPtr::NullCheck(_AData))return ChStd::False;
	ChPtr::Shared<BRobot> TmpRob = WpSetRobot.lock();
	if (TmpRob == nullptr)return ChStd::False;
	ChMat_9 TmpMat;
	ChB_Box9 TmpBBox;
	ChGame::APosData TmpData = *_AData;
	for (ChStd::DataNo i = 0; i < PartsCnt; i++)
	{
		if (PartsList.find(i) == PartsList.end())continue;
		if (PartsList[i]->IsBreak())continue;
		TmpBBox = *PartsList[i]->GetBBox();

		if (!TmpBBox.IsHitToBull(
			TmpRob->GetMat()
			, &TmpData.Pos
			, TmpData.Base.HitLen))continue;

		TmpRob->IsDamage(TmpData.Base.Damage);
		TmpRob->SetDData(&TmpData.Pos);
		return ChStd::True;
	}
	return ChStd::False;
}

ChPtr::Shared<ChGame::WeapData> RobPartsList::GetMWeap()
{
	auto WHead = WpHeadWeapon.lock();
	auto Weap = WpMWeapons.lock();

	if (WHead == nullptr && Weap == nullptr)return nullptr;
	if (WHead != nullptr)return WHead->GetWeapDate();
	
	return Weap->GetWeapDate();
}

ChPtr::Shared<ChGame::WeapData> RobPartsList::GetSWeap()
{
	auto WHead = WpHeadWeapon.lock();
	auto Weap = WpSWeapons.lock();

	if (WHead == nullptr && Weap == nullptr)return nullptr;
	if (WHead != nullptr)return WHead->GetWeapDate();

	return Weap->GetWeapDate();
}

void RobPartsList::Updata()
{
	for (ChStd::DataNo i = 0; i < PartsCnt; i++)
	{
		if (PartsList.find(i) == PartsList.end())continue;
		PartsList[i]->Update();
	}

}