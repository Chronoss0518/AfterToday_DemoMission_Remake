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
	std::wstring additionalCompensationExplanation = L"";
	//���ʌ�������//
	std::wstring specialPayReductionExplanation = L"";
};

struct StageDataStructure : public ChCpp::SendDataClass
{

	void Load(const std::wstring& _filePath);

	void Save(const std::wstring& _filePath);

	ChPtr::Shared<ResultStructure> resultData = ChPtr::Make_S<ResultStructure>();

	//�͂��߂ɒ񎦂���鐬����V//
	unsigned long drawSuccessFee = 0;

	//Stage��ScriptPath//
	std::wstring stageScriptPath = L"";

	//Stage�̍�펞��(s)//
	unsigned long missionTimeSeccond = 0;
	
	//Stage�̖���//
	std::wstring stageName = L"";

	//Stage��ImagePath//
	std::wstring stageImagePath = L"";

	//Stage�̒n��//
	std::wstring stageLocationName = L"";

	//Stage�̓��e//
	std::wstring stageStrategyOverview = L"";

	//Mecha�����[�h����ѕҏW���ł��邽�߂̃t���O//
	bool selectModelFlg = true;
};