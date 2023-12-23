#pragma once

#include"../../Frames/StageDataStructure.h"

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

	ChPtr::Shared<StageDataStructure>stageDatas = ChPtr::Make_S<StageDataStructure>();

	std::wstring stageName = L"";
	ChPtr::Shared<ChD3D11::Texture11>stageNameText = nullptr;

	UseImageData stageImage;

	std::wstring targetName = L""; 
	ChPtr::Shared<ChD3D11::Texture11>targetNameText = nullptr;

	ChPtr::Shared<UseImageData> targetImages[10];

	
};

