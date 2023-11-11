
#include"../BaseIncluder.h"

#include"StageSelectFrame.h"
#include"../StageSelectFrame/StageData/StageData.h"
#include"../StageSelectFrame/StageDataDrawer/StageDataDrawer.h"

#define SELECT_TEXTURE_DIRECTORY(current_path) TEXTURE_DIRECTORY("StageImage/") current_path

void StageSelectFrame::Init()
{
	ChD3D11::Shader11().SetBackColor(ChVec4(0.0f, 0.0f, 0.0f, 1.0f));

	auto&& device = ChD3D11::D3D11Device();

	spriteShader.Init(device);

	textDrawer.Init(ChD3D11::D3D11API().GetSurface(), ChD3D::DirectFont::LocaleNameId::Japanese);

	for (auto&& sprite : stagePreImageSprite)
	{
		sprite.Init(device);
	}



	for (auto&& file : std::filesystem::directory_iterator(STAGE_DATA_DIRECTORY("")))
	{
		std::string path = ChStr::UTF8ToString(file.path().c_str());
		auto stageData = ChPtr::Make_S<StageData>();
		stageData->LoadData(path);
		stageDataList.push_back(stageData);
	}

	

}

void StageSelectFrame::Release()
{

}

void StageSelectFrame::DrawFunction()
{
	auto&& dc = ChD3D11::D3D11DC();

	ChD3D11::Shader11().DrawStart();

	spriteShader.DrawStart(dc);



	spriteShader.DrawEnd();

	ChD3D11::Shader11().DrawEnd();
}

void StageSelectFrame::UpdateFunction()
{

	UpdateKeyboard();
	UpdateMouse();

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

		//nowSelect = (nowSelect + NEXT_BUTTON_TYPE_COUNT) % NEXT_BUTTON_TYPE_COUNT;

		if (inputData == ActionType::Decision)
		{

		}

	}

	inputDataList.clear();
}

void StageSelectFrame::Update()
{
	UpdateFunction();

	DrawFunction();
}

void StageSelectFrame::UpdateMouse()
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

	//if (IsMoucePosOnSprite(toBattleButtonSprite))

}

void StageSelectFrame::UpdateKeyboard()
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

void StageSelectFrame::UpdateController()
{

}

void StageSelectFrame::SetScript()
{

}
