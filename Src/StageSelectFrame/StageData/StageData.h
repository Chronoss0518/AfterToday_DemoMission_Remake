#pragma once

#include"../../Frames/StageDataStructure.h"

class StageData
{
public:

	void LoadData(const std::string& _stageFilePath);

public:

	void SaveData(const std::string& _stageFilePath);

public:

	ChPtr::Shared<StageDataStructure>stageDatas = ChPtr::Make_S<StageDataStructure>();

	ChPtr::Shared<ChD3D11::RenderTarget11>selectPanel = nullptr;

	ChPtr::Shared<ChD3D11::Texture11>description = nullptr;

	ChD3D11::Texture11 stageImage;
	
};

