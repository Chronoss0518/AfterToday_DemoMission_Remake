#pragma once

struct ResultStructure :public ChCpp::SendDataClass
{
	//¬Œ÷•ñV//
	unsigned long successFee = 0;
	//’Ç‰Á•ñV//
	unsigned long additionalCompensation = 0;
	//“Á•ÊŒ¸‹‹//
	unsigned long specialPayReduction = 0;
	//C‘U”ï//
	unsigned long repairCosts = 0;
	//’e–ò”ï//
	unsigned long ammunitionExpense = 0;
	//‹Ÿ‹‹”ï//
	unsigned long energyCosts = 0;

	//’Ç‰Á•ñVà–¾//
	std::wstring additionalCompensationExplanation = L"";
	//“Á•ÊŒ¸‹‹à–¾//
	std::wstring specialPayReductionExplanation = L"";
};

struct StageDataStructure : public ChCpp::SendDataClass
{

	void Load(const std::wstring& _filePath);

	void Save(const std::wstring& _filePath);

	ChPtr::Shared<ResultStructure> resultData = ChPtr::Make_S<ResultStructure>();

	//‚Í‚¶‚ß‚É’ñ¦‚³‚ê‚é¬Œ÷•ñV//
	unsigned long drawSuccessFee = 0;

	//Stage‚ÌScriptPath//
	std::wstring stageScriptPath = L"";

	//Stage‚ÌìíŠÔ(s)//
	unsigned long missionTimeSeccond = 0;
	
	//Stage‚Ì–¼Ì//
	std::wstring stageName = L"";

	//Stage‚ÌImagePath//
	std::wstring stageImagePath = L"";

	//Stage‚Ì’n–¼//
	std::wstring stageLocationName = L"";

	//Stage‚Ì“à—e//
	std::wstring stageStrategyOverview = L"";

	//Mecha‚ğƒ[ƒh‚¨‚æ‚Ñ•ÒW‚ª‚Å‚«‚é‚½‚ß‚Ìƒtƒ‰ƒO//
	bool selectModelFlg = true;
};