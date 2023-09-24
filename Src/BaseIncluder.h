#pragma once

#include<Windows.h>
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

#ifndef CPU_DIRECTORY
#define CPU_DIRECTORY(current_path) TARGET_DIRECTORY("CPUCharactor/" current_path) 
#endif

#ifndef SAVE_PATH
#define SAVE_PATH(_fileName) TARGET_DIRECTORY("Save/AssemMechaFrame/" _fileName)
#endif

#ifndef GAME_WINDOW_WITDH
#define GAME_WINDOW_WITDH 1280.0f
#endif

#ifndef GAME_WINDOW_HEIGHT
#define GAME_WINDOW_HEIGHT 720.0f
#endif

#ifndef GAME_WINDOW_WITDH_LONG
#define GAME_WINDOW_WITDH_LONG static_cast<unsigned long>(GAME_WINDOW_WITDH)
#endif

#ifndef GAME_WINDOW_HEIGHT_LONG
#define GAME_WINDOW_HEIGHT_LONG static_cast<unsigned long>(GAME_WINDOW_HEIGHT)
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
	Game,
};

struct MeshDrawer
{
	ID3D11DeviceContext* dc = nullptr;
	ChD3D11::Shader::BaseDrawMesh11 drawer;
};

struct PlayerData
{
	std::string stageName;
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

static inline bool IsMoucePosOnSprite(const ChD3D11::Sprite11& _sprite)
{
	ChVec2 lt = _sprite.GetPos(ChD3D11::SpritePositionName::LeftTop);
	ChVec2 rb = _sprite.GetPos(ChD3D11::SpritePositionName::RightDown);

	ChVec2 moucePos = ChWin::Mouse().GetNowProPosToChVec2();

	if (moucePos.x < lt.x || moucePos.x > lt.x + rb.x)return false;
	if (moucePos.y > lt.y || moucePos.y < lt.y + rb.y)return false;

	return true;
}