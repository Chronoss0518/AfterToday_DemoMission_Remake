
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
	
	controller.Init();

	MenuBase::InitMenu(&controller);

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

}

void SelectFrame::Release()
{
	controller.Release();
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

void SelectFrame::UpdateAction(ActionType _type)
{
	if (_type == ActionType::Decision)
	{
		nextFrameFunction[(NextButtonType)nowSelect]();
		SetLoopBreakTrue();
	}

	if (_type == ActionType::Up)
	{

		nowSelect = (nowSelect + NEXT_BUTTON_TYPE_COUNT - 1) % NEXT_BUTTON_TYPE_COUNT;
	}

	if (_type == ActionType::Down)
	{
		nowSelect = (nowSelect + 1) % NEXT_BUTTON_TYPE_COUNT;
	}

}

void SelectFrame::Update()
{

	UpdateFunction();

	ChVec4 rect = toButton[nowSelect].sprite.GetPosRect();

	DrawFunction();

}

void SelectFrame::UpdateMouse()
{

	auto&& manager = ChSystem::SysManager();

	MenuBase::InputTest(ActionType::Decision, manager.IsPushKeyNoHold(VK_LBUTTON));

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
