#pragma once

class StageData
{
	struct UseImageData
	{
		ChD3D11::Texture11 image;
		std::string path = "";
	};

public:

	void LoadData(const std::string& _stageFilePath);

public:

	void SaveData(const std::string& _stageFilePath);

public:

	std::string stagePath = "";
	std::wstring stageName = L"";
	UseImageData stageImage;

	std::wstring targetName = L"";
	ChPtr::Shared<UseImageData> targetImages[10];
	
};

