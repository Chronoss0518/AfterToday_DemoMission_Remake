#pragma once

#include<Windows.h>
#include<filesystem>

#include<ChBaseLibrary.h>
#include<ChWindowsLibrary.h>
#include<ChDirect3D11Library.h>
#include<PhysicsMachine.h>

#ifndef BETA 
#define BETA
#endif

#ifndef USE_THREAD
#define USE_THREAD false
#endif

#ifndef USE_CPU_THREAD
#define USE_CPU_THREAD true
#endif

#ifndef TARGET_DIRECTORY 
//#define	TARGET_DIRECTORY(current_path) "../é¿çs/data/" current_path
#define	TARGET_DIRECTORY(current_path) "Resource/" current_path
#endif

#ifndef SAVE_DIRECTORY
#define SAVE_DIRECTORY(current_path) TARGET_DIRECTORY("Save/" current_path) 
#endif

#ifndef PLAYER_USE_MECHA_PATH
#define PLAYER_USE_MECHA_PATH SAVE_DIRECTORY("Player.amf") 
#endif

#ifndef PLAYER_MECHA_PATH
#define PLAYER_MECHA_PATH SAVE_DIRECTORY("AssemMechaFrameList.json")
#endif

#ifndef SOUND_DIRECTORY
#define SOUND_DIRECTORY(current_path) TARGET_DIRECTORY("Sound/" current_path) 
#endif

#ifndef TEXTURE_DIRECTORY
#define TEXTURE_DIRECTORY(current_path) TARGET_DIRECTORY("Texture/" current_path) 
#endif

#ifndef MESH_DIRECTORY
#define MESH_DIRECTORY(current_path) TARGET_DIRECTORY("XFile/" current_path) 
#endif

#ifndef STAGE_DIRECTORY
#define STAGE_DIRECTORY(current_path) TARGET_DIRECTORY("StageScript/" current_path) 
#endif

#ifndef STAGE_DATA_DIRECTORY
#define STAGE_DATA_DIRECTORY(current_path) TARGET_DIRECTORY("StageData/" current_path) 
#endif

#ifndef CPU_DIRECTORY
#define CPU_DIRECTORY(current_path) TARGET_DIRECTORY("CPUCharactor/" current_path) 
#endif

#ifndef CPU_MECHA_PATH
#define CPU_MECHA_PATH(_fileName) TARGET_DIRECTORY("StageScript/CPU/" _fileName)
#endif

#ifndef GAME_WINDOW_WIDTH
#define GAME_WINDOW_WIDTH 1280.0f
#endif

#ifndef GAME_WINDOW_HEIGHT
#define GAME_WINDOW_HEIGHT 720.0f
#endif

#ifndef GAME_WINDOW_WIDTH_LONG
#define GAME_WINDOW_WIDTH_LONG static_cast<unsigned long>(GAME_WINDOW_WIDTH)
#endif

#ifndef GAME_WINDOW_HEIGHT_LONG
#define GAME_WINDOW_HEIGHT_LONG static_cast<unsigned long>(GAME_WINDOW_HEIGHT)
#endif

//px = (sx / sw) * 2.0f - 1.0f
#ifndef SCREEN_TO_PROJECTION_X
#define SCREEN_TO_PROJECTION_X(value) (static_cast<float>(value)/(GAME_WINDOW_WIDTH)) * 2.0f - 1.0f
#endif

//py = 1.0f - ((sy / sh) * 2.0f)
#ifndef SCREEN_TO_PROJECTION_Y
#define SCREEN_TO_PROJECTION_Y(value) 1.0f - ((static_cast<float>(value)/(GAME_WINDOW_HEIGHT)) * 2.0f)
#endif

#ifndef GAME_PROJECTION_NEAR
#define GAME_PROJECTION_NEAR 0.1f
#endif

#ifndef GAME_PROJECTION_FAR
#define GAME_PROJECTION_FAR 10000.0f
#endif

#ifndef SPRITE_INIT
#define SPRITE_INIT(_SPRITE,_RECT) \
_SPRITE.Init(); \
_SPRITE.SetInitPosition(); \
_SPRITE.SetPosRect(_RECT)
#endif

#ifndef TEXT_TO_COLOR_NUMBER
#define TEXT_TO_COLOR_NUMBER(_text)  static_cast<float>(ChStd::BaseNumberToDecimalNumber(_text, ChStd::HEXA_DECIMAL())) / 255.0f
#endif

#ifndef USE_TITLE_FRAME_FLG
#define USE_TITLE_FRAME_FLG 0
#endif

#ifndef USE_SELECT_FRAME_FLG
#define USE_SELECT_FRAME_FLG 1
#endif

#ifndef USE_STAGE_SELECT_FRAME_FLG
#define USE_STAGE_SELECT_FRAME_FLG 1
#endif

#ifndef USE_GAME_FRAME_FLG
#define USE_GAME_FRAME_FLG 1
#endif

#ifndef USE_EDIT_FRAME_FLG
#define USE_EDIT_FRAME_FLG 0
#endif

#ifndef USE_SETTING_FRAME_FLG
#define USE_SETTING_FRAME_FLG 0
#endif

#ifndef USE_RESULT_FRAME_FLG
#define USE_RESULT_FRAME_FLG 1
#endif



enum class FrameNo :unsigned long
{
#if USE_TITLE_FRAME_FLG
	Title,
#endif
#if USE_SELECT_FRAME_FLG
	Select,
#endif
#if USE_STAGE_SELECT_FRAME_FLG
	SelectStage,
#endif
#if USE_GAME_FRAME_FLG
	Game,
#endif
#if USE_EDIT_FRAME_FLG
	Edit,
#endif
#if USE_SETTING_FRAME_FLG
	Setting,
#endif
#if USE_RESULT_FRAME_FLG
	Result,
#endif
};

struct PlayerData:public ChCpp::SaveDataClass
{
	std::string playerName = "";
};

struct ImageSprite
{
	ChD3D11::Sprite11 sprite;
	ChD3D11::Texture11 image;
};

struct TextDrawerWICBitmap{
	ChD3D::DirectFontFromWICBitmap drawer;
	ChD3D::TextFormatObject format;
	ChD3D::BrushObject brush;
	ChD3D::WICBitmapObject bitmap;
};

static inline void ReleaseMesh11(ChPtr::Shared<ChD3D11::Mesh11>& _meshObject)
{
	if (_meshObject == nullptr)return;
	_meshObject->Release();
	_meshObject = nullptr;
}

static inline float HorizontalToProjection(float _x)
{
	float res = _x;
	res = res / GAME_WINDOW_WIDTH;
	res = (res * 2.0f) - 1.0f;
	return res;
}

static inline float VerticalToProjection(float _y)
{
	float res = _y;
	res = res / GAME_WINDOW_HEIGHT;
	res = (res * -2.0f) + 1.0f;
	return res;
}

static inline ChVec4 RectToGameWindow(const ChVec4 _rect)
{
	ChVec4 res = _rect;

	res.left = HorizontalToProjection(res.left);
	res.right = HorizontalToProjection(res.right);

	res.top = VerticalToProjection(res.top);
	res.bottom = VerticalToProjection(res.bottom);

	return res;
}

static inline ChVec3 ColorTextToColorVector3(
	const std::string& _r,
	const std::string& _g, 
	const std::string& _b)
{
	std::vector<char> r = { _r[0],_r[1] };
	std::vector<char> g = { _g[0],_g[1] };
	std::vector<char> b = { _b[0],_b[1] };

	return ChVec3::FromColor(
		TEXT_TO_COLOR_NUMBER(r),
		TEXT_TO_COLOR_NUMBER(g),
		TEXT_TO_COLOR_NUMBER(b)
		);
}

static inline bool IsMoucePosOnRect(const ChVec4& _rect)
{
	ChVec2 mousePos = ChWin::Mouse().GetNowProPosToChVec2();

	bool res = false;

	if (_rect.IsOnPoint(mousePos))res = true;

	return res;
}

static inline bool IsMoucePosOnSprite(const ChD3D11::Sprite11& _sprite)
{
	ChVec2 lt = _sprite.GetPos(ChD3D11::SpritePositionName::LeftTop);
	ChVec2 rb = _sprite.GetPos(ChD3D11::SpritePositionName::RightBottom);

	ChVec4 tmpRect;
	tmpRect.left = lt.x;
	tmpRect.top = lt.y;
	tmpRect.right = rb.x;
	tmpRect.bottom = rb.y;

	return IsMoucePosOnRect(tmpRect);
}

static inline std::wstring CreateMoneyText(const std::wstring& _money)
{
	std::wstring result = L"";

	for (unsigned long i = 0; i < _money.length(); i++)
	{
		result += _money[i];
		if (i + 1 >= _money.length())break;
		if ((_money.length() - i - 1) % 3 != 0)continue;
		result += L",";
	}

	return result;
}
