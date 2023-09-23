
#include"../BaseIncluder.h"

#include"SelectFrame.h"

#define SELECT_TEXTURE_DIRECTORY(current_path) TEXTURE_DIRECTORY("Select/") current_path
#define SELECT_MESH_DIRECTORY(current_path) MESH_DIRECTORY("Select/") current_path

ChVec4 RectToGameWindow(const ChVec4 _rect)
{
	ChVec4 res = _rect;

	res.left = res.left / GAME_WINDOW_WITDH;
	res.left = (res.left * 2.0f) - 1.0f;

	res.right = res.right / GAME_WINDOW_WITDH;
	res.right = (res.right * 2.0f) - 1.0f;

	res.top = res.top / GAME_WINDOW_HEIGHT;
	res.top = (res.top * -2.0f) + 1.0f;

	res.bottom = res.bottom / GAME_WINDOW_HEIGHT;
	res.bottom = (res.bottom * -2.0f) + 1.0f;

	return res;
}

void SelectFrame::Init()
{
	auto&& device = ChD3D11::D3D11Device();

	ChD3D11::Shader11().SetBackColor(ChVec4(0.0f, 0.0f, 0.0f, 1.0f));

	spriteShader.Init(device);

	descriptionWindowSprite.Init(device);
	descriptionWindowSprite.SetInitPosition();
	descriptionWindowSprite.SetPosRect(RectToGameWindow(ChVec4(0.0f, 544.0f, 1280.0f, 544.0f + 176.0f)));

	toBattle.CreateTexture(SELECT_TEXTURE_DIRECTORY("BattleButton.png"), device);
	descriptionBattle.CreateTexture(SELECT_TEXTURE_DIRECTORY("BattleButtonDescription_jp.png"), device);

	toBattleButtonSprite.Init(device);
	toBattleButtonSprite.SetInitPosition();
	toBattleButtonSprite.SetPosRect(RectToGameWindow(ChVec4(340.0f, 62.0f, 600.0f + 340.0f, 62.0f + 100.0f)));

	toEdit.CreateTexture(SELECT_TEXTURE_DIRECTORY("EditButton.png"), device);
	descriptionEdit.CreateTexture(SELECT_TEXTURE_DIRECTORY("EditButtonDescription_jp.png"), device);

	toEditButtonSprite.Init(device);
	toEditButtonSprite.SetInitPosition();
	toEditButtonSprite.SetPosRect(RectToGameWindow(ChVec4(340.0f, 232.0f, 600.0f + 340.0f, 232.0f + 100.0f)));
	
	toSetting.CreateTexture(SELECT_TEXTURE_DIRECTORY("OptionButton.png"), device);
	descriptionSetting.CreateTexture(SELECT_TEXTURE_DIRECTORY("OptionButtonDescription_jp.png"), device);

	toSettingButtonSprite.Init(device);
	toSettingButtonSprite.SetInitPosition();
	toSettingButtonSprite.SetPosRect(RectToGameWindow(ChVec4(340.0f, 401.0f, 600.0f + 340.0f, 401.0f + 100.0f)));
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

	spriteShader.Draw(descriptionBattle, descriptionWindowSprite);
	spriteShader.DrawEnd();

	ChD3D11::Shader11().DrawEnd();
}

void SelectFrame::UpdateFunction()
{

}

void SelectFrame::Update()
{
	UpdateFunction();

	DrawFunction();
}

void SelectFrame::SetScript()
{

}
