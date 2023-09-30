
#include"../BaseIncluder.h"

#include"SelectFrame.h"

#define SELECT_TEXTURE_DIRECTORY(current_path) TEXTURE_DIRECTORY("Select/") current_path
#define SELECT_MESH_DIRECTORY(current_path) MESH_DIRECTORY("Select/") current_path

void SelectFrame::Init()
{
	ChD3D11::Shader11().SetBackColor(ChVec4(0.0f, 0.0f, 0.0f, 1.0f));

	auto&& device = ChD3D11::D3D11Device();

	spriteShader.Init(device);

	descriptionWindowSprite.Init(device);
	descriptionWindowSprite.SetInitPosition();
	descriptionWindowSprite.SetPosRect(RectToGameWindow(ChVec4(0.0f, 544.0f, 1280.0f, 544.0f + 176.0f)));

	selectEdge.CreateTexture(SELECT_TEXTURE_DIRECTORY("SelectButtonEdge.png"), device);


	toBattle.CreateTexture(SELECT_TEXTURE_DIRECTORY("BattleButton.png"), device);
	description[ChStd::EnumCast(NextButtonType::Battle)].CreateTexture(SELECT_TEXTURE_DIRECTORY("BattleButtonDescription_jp.png"), device);

	toBattleButtonSprite.Init(device);
	toBattleButtonSprite.SetInitPosition();
	toBattleButtonSprite.SetPosRect(RectToGameWindow(ChVec4(340.0f, 62.0f, 600.0f + 340.0f, 62.0f + 100.0f)));
	
	selectSprite[ChStd::EnumCast(NextButtonType::Battle)].Init(device);
	selectSprite[ChStd::EnumCast(NextButtonType::Battle)].SetInitPosition();
	selectSprite[ChStd::EnumCast(NextButtonType::Battle)].SetPosRect(RectToGameWindow(ChVec4(340.0f - 5.0f, 62.0f - 5.0f, 600.0f + 340.0f + 5.0f, 62.0f + 100.0f + 5.0f)));


	toEdit.CreateTexture(SELECT_TEXTURE_DIRECTORY("EditButton.png"), device);
	description[ChStd::EnumCast(NextButtonType::Edit)].CreateTexture(SELECT_TEXTURE_DIRECTORY("EditButtonDescription_jp.png"), device);

	toEditButtonSprite.Init(device);
	toEditButtonSprite.SetInitPosition();
	toEditButtonSprite.SetPosRect(RectToGameWindow(ChVec4(340.0f, 232.0f, 600.0f + 340.0f, 232.0f + 100.0f)));

	selectSprite[ChStd::EnumCast(NextButtonType::Edit)].Init(device);
	selectSprite[ChStd::EnumCast(NextButtonType::Edit)].SetInitPosition();
	selectSprite[ChStd::EnumCast(NextButtonType::Edit)].SetPosRect(RectToGameWindow(ChVec4(340.0f - 5.0f, 232.0f - 5.0f, 600.0f + 340.0f + 5.0f, 232.0f + 100.0f + 5.0f)));


	toSetting.CreateTexture(SELECT_TEXTURE_DIRECTORY("OptionButton.png"), device);
	description[ChStd::EnumCast(NextButtonType::Setting)].CreateTexture(SELECT_TEXTURE_DIRECTORY("OptionButtonDescription_jp.png"), device);

	toSettingButtonSprite.Init(device);
	toSettingButtonSprite.SetInitPosition();
	toSettingButtonSprite.SetPosRect(RectToGameWindow(ChVec4(340.0f, 401.0f, 600.0f + 340.0f, 401.0f + 100.0f)));

	selectSprite[ChStd::EnumCast(NextButtonType::Setting)].Init(device);
	selectSprite[ChStd::EnumCast(NextButtonType::Setting)].SetInitPosition();
	selectSprite[ChStd::EnumCast(NextButtonType::Setting)].SetPosRect(RectToGameWindow(ChVec4(340.0f - 5.0f, 401.0f - 5.0f, 600.0f + 340.0f + 5.0f, 401.0f + 100.0f + 5.0f)));



	nextFrameFunction[NextButtonType::Battle] = [&]()
	{
		ChangeFrame(ChStd::EnumCast(FrameNo::Game));
	};

	nextFrameFunction[NextButtonType::Edit] = [&]()
	{

	};


	nextFrameFunction[NextButtonType::Setting] = [&]()
	{

	};

}

void SelectFrame::Release()
{

}

void SelectFrame::DrawFunction()
{
	auto&& dc = ChD3D11::D3D11DC();

	ChD3D11::Shader11().DrawStart();

	spriteShader.DrawStart(dc);



	spriteShader.Draw(toBattle, toBattleButtonSprite);
	spriteShader.Draw(toEdit, toEditButtonSprite);
	spriteShader.Draw(toSetting, toSettingButtonSprite);
	spriteShader.Draw(selectEdge, selectSprite[nowSelect]);


	spriteShader.Draw(description[nowSelect], descriptionWindowSprite);
	spriteShader.DrawEnd();

	ChD3D11::Shader11().DrawEnd();
}

void SelectFrame::UpdateFunction()
{

	UpdateKeyboard();
	UpdateMouse();

	if (firstFlg)
	{

		inputDataList.clear();
		firstFlg = false;
		return;
	}

	for (auto&& inputData : inputDataList)
	{
		if (inputData == ActionType::UpSelect)
		{
			nowSelect--;
		}

		if (inputData == ActionType::DownSelect)
		{
			nowSelect++;
		}

		nowSelect = (nowSelect + NEXT_BUTTON_TYPE_COUNT) % NEXT_BUTTON_TYPE_COUNT;

		if (inputData == ActionType::Decision)
		{
			nextFrameFunction[(NextButtonType)nowSelect]();
		}

	}

	inputDataList.clear();
}

void SelectFrame::Update()
{
	UpdateFunction();

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

	if (IsMoucePosOnSprite(toBattleButtonSprite))
	{
		nowSelect = ChStd::EnumCast(NextButtonType::Battle);
	}

	if (IsMoucePosOnSprite(toEditButtonSprite))
	{
		nowSelect = ChStd::EnumCast(NextButtonType::Edit);
	}

	if (IsMoucePosOnSprite(toSettingButtonSprite))
	{
		nowSelect = ChStd::EnumCast(NextButtonType::Setting);
	}

}

void SelectFrame::UpdateKeyboard()
{

	auto&& manager = ChSystem::SysManager();

	if (manager.IsPushKeyNoHold(VK_RETURN))
	{
		inputDataList.push_back(ActionType::Decision);
	}

	if (manager.IsPushKeyNoHold(VK_UP))
	{
		inputDataList.push_back(ActionType::UpSelect);
	}

	if (manager.IsPushKeyNoHold(VK_DOWN))
	{
		inputDataList.push_back(ActionType::DownSelect);
	}
}

void SelectFrame::UpdateController()
{

}

void SelectFrame::SetScript()
{

}
