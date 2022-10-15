#pragma once
#include<Windows.h>
#include<ChBaseLibrary.h>
#include<ChWindowsLibrary.h>
#include<ChDirect3D11Library.h>

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

enum class FrameNo :unsigned long
{
	Title,
	Game,
};
