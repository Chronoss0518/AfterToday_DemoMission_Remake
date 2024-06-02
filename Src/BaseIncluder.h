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

#ifndef GAME_SPRITE_WIDTH
#define GAME_SPRITE_WIDTH 1280.0f 
#endif

#ifndef GAME_SPRITE_HEIGHT
#define GAME_SPRITE_HEIGHT 720.0f 
#endif

#ifndef	FULL_HD_FLG
#define	FULL_HD_FLG 0
#endif

#ifndef	HD_FLG
#define	HD_FLG 1
#endif

#ifndef	SD_FLG
#define	SD_FLG 1
#endif

#ifndef GAME_WINDOW_WIDTH
#if FULL_HD_FLG
#define GAME_WINDOW_WIDTH 1920.0f
#elif HD_FLG
#define GAME_WINDOW_WIDTH 1280.0f
#elif SD_FLG
#define GAME_WINDOW_WIDTH 720.0f
#endif
#endif

#ifndef GAME_WINDOW_HEIGHT
#if FULL_HD_FLG
#define GAME_WINDOW_HEIGHT 1080.0f
#elif HD_FLG
#define GAME_WINDOW_HEIGHT 720.0f
#elif SD_FLG
#define GAME_WINDOW_HEIGHT 480.0f
#endif
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
#define SCREEN_TO_PROJECTION_Y(value) ((static_cast<float>(value)/(GAME_WINDOW_HEIGHT)) * -2.0f) + 1.0f
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
#define USE_EDIT_FRAME_FLG 1
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
	return SCREEN_TO_PROJECTION_X(_x);
}

static inline float VerticalToProjection(float _y)
{
	return SCREEN_TO_PROJECTION_Y(_y);
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

//X,Y,ZÇÃêî//
static constexpr unsigned char AXIS_BASE_TYPE_NUM = 3;

//RotateAxisÇ…ìoò^Ç≥ÇÍÇƒÇ¢ÇÈêî//
static constexpr unsigned char AXIS_TYPE_NUM = 6;

enum class RotateAxis : unsigned char
{
	PX,
	PY,
	PZ,
	MX,
	MY,
	MZ
};

class PostureController: public ChCpp::BaseComponent
{
public://Set Functions//


	void Set(const PostureController& _val)
	{
		axis = _val.axis;
		minRotate = _val.minRotate;
		maxRotate = _val.maxRotate;
	}

	inline void SetRotateAxis(const RotateAxis _axis)
	{
		axis = _axis;
	}

	inline void SetMinRotate(const float& _rotate)
	{
		minRotate = _rotate;
	}

	inline void SetMaxRotate(const float& _rotate)
	{
		maxRotate = _rotate;
	}

	inline void SetRotate(float _rotate)
	{
		auto&& frame = LookObj<ChCpp::FrameObject>();

		if (ChPtr::NullCheck(frame))return;
		if(_rotate == 0.0f)
		{
			frame->SetOutSizdTransform(ChLMat());
			return;
		}
		ChLMat tmpMat;

		auto&& direction = ChStd::EnumCast(axis) % AXIS_BASE_TYPE_NUM;

		float rotate = ChStd::EnumCast(axis) > AXIS_BASE_TYPE_NUM ? -_rotate : _rotate;

		if(direction == (ChStd::EnumCast(RotateAxis::PX) % AXIS_BASE_TYPE_NUM))tmpMat.SetRotationXAxis(rotate);
		if(direction == (ChStd::EnumCast(RotateAxis::PY) % AXIS_BASE_TYPE_NUM))tmpMat.SetRotationYAxis(rotate);
		if(direction == (ChStd::EnumCast(RotateAxis::PZ) % AXIS_BASE_TYPE_NUM))tmpMat.SetRotationZAxis(rotate);

		frame->SetOutSizdTransform(tmpMat);
	}


public://Get Functions//

	inline RotateAxis GetRotateAxis() { return axis; }

	inline float GetMinRotate() { return minRotate; }

	inline float GetMaxRotate() { return maxRotate; }

	inline ChCpp::BaseObject* GetLookObject() { return LookObj(); }

private:

	//âÒì]é≤//
	RotateAxis axis = RotateAxis::PX;

	//ç≈í·âÒì]ó //
	float minRotate = 0.0f;

	//ç≈í·âÒì]ó //
	float maxRotate = 0.0f;
};