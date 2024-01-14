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
	std::string additionalCompensationExplanation = "";
	//“Á•ÊŒ¸‹‹à–¾//
	std::string specialPayReductionExplanation = "";
};

struct StageDataStructure : public ChCpp::SendDataClass
{

	void Load(const std::string& _filePath);

	void Save(const std::string& _filePath);

	ChPtr::Shared<ResultStructure> resultData = ChPtr::Make_S<ResultStructure>();

	//‚Í‚¶‚ß‚É’ñ¦‚³‚ê‚é¬Œ÷•ñV//
	unsigned long drawSuccessFee = 0;

	//Stage‚ÌScriptPath//
	std::string stageScriptPath = "";

	//Stage‚ÌìíŠÔ(s)//
	unsigned long missionTimeSeccond = 0;
	
	//Stage‚Ì–¼Ì//
	std::string stageName = "";

	//Stage‚ÌImagePath//
	std::string stageImagePath = "";

	//Stage‚Ì’n–¼//
	std::string stageLocationName = "";

};