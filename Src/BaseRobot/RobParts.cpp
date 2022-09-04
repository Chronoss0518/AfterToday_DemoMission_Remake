#include<Windows.h>
#include<ChBaseLibrary.h>
#include<ChWindowsLibrary.h>
#include<ChDirect3D11Library.h>

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

	auto tmpXList = WpXList.lock();
	if (tmpXList == nullptr)return;

	

	SetParts(_PartsName);

}

void RobParts::SetParts(std::string _PartsName)
{
	ChPtr::Shared<ChSmpXList9> tmpXList = WpXList.lock();

	if (tmpXList == nullptr)return;

	std::string tmpString = "Normal.x";

	if (_PartsName.length() <= 0)
	{
		tmpString = XFilePath + tmpString;

		tmpXList->SetSmpXFile(
			tmpString
			, PType
		);

		BBox.SetBBoxToXFile(tmpXList->GetSmpXFile(PType));

		SetDitailsData(tmpString);

		return;
	}

	tmpString = XFilePath + _PartsName;

	tmpXList->SetSmpXFile(
		tmpString
		, PType
	);

	BBox.SetBBoxToXFile(tmpXList->GetSmpXFile(PType));

	SetDitailsData(tmpString);

}

void RobParts::SetPColor(const ChStd::COLOR1f* _Color)
{
	ChPtr::Shared<ChSmpXList9>tmpXList = WpXList.lock();

	if (tmpXList == nullptr)return;

	for (auto Mate : tmpXList->GetXFileMaterials(PType))
	{
		if (Mate->Name != "Color")continue;
		Mate->Diffuse.a = _Color->a;
		Mate->Diffuse.r = _Color->r;
		Mate->Diffuse.g = _Color->g;
		Mate->Diffuse.b = _Color->b;
	}

}

void RobParts::Draw(const ChLMat *_Mat)
{
	ChPtr::Shared<ChSmpXList9>tmpXList = WpXList.lock();

	if (tmpXList == nullptr)return;

	ChLMat tmpMat = *_Mat;
	tmpMat = BaseMat * tmpMat;

	tmpXList->DrawSmpXFile(&tmpMat,PType);
/*
	ChEffectDraw.SmpXfileNormalDraw(
		tmpXList->GetSmpXFile(PType)
		, &tmpMat
		, ChNoEffect);
*/
}

RPFoot::RPFoot()
{
	HP = 100;
	XFilePath = "Foot/";
	PType = GetEnumCNum(RobPartsList::BodyPartsListName, Foot);
	BaseMat.Trans(&ChVec3(0.0f, 0.0f, 0.0f));
}

RPFoot::~RPFoot()
{

}

void RPFoot::SetPColor(const ChStd::COLOR1f* _Color)
{

	ChPtr::Shared<ChSmpXList9>tmpXList = WpXList.lock();

	if (tmpXList == nullptr)return;

	for (auto Mate : tmpXList->GetXFileMaterials(PType))
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
	BaseMat.Trans(&ChVec3(0.0f, 0.0f, 0.0f));
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
	BaseMat.Trans(&ChVec3(0.0f, 2.1f, 0.0f));
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
	BaseMat.Trans(&ChVec3(0.0f, 1.6f, 0.0f));
}

RPArm::~RPArm()
{

}

void RPArm::SetPColor(const ChStd::COLOR1f* _Color)
{

	ChPtr::Shared<ChSmpXList9>tmpXList = WpXList.lock();

	if (tmpXList == nullptr)return;

	for (auto Mate : tmpXList->GetXFileMaterials(PType))
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
	BaseMat.Trans(&ChVec3(0.0f, 1.2f, 0.0f));
}

RPBoost::~RPBoost()
{

}

void RPBoost::Draw(const ChLMat *_Mat)
{

	ChPtr::Shared<ChSmpXList9>tmpXList = WpXList.lock();

	if (tmpXList == nullptr)return;

	for (auto Mate : tmpXList->GetXFileMaterials(PType))
	{
		for (auto Boost : BoostPowList)
		{
			if (Mate->Name != Boost.first)continue;
			Mate->Mat.ScalingMode(Boost.second);
		}
	}

	ChLMat tmpMat = *_Mat;
	tmpMat = BaseMat * tmpMat;

	tmpXList->DrawSmpXFile(&tmpMat, PType);

/*
	for (int i = 0; i < tmpXList->GetXFileMaterials(PType).size(); i++)
	{
		if (tmpXList->GetXFileMaterials(PType)[i]->Name != "Boost"
			&& tmpXList->GetXFileMaterials(PType)[i]->Name != "Color")
		{
			ChDevice.AddBlendSetting();
		}
		else
		{

			ChDevice.AlphaBlendSetting();
		}
		
		tmpXList->DrawSmpXFileToSubSet(&tmpMat, PType, i);

	}
*/
	ChDevice.AlphaBlendSetting();
/*
	ChEffectDraw.SmpXfileNormalDraw(
		tmpXList->GetSmpXFile(PType)
		, &tmpMat
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
	ChPtr::Shared<ChSmpXList9> tmpXList = WpXList.lock();

	if (tmpXList == nullptr)return;

	std::string tmpString = "Normal.x";

	if (_PartsName.length() <= 0)
	{
		tmpString = XFilePath + tmpString;

		tmpXList->SetSmpXFile(
			tmpString
			, PType
		);

		BBox.SetBBoxToXFile(tmpXList->GetSmpXFile(PType));

		SetDitailsData(tmpString);

		SetBoost();

		return;
	}

	tmpString = XFilePath + _PartsName;

	tmpXList->SetSmpXFile(
		tmpString
		, PType
	);

	BBox.SetBBoxToXFile(tmpXList->GetSmpXFile(PType));

	SetDitailsData(tmpString);

	SetBoost();
}

void RPBoost::SetBoost()
{
	ChPtr::Shared<ChSmpXList9>tmpXList = WpXList.lock();

	if (tmpXList == nullptr)return;

	for (auto Mate : tmpXList->GetXFileMaterials(PType))
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
	auto tmpPartsList = WpRobParts.lock();

	if (tmpPartsList->GetBRobot() == nullptr)return;
	auto Rob = tmpPartsList->GetBRobot();

	if (WpXList.lock() == nullptr)return;
	auto tmpXList = WpXList.lock();

	ChVec3 tmpVec;
	float tmpMovePow;
	ChLMat tmpMat;


	tmpMovePow = Rob->GetMoveData().AvoV;
	
	tmpVec = Rob->GetMoveData().AvoDir;

	tmpMat = *Rob->GetMat();
	tmpMat.Inverse();

	tmpVec.MatNormal(&tmpMat, &tmpVec);

}

RPWeapon::RPWeapon()
{
	HP = 100;
	Weap = ChPtr::Make_U<ChGame::WeapData>();
	XFilePath = "Weapon/";
	PType = GetEnumCNum(RobPartsList::BodyPartsListName, MWeap);

	BaseMat.RotYPR(-90.0f, 0.0f, 0.0f);

	BaseMat = ChVec3(0.0f, -0.09f, 0.0f);
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

	ChPtr::Shared<ChSmpXList9> tmpXList = WpXList.lock();

	if (tmpXList == nullptr)return;

	std::string tmpString = "GunSample.x";

	if (_PartsName.length() <= 0)
	{
		tmpString = XFilePath + tmpString;

		tmpXList->SetSmpXFile(
			tmpString
			, PType
		);
		BBox.SetBBoxToXFile(tmpXList->GetSmpXFile(PType));

		SetDitailsData(tmpString);
		SetStandardParts();
		Weap->SEName = "SE_Shot.wav";

		return;
	}

	tmpString = XFilePath + _PartsName;

	tmpXList->SetSmpXFile(
		tmpString
		, PType
	);

	BBox.SetBBoxToXFile(tmpXList->GetSmpXFile(PType));
	SetStandardParts();
	SetDitailsData(tmpString);
	Weap->SEName = "SE_Shot2.wav";

	Weap->Damage = 10;
	Weap->HitLen = 2.0f;
	Weap->ShotPos.MatPos(&BaseMat, &ChVec3(0.0f, -10.0f, 0.0f));
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
	Weap->ShotPos.MatPos(&BaseMat, &ChVec3(0.0f, -10.0f, 0.0f));
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
		std::string tmpStr;
		tmpStr = tmpStr.substr(0, 3);
		std::string::iterator tmpIt;
		tmpIt = tmpStr.begin();
		for (unsigned short i = 0 ; tmpIt != tmpStr.end(); i++)
		{
			if (tmpStr[i] == ' ')
			{
				tmpStr.erase(tmpIt);
				continue;
			}

			i++;
			tmpIt++;
		}
	*/

	SetMWeapParts("tmpObj.x");

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
	unsigned short tmpNum = _RobotNum % 3;
/*
	std::string tmpStr;
	tmpStr = tmpStr.substr(0, 3);
	std::string::iterator tmpIt;
	tmpIt = tmpStr.begin();
	for (unsigned short i = 0 ; tmpIt != tmpStr.end(); i++)
	{
		if (tmpStr[i] == ' ')
		{
			tmpStr.erase(tmpIt);
			continue;
		}

		i++;
		tmpIt++;
	}
*/

	std::string tmpString = *TestDate[tmpNum];

	SetFootParts(tmpString);

	SetBodyParts(tmpString);

	SetHeadParts(tmpString);

	SetArmParts(tmpString);

	SetBoostParts(tmpString);

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
	ChStd::DataNo tmpNum = GetEnumCNum(BodyPartsListName, Foot);

	PartsList.insert(
		std::pair
		<ChStd::DataNo,ChPtr::Shared<RobParts>>
		(tmpNum,ChPtr::Make_S<RPFoot>()));

	PartsList[tmpNum]->SetXList(XList);
	PartsList[tmpNum]->Init(shared_from_this(), _PartsName);
	return 0.0f;
}

float RobPartsList::SetBodyParts(std::string _PartsName)
{
	ChStd::DataNo tmpNum = GetEnumCNum(BodyPartsListName, Body);

	PartsList.insert(
		std::pair
		<ChStd::DataNo, ChPtr::Shared<RobParts>>
		(tmpNum, ChPtr::Make_S<RPBody>()));

	PartsList[tmpNum]->SetXList(XList);
	PartsList[tmpNum]->Init(shared_from_this(), _PartsName);

	return 0.0f;
}

float RobPartsList::SetHeadParts(std::string _PartsName)
{
	ChStd::DataNo tmpNum = GetEnumCNum(BodyPartsListName, Head);

	auto tmpHead = ChPtr::Make_S<RPHead>(ChStd::False);

	PartsList.insert(
		std::pair
		<ChStd::DataNo, ChPtr::Shared<RobParts>>
		(tmpNum, tmpHead));

	PartsList[tmpNum]->SetXList(XList);
	PartsList[tmpNum]->Init(shared_from_this(), _PartsName);

	if (ChStd::False)WpHeadWeapon = tmpHead;
	return 0.0f;
}

void RobPartsList::SetArmParts(std::string _PartsName)
{
	ChStd::DataNo tmpNum = GetEnumCNum(BodyPartsListName, Arm);

	PartsList.insert(
		std::pair
		<ChStd::DataNo, ChPtr::Shared<RobParts>>
		(GetEnumCNum(BodyPartsListName, Arm), ChPtr::Make_S<RPArm>()));

	PartsList[tmpNum]->SetXList(XList);
	PartsList[tmpNum]->Init(shared_from_this(), _PartsName);

}

float RobPartsList::SetBoostParts(std::string _PartsName)
{
	ChStd::DataNo tmpNum = GetEnumCNum(BodyPartsListName, Boost);

	PartsList.insert(
		std::pair
		<ChStd::DataNo, ChPtr::Shared<RobParts>>
		(tmpNum, ChPtr::Make_S<RPBoost>()));
	PartsList[tmpNum]->SetXList(XList);
	PartsList[tmpNum]->Init(shared_from_this(), _PartsName);
	return 0.0f;
}

void RobPartsList::SetMWeapParts(std::string _PartsName)
{
	
	//if (PartsList.find(GetEnumCNum(BodyPartsListName, Arm)) == PartsList.end())return;
	//if (WpHeadWeapon.lock() != nullptr)return;

	ChStd::DataNo tmpNum = GetEnumCNum(BodyPartsListName, MWeap);

	ChPtr::Shared<RPWeapon>tmpWeap = ChPtr::Make_S<RPWeapon>();
	PartsList.insert(
		std::pair
		<ChStd::DataNo, ChPtr::Shared<RobParts>>
		(tmpNum, tmpWeap));

	PartsList[tmpNum]->SetXList(XList);
	PartsList[tmpNum]->Init(shared_from_this(), _PartsName);
	WpSWeapons = tmpWeap;
}

void RobPartsList::SetSWeapParts(std::string _PartsName)
{

	//if (WpHeadWeapon.lock() != nullptr)return;
	ChStd::DataNo tmpNum = GetEnumCNum(BodyPartsListName, SWeap);

	ChPtr::Shared<RPWeapon>tmpWeap = ChPtr::Make_S<RPWeapon>();

	PartsList.insert(
		std::pair
		<ChStd::DataNo, ChPtr::Shared<RobParts>>
		(tmpNum, tmpWeap));

	PartsList[tmpNum]->SetXList(XList);
	PartsList[tmpNum]->Init(shared_from_this(), _PartsName);
	WpSWeapons = tmpWeap;

}

ChStd::Bool RobPartsList::IsPosHit(
	const ChGame::APosData* _AData)
{
	if (ChPtr::NullCheck(_AData))return ChStd::False;
	ChPtr::Shared<BRobot> tmpRob = WpSetRobot.lock();
	if (tmpRob == nullptr)return ChStd::False;
	ChLMat tmpMat;
	ChB_Box9 tmpBBox;
	ChGame::APosData tmpData = *_AData;
	for (ChStd::DataNo i = 0; i < PartsCnt; i++)
	{
		if (PartsList.find(i) == PartsList.end())continue;
		if (PartsList[i]->IsBreak())continue;
		tmpBBox = *PartsList[i]->GetBBox();

		if (!tmpBBox.IsHitToBull(
			tmpRob->GetMat()
			, &tmpData.Pos
			, tmpData.Base.HitLen))continue;

		tmpRob->IsDamage(tmpData.Base.Damage);
		tmpRob->SetDData(&tmpData.Pos);
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