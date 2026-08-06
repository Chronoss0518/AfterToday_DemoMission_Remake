#pragma once

#include"../Frames/EditFrame.h"

class SelectPartsList;
class SelectPartsListItem;

class PartsChangeDisplay : public EditFrame::EditFrameDisplayBase
{
public:

	void Init(EditFrame* _frame)override;

	void Update(MenuBase::ActionType _type)override;

	void UpdateMouse()override;

	void Draw(ChD3D11::Shader::BaseDrawSprite11& _spriteShader)override;

public:

	void InitChangeData(ChPtr::Shared<MechaPartsObject>_targetParts);

	void InitChangeData(ChPtr::Shared<MechaPartsObject>_parentParts,const std::wstring& _partsPos);

	void Close();

	void ResetBeforeParts();

public:

	inline ChPtr::Shared<MechaPartsObject> GetResultParts() { return resultParts; }

public:

	void AddMechaParts(ChPtr::Shared<MechaPartsObject> _parts);

private:

	void UpdateCoreParts();

	void UpdateNotCoreParts();

private:

	ChD3D11::Texture11 backGroundTexture;
	ChD3D11::Sprite11 backgroundSprite;

	ChPtr::Shared<SelectPartsList>selectPartsList = nullptr;

	std::vector<ChPtr::Shared<SelectPartsListItem>>partsPanelList;
	ChPtr::Shared<SelectPartsListItem>cancelPanel = nullptr;

	//変更後のパーツ//
	ChPtr::Shared<MechaPartsObject>nowChangeTargetPartsParent = nullptr;
	std::wstring nowChangeTargetPartsPosName = L"";

	ChPtr::Shared<MechaPartsObject> resultParts = nullptr;

	ChPtr::Shared<ChCpp::JsonObject<wchar_t>>beforePartsJson = nullptr;
};