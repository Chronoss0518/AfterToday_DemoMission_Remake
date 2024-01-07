#include"../BaseIncluder.h"
#include"../AllStruct.h"

#include"StageDataStructure.h"

#define J_STAGE_NAME "StageName"
#define J_STAGE_SCRIPT_PATH "StageScriptPath"
#define J_STAGE_LOCATIN_NAME "StageLoacationName"
#define J_STAGE_IMAGE_PATH "StageImager"
#define J_SUCCESS_FEE "SuccessFee"
#define J_DRAW_SUCCESS_FEE "DrawSuccessFee"
#define J_ADDITIONAL_COMPENSATION "AdditionalCompensation"
#define J_ADDITIONAL_COMPENSATION_EXPLANATION "AdditionalCompensationExplanation"
#define J_SPECIAL_PAY_REDUCTION "SpecialPayReduction"
#define J_SPECIAL_PAY_REDUCTION_EXPLANATION "SpecialPayReductionExplanation"

void StageDataStructure::Load(const std::string& _filePath)
{

	std::string text = "";

	{
		ChCpp::CharFile file;
		file.FileOpen(_filePath);
		text = file.FileReadText();
		file.FileClose();
	}

	auto jsonObject = ChPtr::SharedSafeCast<ChCpp::JsonObject>(ChCpp::JsonBaseType::GetParameter(text));

	if (jsonObject == nullptr)return;

	auto stageNameJson = jsonObject->GetJsonString(J_STAGE_NAME);
	if (stageNameJson != nullptr)stageName = *stageNameJson;

	auto stageImageJson = jsonObject->GetJsonString(J_STAGE_IMAGE_PATH);
	if (stageImageJson != nullptr)stageImagePath = *stageImageJson;

	auto stageLocationNameJson = jsonObject->GetJsonString(J_STAGE_LOCATIN_NAME);
	if (stageLocationNameJson != nullptr)stageLocationName = *stageLocationNameJson;

	auto stageScriptPathJson = jsonObject->GetJsonString(J_STAGE_SCRIPT_PATH);
	if (stageScriptPathJson != nullptr)stageScriptPath = *stageScriptPathJson;

	auto successFeeJson = jsonObject->GetJsonNumber(J_SUCCESS_FEE);
	if (successFeeJson != nullptr)resultData->successFee = *successFeeJson;

	auto drawSuccessFeeJson = jsonObject->GetJsonNumber(J_DRAW_SUCCESS_FEE);
	if (drawSuccessFeeJson != nullptr)drawSuccessFee = *drawSuccessFeeJson;
	else drawSuccessFee = resultData->successFee;

	auto additionalCompensationJson = jsonObject->GetJsonNumber(J_ADDITIONAL_COMPENSATION);
	if (additionalCompensationJson != nullptr)resultData->additionalCompensation = *additionalCompensationJson;

	auto additionalCompensationExplanationJson = jsonObject->GetJsonString(J_ADDITIONAL_COMPENSATION_EXPLANATION);
	if (additionalCompensationExplanationJson != nullptr)resultData->additionalCompensationExplanation = *additionalCompensationExplanationJson;

	auto specialPayReductionJson = jsonObject->GetJsonNumber(J_SPECIAL_PAY_REDUCTION);
	if (specialPayReductionJson != nullptr)resultData->specialPayReduction = *specialPayReductionJson;

	auto specialPayReductionExplanationJson = jsonObject->GetJsonString(J_SPECIAL_PAY_REDUCTION_EXPLANATION);
	if (specialPayReductionExplanationJson != nullptr)resultData->specialPayReductionExplanation = *specialPayReductionExplanationJson;

}

void StageDataStructure::Save(const std::string& _filePath)
{

	auto res = ChPtr::Make_S<ChCpp::JsonObject>();

	res->SetObject(J_STAGE_NAME, ChCpp::JsonString::CreateObject(stageName));

	res->SetObject(J_STAGE_IMAGE_PATH, ChCpp::JsonString::CreateObject(stageImagePath));

	res->SetObject(J_STAGE_LOCATIN_NAME, ChCpp::JsonString::CreateObject(stageLocationName));

	res->SetObject(J_STAGE_SCRIPT_PATH, ChCpp::JsonString::CreateObject(stageScriptPath));

	res->SetObject(J_SUCCESS_FEE, ChCpp::JsonNumber::CreateObject(resultData->successFee));

	if(resultData->successFee != drawSuccessFee) res->SetObject(J_DRAW_SUCCESS_FEE, ChCpp::JsonNumber::CreateObject(drawSuccessFee));

	res->SetObject(J_ADDITIONAL_COMPENSATION, ChCpp::JsonNumber::CreateObject(resultData->additionalCompensation));
	
	res->SetObject(J_ADDITIONAL_COMPENSATION_EXPLANATION, ChCpp::JsonString::CreateObject(resultData->additionalCompensationExplanation));
	
	res->SetObject(J_SPECIAL_PAY_REDUCTION, ChCpp::JsonNumber::CreateObject(resultData->specialPayReduction));
	
	res->SetObject(J_SPECIAL_PAY_REDUCTION_EXPLANATION, ChCpp::JsonString::CreateObject(resultData->specialPayReductionExplanation));

	{
		ChCpp::CharFile file;
		file.FileOpen(_filePath);
		auto result = file.FileWriteText(res->GetRawData());
		file.FileClose();
	}

}