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
#define USE_THREAD true
#endif

#ifndef USE_CPU_THREAD
#define USE_CPU_THREAD true
#endif

#ifndef TARGET_DIRECTORY 
#define	TARGET_DIRECTORY(current_path) L"Resource/" current_path
#endif

#ifndef SAVE_DIRECTORY
#define SAVE_DIRECTORY(current_path) TARGET_DIRECTORY(L"Save/" current_path) 
#endif

#ifndef PLAYER_USE_MECHA_PATH
#define PLAYER_USE_MECHA_PATH SAVE_DIRECTORY(L"Player.amf") 
#endif

#ifndef PLAYER_MECHA_PATH
#define PLAYER_MECHA_PATH SAVE_DIRECTORY(L"AssemMechaFrameList.json")
#endif

#ifndef SOUND_DIRECTORY
#define SOUND_DIRECTORY(current_path) TARGET_DIRECTORY(L"Sound/" current_path) 
#endif

#ifndef TEXTURE_DIRECTORY
#define TEXTURE_DIRECTORY(current_path) TARGET_DIRECTORY(L"Texture/" current_path) 
#endif

#ifndef MESH_DIRECTORY
#define MESH_DIRECTORY(current_path) TARGET_DIRECTORY(L"XFile/" current_path) 
#endif

#ifndef STAGE_DIRECTORY
#define STAGE_DIRECTORY(current_path) TARGET_DIRECTORY(L"StageScript/" current_path) 
#endif

#ifndef STAGE_DATA_DIRECTORY
#define STAGE_DATA_DIRECTORY(current_path) TARGET_DIRECTORY(L"StageData/" current_path) 
#endif

#ifndef CPU_DIRECTORY
#define CPU_DIRECTORY(current_path) TARGET_DIRECTORY(L"CPUCharactor/" current_path) 
#endif

#ifndef CPU_MECHA_PATH
#define CPU_MECHA_PATH(_fileName) TARGET_DIRECTORY(L"StageScript/CPU/" _fileName)
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
#define TEXT_TO_COLOR_NUMBER(_text)  static_cast<float>(ChStd::BaseNumberToDecimalNumber<wchar_t>(_text, ChStd::HEXA_DECIMAL<wchar_t>())) / 255.0f
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



enum class FrameNo :unsigned char
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

#ifndef PALETTE_COUNT 
#define PALETTE_COUNT 5
#endif

enum class RotateDirectionType
{
	None,
	Vertical,//êÇíºâÒì]//
	Horizontal,//êÖïΩâÒì]//
};

#include"PublicFunctions.h"
