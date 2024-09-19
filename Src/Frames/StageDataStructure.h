#pragma once

struct ResultStructure :public ChCpp::SendDataClass
{
	//成功報酬//
	unsigned long successFee = 0;
	//追加報酬//
	unsigned long additionalCompensation = 0;
	//特別減給//
	unsigned long specialPayReduction = 0;
	//修繕費//
	unsigned long repairCosts = 0;
	//弾薬費//
	unsigned long ammunitionExpense = 0;
	//供給費//
	unsigned long energyCosts = 0;

	//追加報酬説明//
	std::wstring additionalCompensationExplanation = L"";
	//特別減給説明//
	std::wstring specialPayReductionExplanation = L"";
};

struct StageDataStructure : public ChCpp::SendDataClass
{

	void Load(const std::wstring& _filePath);

	void Save(const std::wstring& _filePath);

	ChPtr::Shared<ResultStructure> resultData = ChPtr::Make_S<ResultStructure>();

	//はじめに提示される成功報酬//
	unsigned long drawSuccessFee = 0;

	//StageのScriptPath//
	std::wstring stageScriptPath = L"";

	//Stageの作戦時間(s)//
	unsigned long missionTimeSeccond = 0;
	
	//Stageの名称//
	std::wstring stageName = L"";

	//StageのImagePath//
	std::wstring stageImagePath = L"";

	//Stageの地名//
	std::wstring stageLocationName = L"";

	//Stageの内容//
	std::wstring stageStrategyOverview = L"";

	//Mechaをロードおよび編集ができるためのフラグ//
	bool selectModelFlg = true;
};