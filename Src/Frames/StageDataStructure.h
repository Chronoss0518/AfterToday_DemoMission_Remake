#pragma once

struct ResultStructure :public ChCpp::SendDataClass
{
	//������V//
	unsigned long successFee = 0;
	//�ǉ���V//
	unsigned long additionalCompensation = 0;
	//���ʌ���//
	unsigned long specialPayReduction = 0;
	//�C�U��//
	unsigned long repairCosts = 0;
	//�e���//
	unsigned long ammunitionExpense = 0;
	//������//
	unsigned long energyCosts = 0;

	//�ǉ���V����//
	std::string additionalCompensationExplanation = "";
	//���ʌ�������//
	std::string specialPayReductionExplanation = "";
};

struct StageDataStructure : public ChCpp::SendDataClass
{

	void Load(const std::string& _filePath);

	void Save(const std::string& _filePath);

	ChPtr::Shared<ResultStructure> resultData = ChPtr::Make_S<ResultStructure>();

	//�͂��߂ɒ񎦂���鐬����V//
	unsigned long drawSuccessFee = 0;

	//Stage��ScriptPath//
	std::string stageScriptPath = "";

	//Stage�̍�펞��(s)//
	unsigned long missionTimeSeccond = 0;
	
	//Stage�̖���//
	std::string stageName = "";

	//Stage��ImagePath//
	std::string stageImagePath = "";

	//Stage�̒n��//
	std::string stageLocationName = "";

};