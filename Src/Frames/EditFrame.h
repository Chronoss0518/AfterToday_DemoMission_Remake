#pragma once

class MechaParts;
class MechaPartsObject;

#include"../BaseMecha/BaseMecha.h"

class PanelList;

class EditFrame :public ChCpp::BaseFrame,public ChCp::Initializer
{
private:

	struct MouseData
	{
		bool flg;
		ChVec2 position;
	};

public:

	void Init(ChPtr::Shared<ChCpp::SendDataClass> _sendData)override;

	void Release()override;

	void Update()override;

public:

	inline void SetFileName(const std::string& _fileName)
	{
		fileName = _fileName;
	}

public:

	void AddPartsObject(BaseMecha::PartsPosNames _name, ChPtr::Shared<MechaPartsObject> _createParts);

	void ChangePartsObejct(BaseMecha::PartsPosNames _name, ChPtr::Shared<MechaPartsObject> _createParts);

	void RemovePartsObejct(BaseMecha::PartsPosNames _name, ChPtr::Shared<MechaPartsObject> _createParts);


public:

	void CreateNewFile();

	void CreateLoadFileList();

	void LoadFile(const std::string& _fileName);

	void SaveFile();

private:

	void DrawFunction();

	void UpdateFunction();

	ChD3D11::Texture11 loadTitlePanel, partsTitlePanel;
	ChD3D11::Texture11 partsTitlePanelList[ChStd::EnumCast(BaseMecha::PartsPosNames::None)];

	ChD3D11::Texture11 *addButtonTexture = nullptr;

	ChD3D11::Texture11 addMecha, addParts[ChStd::EnumCast(BaseMecha::PartsPosNames::None)];
	
	ChD3D11::Sprite11 panelSprite;
	ChVec2 panelSize = ChVec2();
	float padding = 0.0f;

	ChPtr::Shared<PanelList>leftPanel = nullptr, rightPanel = nullptr;
	ChD3D11::Texture11 panelBackGround;
	
	ChPtr::Shared<BaseMecha> CreateMecha();

	ChD3D11::Shader::BaseDrawSprite11 spriteShader;

	ChPtr::Shared<BaseMecha>editMecha = nullptr;
	MeshDrawer meshDrawer;
	ChD3D::DirectFontFromDXGISurface drawTexter;

	std::string fileName = "";
	std::vector<std::filesystem::path> fileList;

	MouseData downData, upData;
};
