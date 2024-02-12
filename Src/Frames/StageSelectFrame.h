#pragma once

#include"FromStageSelectFrameData.h"

#include"MenuBase.h"

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

class LoadDisplay;
class StageData;

class StageSelectFrame :public ChCpp::BaseFrame,public MenuBase
{
public:

	enum class DisplayType
	{
		Select,//‘I‘ð‰æ–Ê//
		Detailed//Ú×‰æ–Ê//
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

		virtual void UpdateAction(ActionType _action) = 0;

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
			frame->MenuBase::AddActionType(_type);
		}

		void AddAction(ActionType _type,bool _inputFlg)
		{
			frame->MenuBase::MouseTest(_type, _inputFlg);
		}

	protected:

		inline unsigned long GetNowSelectCount()
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

		void OpenLoadDisplay();

	protected:

		inline bool UpNowSelectStage()
		{
			bool res = static_cast<int>(frame->nowSelectStage) - 1< 0;

			frame->nowSelectStage = (frame->nowSelectStage + frame->stageDataList.size() - 1) % frame->stageDataList.size();

			return res;
		}

		inline bool DownNowSelectStage()
		{
			bool res = frame->nowSelectStage + 1 >= frame->stageDataList.size();

			frame->nowSelectStage = (frame->nowSelectStage + frame->stageDataList.size() + 1) % frame->stageDataList.size();

			return res;
		}

	private:

		StageSelectFrame* frame = nullptr;
	};

public:

	void Init(ChPtr::Shared<ChCpp::SendDataClass> _sendData)override;

	void Release()override;

	void Update()override;

private:

	void UpdateAction(ActionType _type)override;

	void UpdateMouse()override;

private:

	void InitStageDataList();

	void DrawFunction();

private:

	void SetGameFrame();

	void SetEditFrame();

private:

	void Cancel();

	void OpenLoadDisplay();

private:

	ChPtr::Shared<LoadDisplay>loadDisplay = nullptr;

	ID3D11RenderTargetView* rtView = nullptr;

	DisplayType type = DisplayType::Select;

	std::vector<ChPtr::Shared<StageData>>stageDataList;

	ChD3D11::Shader::BaseDrawSprite11 spriteShader;

	unsigned long nowSelectStage = 0;

	ChD3D11::Texture11 notImageTexture;

	ChPtr::Shared<StageSelectFrameDisplay>stageSelectFrameDisplay[2];

	ChD3D::XInputController controller;
};
