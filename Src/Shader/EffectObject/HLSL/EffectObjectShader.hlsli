#ifndef Effect_Object_Shader
#define Effect_Object_Shader


#include"../../../External/ChCppLibrary/ChCppLibrary/ShaderHeaderFiles/HLSL/ShaderPublicInclude.hlsli"

#ifdef __SHADER__

#define S_POSITION(no) :POSITION##no
#define S_TEXCOORD(no) :TEXCOORD##no
#define S_BLENDINDICES(no) :BLENDINDICES##no
#define SV_POSITION :SV_Position

#else

#define S_POSITION(no)
#define S_TEXCOORD(no)
#define S_BLENDINDICES(no)
#define SV_POSITION

using uint2 = ChMath::Vector2Base<unsigned int>;

#endif

struct In_Vertex
{
	float3 pos S_POSITION(0);
	uint2 animationCount S_TEXCOORD(0);
	uint displayFlg  S_TEXCOORD(1);
};

#ifdef __SHADER__

struct In_Geometry
{
	float4 pos SV_POSITION;
	uint2 animationCount S_TEXCOORD(0);
	uint displayFlg  S_TEXCOORD(1);
};

struct In_Pixel
{
	float4 pos SV_POSITION;
	float2 uv S_TEXCOORD(0);
};

#endif

#ifndef EFFECT_OBJECT_GEOMETRY_DATA
#define EFFECT_OBJECT_GEOMETRY_DATA 0
#endif

#ifndef EFFECT_OBJECT_PIXEL_DATA
#define EFFECT_OBJECT_PIXEL_DATA 1
#endif

#ifndef EFFECT_OBJECT_PIXEL_TEXTURE
#define EFFECT_OBJECT_PIXEL_TEXTURE 0
#endif

#ifdef __SHADER__
#ifdef _SM5_0_
cbuffer GSEffectObjectData :register(CHANGE_CBUFFER_5(EFFECT_OBJECT_GEOMETRY_DATA))
#else
cbuffer GSEffectObjectData : register(b[EFFECT_OBJECT_GEOMETRY_DATA])
#endif
#else
struct GSEffectObjectData
#endif
{
	row_major float4x4 viewMatrix;
	row_major float4x4 projectionMatrix;
	float2 objectSize;
	float2 animationSize;
};

#ifdef __SHADER__
#ifdef _SM5_0_
cbuffer PSEffectObjectData :register(CHANGE_CBUFFER_5(EFFECT_OBJECT_PIXEL_DATA))
#else
cbuffer PSEffectObjectData : register(b[EFFECT_OBJECT_PIXEL_DATA])
#endif
#else
struct PSEffectObjectData
#endif
{
	float4 luminescencePower;
};

#ifdef __SHADER__

#ifdef _SM5_0_
texture2D baseTex :register(CHANGE_TBUFFER_5(EFFECT_OBJECT_PIXEL_TEXTURE));

//画像から1ピクセルの色を取得するための物//
sampler baseSmp:register(CHANGE_SBUFFER_5(EFFECT_OBJECT_PIXEL_TEXTURE));

#else
texture2D baseTex : register(t[EFFECT_OBJECT_PIXEL_TEXTURE]);

//画像から1ピクセルの色を取得するための物//
sampler baseSmp : register(s[EFFECT_OBJECT_PIXEL_TEXTURE]);

#endif
#endif

#endif