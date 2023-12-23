#pragma once

struct ResultStructure :public ChCpp::SendDataClass
{
	//¬Œ÷•ñV//
	unsigned long successFee;
	//’Ç‰Á•ñV//
	unsigned long additionalCompensation;
	//“Á•ÊŒ¸‹‹//
	unsigned long specialPayReduction;
	//C‘U”ï//
	unsigned long repairCosts;
	//’e–ò”ï//
	unsigned long ammunitionExpense;
	//‹Ÿ‹‹”ï//
	unsigned long energyCosts;

	//’Ç‰Á•ñVà–¾//
	std::string additionalCompensationExplanation;
	//“Á•ÊŒ¸‹‹à–¾//
	std::string specialPayReductionExplanation;
};

struct StageDataStructure : public ChCpp::SendDataClass
{
	ChPtr::Shared<ResultStructure> resultData = ChPtr::Make_S<ResultStructure>();

	//StageScriptPath//
	std::string stageScriptPath = "";
};