#include"../../BaseIncluder.h"
#include"StageData.h"


void StageData::LoadData(const std::string& _stageFilePath)
{
	std::string text = "";

	{
		ChCpp::CharFile file;
		file.FileOpen(_stageFilePath);
		text = file.FileReadText();
		file.FileClose();
	}

	auto jsonObject = ChPtr::SharedSafeCast<ChCpp::JsonObject>(ChCpp::JsonBaseType::GetParameter(text));
	
	if (jsonObject == nullptr)return;

	auto stagePathJson = jsonObject->GetJsonString("StagePath");
	if (stagePathJson != nullptr)stageDatas->stageName = stagePathJson->GetString();

	auto stageNameJson = jsonObject->GetJsonString("StageName");
	if (stageNameJson != nullptr)stageName = ChStr::UTF8ToWString(stageNameJson->GetString());

	auto stageImageJson = jsonObject->GetJsonString("StageImage");
	if (stageImageJson != nullptr)
	{
		stageImage.path = stagePathJson->GetString();
		stageImage.image.CreateTexture(stageImage.path);
	}

	auto targetNameJson = jsonObject->GetJsonString("TargetName");
	if (targetNameJson != nullptr)targetName = ChStr::UTF8ToWString(targetNameJson->GetString());

	auto targetImageList = jsonObject->GetJsonArray("TargetImags");
	if (targetImageList != nullptr)
	{
		for (unsigned long i = 0; i < targetImageList->GetCount(); i++)
		{
			auto targetImageJson = targetImageList->GetJsonString(i);
			if (targetImageJson == nullptr)continue;
			targetImages[i] = ChPtr::Make_S<UseImageData>();
			targetImages[i]->path = targetImageJson->GetString();
			targetImages[i]->image.CreateTexture(targetImages[i]->path);
		}
	}
}

void StageData::SaveData(const std::string& _stageFilePath)
{
	auto res = ChPtr::Make_S<ChCpp::JsonObject>();

	res->SetObject("StageScriptPath", ChCpp::JsonString::CreateObject(stageDatas->stageScriptPath));
	res->SetObject("SuccessFee", ChCpp::JsonNumber::CreateObject(stageDatas->resultData->successFee));
	res->SetObject("AdditionalCompensation", ChCpp::JsonNumber::CreateObject(stageDatas->resultData->additionalCompensation));
	res->SetObject("AdditionalCompensationExplanation", ChCpp::JsonString::CreateObject(stageDatas->resultData->additionalCompensationExplanation));
	res->SetObject("SpecialPayReduction", ChCpp::JsonNumber::CreateObject(stageDatas->resultData->specialPayReduction));
	res->SetObject("SpecialPayReductionExplanation", ChCpp::JsonString::CreateObject(stageDatas->resultData->specialPayReductionExplanation));

	res->SetObject("StageName", ChCpp::JsonString::CreateObject(ChStr::UTF8ToString(stageName)));

	res->SetObject("StageImage", ChCpp::JsonString::CreateObject(stageImage.path));

	auto targetImageList = ChPtr::Make_S<ChCpp::JsonArray>();

	for (unsigned char i = 0; i < 10; i++)
	{
		if (targetImages[i] == nullptr)continue;

		targetImageList->AddObject(ChCpp::JsonString::CreateObject(targetImages[i]->path));

	}

	res->SetObject("TargetImages", targetImageList);

	res->SetObject("TargetName", ChCpp::JsonString::CreateObject(ChStr::UTF8ToString(targetName)));


	{
		ChCpp::CharFile file;
		file.FileOpen(_stageFilePath);
		auto result = file.FileWriteText(res->GetRawData());
		file.FileClose();
	}

}
