#include"../BaseIncluder.h"
#include"../AllStruct.h"

#include"StageDataStructure.h"

#define J_STAGE_NAME L"StageName"
#define J_STAGE_SCRIPT_PATH L"StageScriptPath"
#define J_STAGE_LOCATIN_NAME L"StageLocationName"
#define J_STAGE_IMAGE_PATH L"StageImager"
#define J_STAGE_STRATEGY_OVERVIEW L"StageStrategyOverview"
#define J_STAGE_MISSION_TIME L"MissionTimeSeccond"
#define J_SUCCESS_FEE L"SuccessFee"
#define J_DRAW_SUCCESS_FEE L"DrawSuccessFee"
#define J_ADDITIONAL_COMPENSATION L"AdditionalCompensation"
#define J_ADDITIONAL_COMPENSATION_EXPLANATION L"AdditionalCompensationExplanation"
#define J_SPECIAL_PAY_REDUCTION L"SpecialPayReduction"
#define J_SPECIAL_PAY_REDUCTION_EXPLANATION L"SpecialPayReductionExplanation"
#define J_SPECIAL_PAY_REDUCTION_EXPLANATION L"SpecialPayReductionExplanation"
#define J_SELECT_MODEL_FLG L"SelectModelFlg"

void StageDataStructure::Load(const std::wstring& _filePath)
{

	std::wstring text = L"";

	{
		ChCpp::WCharFile file;
		file.SetLocaleName("Japanese");
		file.FileOpen(_filePath);
		text = file.FileReadText();
		file.FileClose();
	}

	auto jsonObject = ChPtr::SharedSafeCast<ChCpp::JsonObject<wchar_t>>(ChCpp::JsonBaseType<wchar_t>::GetParameter(text));

	if (jsonObject == nullptr)return;

	auto stageNameJson = jsonObject->GetJsonString(J_STAGE_NAME);
	if (stageNameJson != nullptr)stageName = stageNameJson->GetString();

	auto stageImageJson = jsonObject->GetJsonString(J_STAGE_IMAGE_PATH);
	if (stageImageJson != nullptr)stageImagePath = stageImageJson->GetString();

	auto stageLocationNameJson = jsonObject->GetJsonString(J_STAGE_LOCATIN_NAME);
	if (stageLocationNameJson != nullptr)stageLocationName = stageLocationNameJson->GetString();

	auto stageScriptPathJson = jsonObject->GetJsonString(J_STAGE_SCRIPT_PATH);
	if (stageScriptPathJson != nullptr)stageScriptPath = stageScriptPathJson->GetString();

	auto stageStrategyOverviewJson = jsonObject->GetJsonString(J_STAGE_STRATEGY_OVERVIEW);
	if (stageStrategyOverviewJson != nullptr)stageStrategyOverview = stageStrategyOverviewJson->GetString();

	auto missionTimeSeccondJson = jsonObject->GetJsonNumber(J_STAGE_MISSION_TIME);
	if (missionTimeSeccondJson != nullptr)missionTimeSeccond = *missionTimeSeccondJson;

	auto successFeeJson = jsonObject->GetJsonNumber(J_SUCCESS_FEE);
	if (successFeeJson != nullptr)resultData->successFee = *successFeeJson;

	auto drawSuccessFeeJson = jsonObject->GetJsonNumber(J_DRAW_SUCCESS_FEE);
	if (drawSuccessFeeJson != nullptr)drawSuccessFee = *drawSuccessFeeJson;
	else drawSuccessFee = resultData->successFee;

	auto additionalCompensationJson = jsonObject->GetJsonNumber(J_ADDITIONAL_COMPENSATION);
	if (additionalCompensationJson != nullptr)resultData->additionalCompensation = *additionalCompensationJson;

	auto additionalCompensationExplanationJson = jsonObject->GetJsonString(J_ADDITIONAL_COMPENSATION_EXPLANATION);
	if (additionalCompensationExplanationJson != nullptr)resultData->additionalCompensationExplanation = additionalCompensationExplanationJson->GetString();

	auto specialPayReductionJson = jsonObject->GetJsonNumber(J_SPECIAL_PAY_REDUCTION);
	if (specialPayReductionJson != nullptr)resultData->specialPayReduction = *specialPayReductionJson;

	auto specialPayReductionExplanationJson = jsonObject->GetJsonString(J_SPECIAL_PAY_REDUCTION_EXPLANATION);
	if (specialPayReductionExplanationJson != nullptr)resultData->specialPayReductionExplanation = specialPayReductionExplanationJson->GetString();

	auto selectModelFlgJson = jsonObject->GetJsonBoolean(J_SELECT_MODEL_FLG);
	if (selectModelFlgJson != nullptr)selectModelFlg = *selectModelFlgJson;

}

void StageDataStructure::Save(const std::wstring& _filePath)
{

	auto res = ChPtr::Make_S<ChCpp::JsonObject<wchar_t>>();

	res->Set(J_STAGE_NAME, ChCpp::JsonString<wchar_t>::CreateObject(stageName));

	res->Set(J_STAGE_IMAGE_PATH, ChCpp::JsonString<wchar_t>::CreateObject(stageImagePath));

	res->Set(J_STAGE_LOCATIN_NAME, ChCpp::JsonString<wchar_t>::CreateObject(stageLocationName));

	res->Set(J_STAGE_SCRIPT_PATH, ChCpp::JsonString<wchar_t>::CreateObject(stageScriptPath));

	res->Set(J_STAGE_STRATEGY_OVERVIEW, ChCpp::JsonString<wchar_t>::CreateObject(stageStrategyOverview));

	res->Set(J_STAGE_MISSION_TIME, ChCpp::JsonNumber<wchar_t>::CreateObject(missionTimeSeccond));

	res->Set(J_SUCCESS_FEE, ChCpp::JsonNumber<wchar_t>::CreateObject(resultData->successFee));

	if(resultData->successFee != drawSuccessFee) res->Set(J_DRAW_SUCCESS_FEE, ChCpp::JsonNumber<wchar_t>::CreateObject(drawSuccessFee));

	res->Set(J_ADDITIONAL_COMPENSATION, ChCpp::JsonNumber<wchar_t>::CreateObject(resultData->additionalCompensation));
	
	res->Set(J_ADDITIONAL_COMPENSATION_EXPLANATION, ChCpp::JsonString<wchar_t>::CreateObject(resultData->additionalCompensationExplanation));
	
	res->Set(J_SPECIAL_PAY_REDUCTION, ChCpp::JsonNumber<wchar_t>::CreateObject(resultData->specialPayReduction));
	
	res->Set(J_SPECIAL_PAY_REDUCTION_EXPLANATION, ChCpp::JsonString<wchar_t>::CreateObject(resultData->specialPayReductionExplanation));

	res->Set(J_SELECT_MODEL_FLG, ChCpp::JsonBoolean<wchar_t>::CreateObject(selectModelFlg));

	{
		ChCpp::WCharFile file;
		file.SetLocaleName("Japanese");
		file.FileOpen(_filePath);
		auto result = file.FileWriteText(ChCpp::JsonBaseType<wchar_t>::FormatDocument(res->GetRawData()));
		file.FileClose();
	}

}
