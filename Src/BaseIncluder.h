#pragma once
#include<Windows.h>
#include<ChBaseLibrary.h>
#include<ChWindowsLibrary.h>
#include<ChDirect3D11Library.h>

#ifndef SOUND_DIRECTORY
#define SOUND_DIRECTORY(current_path) "../é¿çs/data/Sound/" current_path
#endif

#ifndef TEXTURE_DIRECTORY
#define TEXTURE_DIRECTORY(current_path) "../é¿çs/data/Texture/" current_path
#endif

#ifndef MESH_DIRECTORY
#define MESH_DIRECTORY(current_path) "../é¿çs/data/XFile/" current_path
#endif

enum class FrameNo :unsigned long
{
	Title,
	Game,
};
