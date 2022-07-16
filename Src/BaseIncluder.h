#pragma once
#include<Windows.h>
#include<ChBaseLibrary.h>
#include<ChWindowsLibrary.h>
#include<ChDirect3D11Library.h>

#ifndef SOUND_DIRECTORY
#define SOUND_DIRECTORY "../���s/data/Sound/"
#endif

#ifndef EFFECT_FILE
#define EFFECT_FILE "../���s/data/Model.cso"
#endif

#ifndef TEXTURE_DIRECTORY
#define TEXTURE_DIRECTORY(current_path) "../���s/data/Texture/" currernt_path
#endif

#ifndef MESH_DIRECTORY
#define MESH_DIRECTORY(current_path) "../���s/data/XFile/" current_path
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
