#pragma once
#include<Windows.h>
#include<ChBaseLibrary.h>
#include<ChWindowsLibrary.h>
#include<ChDirect3D11Library.h>

#ifndef SOUND_DIRECTORY
#define SOUND_DIRECTORY "../実行/data/Sound/"
#endif

#ifndef EFFECT_FILE
#define EFFECT_FILE "../実行/data/Model.cso"
#endif

#ifndef TEXTURE_DIRECTORY
#define TEXTURE_DIRECTORY(current_path) "../実行/data/Texture/" currernt_path
#endif

#ifndef MESH_DIRECTORY
#define MESH_DIRECTORY(current_path) "../実行/data/XFile/" current_path
#endif


enum class FrameNo :unsigned long
{
	Title,
	Game,
};

namespace ChGame
{
	struct Camera
	{
		ChVec3 pos;
		ChVec3 dir;
		ChVec3 up;
	};

}
