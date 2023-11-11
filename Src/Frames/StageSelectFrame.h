#pragma once

class StageData;

class StageSelectFrame :public ChCpp::BaseFrame
{
public:

	void Init()override;

	void Release()override;

	void Update()override;

private:

	void SetScript();

	void DrawFunction();

	void UpdateFunction();

	void UpdateMouse();

	void UpdateKeyboard();

	void UpdateController();

	enum class ActionType
	{
		UpSelect,
		DownSelect,
		Decision
	};

	std::vector<ActionType> inputDataList;

	std::vector<ChPtr::Shared<StageData>>stageDataList;

	ChD3D11::Sprite11 stagePreImageSprite[5];

	ChD3D11::Shader::BaseDrawSprite11 spriteShader;
	ChD3D::DirectFont textDrawer;

	int nowSelect = 0;


	ChD3D11::Sprite11 stageImageSprite;

	ChD3D11::Sprite11 targetImageSprite[10];

};
