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
#define USE_CPU_THREAD false
#endif

#ifndef TARGET_DIRECTORY 
//#define	TARGET_DIRECTORY(current_path) "../é¿çs/data/" current_path
#define	TARGET_DIRECTORY(current_path) "Resource/" current_path
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

#ifndef SAVE_PATH
#define SAVE_PATH(_fileName) TARGET_DIRECTORY("Save/AssemMechaFrame/" _fileName)
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

//py = (sy / sh) * -2.0f + 1.0f
#ifndef SCREEN_TO_PROJECTION_Y
#define SCREEN_TO_PROJECTION_Y(value) (static_cast<float>(value)/(GAME_WINDOW_HEIGHT)) * -2.0f + 1.0f
#endif

#ifndef GAME_PROJECTION_NEAR
#define GAME_PROJECTION_NEAR 0.1f
#endif

#ifndef GAME_PROJECTION_FAR
#define GAME_PROJECTION_FAR 10000.0f
#endif

enum class FrameNo :unsigned long
{
	Title,
	Select,
	Game,
	Edit,
	Setting,
};

struct MeshDrawer
{
	ID3D11DeviceContext* dc = nullptr;
	ChD3D11::Shader::BaseDrawMesh11 drawer;
};

struct PlayerData
{
	std::string stageName = "";
	std::string useMechaData = "";
};

static inline void ReleaseMesh11(ChPtr::Shared<ChD3D11::Mesh11>& _meshObject)
{
	if (_meshObject == nullptr)return;
	_meshObject->Release();
	_meshObject = nullptr;
}

static inline ChVec4 RectToGameWindow(const ChVec4 _rect)
{
	ChVec4 res = _rect;

	res.left = res.left / GAME_WINDOW_WIDTH;
	res.left = (res.left * 2.0f) - 1.0f;

	res.right = res.right / GAME_WINDOW_WIDTH;
	res.right = (res.right * 2.0f) - 1.0f;

	res.top = res.top / GAME_WINDOW_HEIGHT;
	res.top = (res.top * -2.0f) + 1.0f;

	res.bottom = res.bottom / GAME_WINDOW_HEIGHT;
	res.bottom = (res.bottom * -2.0f) + 1.0f;

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

	ChVec2 mousePos = ChWin::Mouse().GetNowProPosToChVec2();

	bool res = false;

	if (tmpRect.IsOnPoint(mousePos))res = true;

	return res;
}