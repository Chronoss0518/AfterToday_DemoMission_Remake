
#include"../BaseIncluder.h"

#include"SelectFrame.h"

#define SELECT_TEXTURE_DIRECTORY(current_path) TEXTURE_DIRECTORY("Select/") current_path
#define SELECT_MESH_DIRECTORY(current_path) MESH_DIRECTORY("Select/") current_path

#define BUTTON_X_POSITION 340.0f

#define GAME_BUTTON_Y_POSITION 62.0f
#define EDIT_BUTTON_Y_POSITION 232.0f
#define SETTING_BUTTON_Y_POSITION 401.0f

#define BUTTON_WIDTH 600.0f + BUTTON_X_POSITION
#define BUTTON_HEIGHT(Y_POSITION) 100.0f + Y_POSITION


void SelectFrame::Init(ChPtr::Shared<ChCpp::SendDataClass> _sendData)
{
	ChD3D11::Shader11().SetBackColor(ChVec4(0.0f, 0.0f, 0.0f, 1.0f));

	auto&& device = ChD3D11::D3D11Device();

	spriteShader.Init(device);

	SPRITE_INIT(descriptionWindowSprite, RectToGameWindow(ChVec4(0.0f, 544.0f, GAME_WINDOW_WIDTH, 544.0f + 176.0f)));


	selectEdge.CreateTexture(SELECT_TEXTURE_DIRECTORY("SelectButtonEdge.png"), device);
	comingSoonTexture.CreateTexture(SELECT_TEXTURE_DIRECTORY("Coming Soon.png"), device);

	toButton[ChStd::EnumCast(NextButtonType::Battle)].image.CreateTexture(SELECT_TEXTURE_DIRECTORY("BattleButton.png"), device);
	description[ChStd::EnumCast(NextButtonType::Battle)].CreateTexture(SELECT_TEXTURE_DIRECTORY("BattleButtonDescription_jp.png"), device);

	toButton[ChStd::EnumCast(NextButtonType::Edit)].image.CreateTexture(SELECT_TEXTURE_DIRECTORY("EditButton.png"), device);
	description[ChStd::EnumCast(NextButtonType::Edit)].CreateTexture(SELECT_TEXTURE_DIRECTORY("EditButtonDescription_jp.png"), device);

	toButton[ChStd::EnumCast(NextButtonType::Setting)].image.CreateTexture(SELECT_TEXTURE_DIRECTORY("OptionButton.png"), device);
	description[ChStd::EnumCast(NextButtonType::Setting)].CreateTexture(SELECT_TEXTURE_DIRECTORY("OptionButtonDescription_jp.png"), device);

	SPRITE_INIT(toButton[ChStd::EnumCast(NextButtonType::Battle)].sprite, RectToGameWindow(ChVec4(BUTTON_X_POSITION, GAME_BUTTON_Y_POSITION, BUTTON_WIDTH, BUTTON_HEIGHT(GAME_BUTTON_Y_POSITION))));

	SPRITE_INIT(toButton[ChStd::EnumCast(NextButtonType::Edit)].sprite, RectToGameWindow(ChVec4(BUTTON_X_POSITION, EDIT_BUTTON_Y_POSITION, BUTTON_WIDTH, BUTTON_HEIGHT(EDIT_BUTTON_Y_POSITION))));

	SPRITE_INIT(toButton[ChStd::EnumCast(NextButtonType::Setting)].sprite, RectToGameWindow(ChVec4(BUTTON_X_POSITION, SETTING_BUTTON_Y_POSITION, BUTTON_WIDTH, BUTTON_HEIGHT(SETTING_BUTTON_Y_POSITION))));

	nextFrameFunction[NextButtonType::Battle] = [&]()
	{
		ChangeFrame(ChStd::EnumCast(FrameNo::SelectStage));
	};

	nextFrameFunction[NextButtonType::Edit] = [&]()
	{
		//ChangeFrame(ChStd::EnumCast(FrameNo::Edit));
	};


	nextFrameFunction[NextButtonType::Setting] = [&]()
	{
		//ChangeFrame(ChStd::EnumCast(FrameNo::Setting));
	};

	controller.Init();
}

void SelectFrame::Release()
{

}

void SelectFrame::DrawFunction()
{
	auto&& dc = ChD3D11::D3D11DC();

	ChD3D11::Shader11().DrawStart();

	spriteShader.DrawStart(dc);

	spriteShader.Draw(selectEdge, toButton[nowSelect].sprite);

	spriteShader.Draw(comingSoonTexture, toButton[ChStd::EnumCast(NextButtonType::Edit)].sprite);
	spriteShader.Draw(comingSoonTexture, toButton[ChStd::EnumCast(NextButtonType::Setting)].sprite);

	for (auto&& imageSprite : toButton)
	{
		spriteShader.Draw(imageSprite.image, imageSprite.sprite);
	}

	spriteShader.Draw(description[nowSelect], descriptionWindowSprite);
	spriteShader.DrawEnd();

	ChD3D11::Shader11().DrawEnd();
}

void SelectFrame::UpdateFunction()
{

	UpdateKeyboard();
	UpdateMouse();
	UpdateController();

	for (auto&& inputData : inputDataList)
	{
		if (inputData == ActionType::Decision)
		{
			nextFrameFunction[(NextButtonType)nowSelect]();
			break;
		}

		if (inputData == ActionType::UpSelect)
		{
			nowSelect--;
		}

		if (inputData == ActionType::DownSelect)
		{
			nowSelect++;
		}

		nowSelect = (nowSelect + NEXT_BUTTON_TYPE_COUNT) % NEXT_BUTTON_TYPE_COUNT;

	}

	inputDataList.clear();
}

void SelectFrame::Update()
{
	if (firstFlg)
	{
		inputDataList.clear();
		conntrollerPushKey.SetBitTrue(ChStd::EnumCast(ActionType::Decision));
		firstFlg = false;
		return;
	}

	UpdateFunction();

	ChVec4 rect = toButton[nowSelect].sprite.GetPosRect();

	DrawFunction();

}

void SelectFrame::UpdateMouse()
{

	auto&& manager = ChSystem::SysManager();

	if (manager.IsPushKeyNoHold(VK_LBUTTON))
	{
		inputDataList.push_back(ActionType::Decision);
	}

	auto&& mouce = ChWin::Mouse();
	mouce.Update();

	auto&& mouseMove = mouce.GetMoveValue();

	if (std::abs(mouseMove.x) <= 1 && std::abs(mouseMove.y) <= 1)return;

	for (int i = 0; i < NEXT_BUTTON_TYPE_COUNT; i++)
	{
		if (!IsMoucePosOnSprite(toButton[i].sprite))continue;
		nowSelect = i;
		break;
	}
}

void SelectFrame::UpdateKeyboard()
{

	auto&& manager = ChSystem::SysManager();

	if (manager.IsPushKeyNoHold(VK_RETURN) || manager.IsPushKeyNoHold(VK_SPACE))
	{
		inputDataList.push_back(ActionType::Decision);
	}

	if (manager.IsPushKeyNoHold(VK_UP) || manager.IsPushKeyNoHold('W'))
	{
		inputDataList.push_back(ActionType::UpSelect);
	}

	if (manager.IsPushKeyNoHold(VK_DOWN) || manager.IsPushKeyNoHold('S'))
	{
		inputDataList.push_back(ActionType::DownSelect);
	}
}

void SelectFrame::UpdateController()
{
	controller.Update();

	bool isPushFlg = false;

	if (controller.GetAFlg())
	{
		if (!conntrollerPushKey.GetBitFlg(ChStd::EnumCast(ActionType::Decision)))
			inputDataList.push_back(ActionType::Decision);
		conntrollerPushKey.SetBitTrue(ChStd::EnumCast(ActionType::Decision));
		isPushFlg = true;
	}

	if (controller.GetUpFlg() || controller.GetLYStick() > 0.3f)
	{
		if (!conntrollerPushKey.GetBitFlg(ChStd::EnumCast(ActionType::UpSelect)))
			inputDataList.push_back(ActionType::UpSelect);
		conntrollerPushKey.SetBitTrue(ChStd::EnumCast(ActionType::UpSelect));
		isPushFlg = true;
	}

	if (controller.GetDownFlg() || controller.GetLYStick() < -0.3f)
	{
		if (!conntrollerPushKey.GetBitFlg(ChStd::EnumCast(ActionType::DownSelect)))
			inputDataList.push_back(ActionType::DownSelect);
		conntrollerPushKey.SetBitTrue(ChStd::EnumCast(ActionType::DownSelect));
		isPushFlg = true;
	}

	if (isPushFlg)return;
	conntrollerPushKey.SetAllDownFlg();

}

void SelectFrame::SetScript()
{

}
