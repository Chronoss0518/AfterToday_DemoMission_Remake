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
	std::string additionalCompensationExplanation = "";
	//特別減給説明//
	std::string specialPayReductionExplanation = "";
};

struct StageDataStructure : public ChCpp::SendDataClass
{

	void Load(const std::string& _filePath);

	void Save(const std::string& _filePath);

	ChPtr::Shared<ResultStructure> resultData = ChPtr::Make_S<ResultStructure>();

	//はじめに提示される成功報酬//
	unsigned long drawSuccessFee = 0;

	//StageのScriptPath//
	std::string stageScriptPath = "";

	//Stageの作戦時間(s)//
	unsigned long missionTimeSeccond = 0;
	
	//Stageの名称//
	std::string stageName = "";

	//StageのImagePath//
	std::string stageImagePath = "";

	//Stageの地名//
	std::string stageLocationName = "";

	//Stageの内容//
	std::string stageStrategyOverview = "";

	//Mechaをロードおよび編集ができるためのフラグ//
	bool selectModelFlg = true;
};