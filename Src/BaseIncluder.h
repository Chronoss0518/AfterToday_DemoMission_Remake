#pragma once
#include<Windows.h>
#include<ChBaseLibrary.h>
#include<ChWindowsLibrary.h>
#include<ChDirect3D11Library.h>
#include"Physics/PhysicsMachine.h"

#define BETA

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
