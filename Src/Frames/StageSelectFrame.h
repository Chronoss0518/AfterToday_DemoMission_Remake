#pragma once

#include"FromStageSelectFrameData.h"

#ifndef STAGE_IMAGE_TEXTURE_DIRECTORY
#define STAGE_IMAGE_TEXTURE_DIRECTORY(current_path) TEXTURE_DIRECTORY("StageImage/") current_path
#endif

#ifndef STAGE_SELECT_TEXTURE_DIRECTORY
#define STAGE_SELECT_TEXTURE_DIRECTORY(current_path) TEXTURE_DIRECTORY("StageSelect/") current_path
#endif

#ifndef STAGE_SELECT_PANEL_WIDTH
#define STAGE_SELECT_PANEL_WIDTH 350.0f
#endif

#ifndef STAGE_SELECT_PANEL_HEIGHT
#define STAGE_SELECT_PANEL_HEIGHT 102.0f
#endif


#ifndef STAGE_SELECT_DESCRIPTION_WIDTH
#define STAGE_SELECT_DESCRIPTION_WIDTH 795.0f
#endif

#ifndef STAGE_SELECT_DESCRIPTION_HEIGHT
#define STAGE_SELECT_DESCRIPTION_HEIGHT 166.0f
#endif


class StageData;

class StageSelectFrame :public ChCpp::BaseFrame
{
public:

	enum class DisplayType
	{
		Select,//‘I‘ð‰æ–Ê//
		Detailed//Ú×‰æ–Ê//
	};

	enum class ActionType
	{
		UpSelect,
		DownSelect,
		Decision,
		Cancel
	};

	enum class SelectSpriteType
	{
		UpButton,
		DownButton,
		StagePanel
	};

	enum class StageSelectButtonType
	{
		Up, Down, None
	};

public:

	class StageSelectFrameDisplay
	{
	public:

		virtual void Init() = 0;

		virtual void Release() = 0;

		virtual void Update() = 0;

		virtual void UpdateAction(const ActionType& _action) = 0;

		virtual void Draw(ChD3D11::Shader::BaseDrawSprite11& _drawer) = 0;

		virtual void OnDisplay(){}

		virtual void SetSelectData(const FromStageSelectFrameData& _selectData) = 0;

		virtual void GetSelectData(FromStageSelectFrameData& _selectData) = 0;

	public:

		inline void SetStageSelectFrame(StageSelectFrame* _frame)
		{
			if (ChPtr::NullCheck(_frame))return;
			frame = _frame;
		}

		inline void SetDisplayType(DisplayType _type)
		{
			frame->type = _type;
		}

		inline void SetNowStageSelect(int _nowSelectStage)
		{
			frame->nowSelectStage = _nowSelectStage;
		}

		inline void SetGameFrame()
		{
			frame->SetGameFrame();
		}

		inline void SetEditFrame()
		{
			frame->SetEditFrame();
		}


	public:

		void AddAction(ActionType _type)
		{
			frame->inputDataList.push_back(_type);
		}

	protected:

		inline int GetNowSelectCount()
		{
			return frame->nowSelectStage;
		}

		inline StageData& GetNowSelectStageData()
		{
			return *frame->stageDataList[frame->nowSelectStage];
		}

		inline const std::vector<ChPtr::Shared<StageData>> GetStageDataList()
		{
			return frame->stageDataList;
		}

		inline ChD3D11::Texture11& GetNonImage()
		{
			return frame->notImageTexture;
		}
	protected:

		inline void Cancel()
		{
			frame->Cancel();
		}

	protected:

		inline void UpNowSelectStage()
		{
			frame->nowSelectStage--;

			frame->nowSelectStage = (frame->nowSelectStage + frame->stageDataList.size()) % frame->stageDataList.size();
		}

		inline void DownNowSelectStage()
		{
			frame->nowSelectStage++;

			frame->nowSelectStage = frame->nowSelectStage % frame->stageDataList.size();
		}

	private:

		StageSelectFrame* frame = nullptr;
	};

public:

	void Init(ChPtr::Shared<ChCpp::SendDataClass> _sendData)override;

	void Release()override;

	void Update()override;

private:

	void InitStageDataList();

	void DrawFunction();

	void UpdateFunction();

	void UpdateKeyboard();

	void UpdateController();

private:

	void SetGameFrame();

	void SetEditFrame();

private:

	void Cancel();

private:

	DisplayType type = DisplayType::Select;

	std::vector<ActionType> inputDataList;

	ChD3D::XInputController controller;
	ChCpp::BitBool conntrollerPushKey;

	std::vector<ChPtr::Shared<StageData>>stageDataList;

	ChD3D11::Shader::BaseDrawSprite11 spriteShader;

	int nowSelectStage = 0;

	ChD3D11::Texture11 notImageTexture;


	ChPtr::Shared<StageSelectFrameDisplay>stageSelectFrameDisplay[2];


	bool firstFlg = true;
};
