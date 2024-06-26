#include"../../BaseIncluder.h"
#include"StageData.h"


void StageData::LoadData(const std::string& _stageFilePath)
{
	stageDatas->Load(_stageFilePath);

	if (!stageDatas->stageImagePath.empty())
		stageImage.CreateTexture(stageDatas->stageImagePath);
}

void StageData::SaveData(const std::string& _stageFilePath)
{
	stageDatas->Save(_stageFilePath);
}
