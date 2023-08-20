#pragma once

class MechaParts;
class MechaPartsObject;

#include"../BaseMecha/BaseMecha.h"

class EditFrame :public ChCpp::BaseFrame
{
public:

	void Init()override;

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

	void LoadFile(const std::string& _fileName);

	void SaveFile();

private:

	void SetScript();

	void DrawFunction();

	void UpdateFunction();

	ChWin::SubWind leftPanels,rightPanels;
	ChWin::Texture leftPanelsTitle, rightPanelsTitle;



	ChPtr::Shared<BaseMecha> CreateMecha();

	ChD3D11::Shader::BaseDrawSprite11 spriteShader;

	ChPtr::Shared<BaseMecha>editMecha = nullptr;
	MeshDrawer meshDrawer;

	std::string fileName = "";

};
