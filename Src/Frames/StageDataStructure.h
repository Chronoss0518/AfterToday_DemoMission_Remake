#pragma once

struct ResultStructure :public ChCpp::SendDataClass
{

	unsigned long successFee;
	unsigned long additionalCompensation;
	unsigned long specialPayReduction;
	unsigned long repairCosts;
	unsigned long ammunitionExpense;
	unsigned long energyCosts;

	std::string additionalCompensationExplanation;
	std::string specialPayReductionExplanation;

};

struct StageDataStructure : public ChCpp::SendDataClass
{
	ChPtr::Shared<ResultStructure> resultData = ChPtr::Make_S<ResultStructure>();

	std::string stageName = "";
};