#pragma once

struct ResultStructure :public ChCpp::SendDataClass
{
	//������V//
	unsigned long successFee;
	//�ǉ���V//
	unsigned long additionalCompensation;
	//���ʌ���//
	unsigned long specialPayReduction;
	//�C�U��//
	unsigned long repairCosts;
	//�e���//
	unsigned long ammunitionExpense;
	//������//
	unsigned long energyCosts;

	//�ǉ���V����//
	std::string additionalCompensationExplanation;
	//���ʌ�������//
	std::string specialPayReductionExplanation;
};

struct StageDataStructure : public ChCpp::SendDataClass
{
	ChPtr::Shared<ResultStructure> resultData = ChPtr::Make_S<ResultStructure>();

	//StageScriptPath//
	std::string stageScriptPath = "";
};