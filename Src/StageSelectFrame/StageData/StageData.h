#pragma once

#include"../../Frames/StageDataStructure.h"

class StageData
{
public:

	void LoadData(const std::wstring& _stageFilePath);

public:

	void SaveData(const std::wstring& _stageFilePath);

public:

	ChPtr::Shared<StageDataStructure>stageDatas = ChPtr::Make_S<StageDataStructure>();

	ChD3D11::Texture11 selectPanel;

	ChD3D11::Texture11 description;

	ChD3D11::Texture11 stageImage;
	
};

