#ifndef Effect_Object_Shader
#define Effect_Object_Shader

#include"../../../External/ChCppLibrary/ChCppLibrary/ShaderHeaderFiles/HLSL/ShaderPublicInclude.hlsli"

#ifdef __SHADER__

#define Se_POSITION(no) :POSITION##no
#define Se_TEXCOORD(no) :TEXCOORD##no
#define Se_BLENDINDICES(no) :BLENDINDICES##no
#define SV_POSITION :SV_Position
#define Se_COLOR(no) :COLOR##no

#define Se_NORMAL(no) :NORMAL##no
#define Se_TANGENT(no) :TANGENT##no
#define Se_BINORMAL(no) :BINORMAL##no

#else

#define Se_POSITION(no)
#define Se_TEXCOORD(no)
#define Se_BLENDINDICES(no)
#define SV_POSITION
#define Se_COLOR(no)

#define Se_NORMAL(no)
#define Se_TANGENT(no)
#define Se_BINORMAL(no)

using uint2 = ChMath::Vector2Base<unsigned int>;

#endif

struct In_Vertex
{
	float3 pos Se_POSITION(0);
	float3 v_normal Se_NORMAL(0);
	float3 v_binormal Se_BINORMAL(0);
	float3 v_tangent Se_TANGENT(0);
	float4 color Se_COLOR(0);
	uint2 animationCount Se_TEXCOORD(0);
	uint displayFlg  Se_TEXCOORD(1);
};

#ifdef __SHADER__

struct In_Geometry
{
	float4 pos SV_POSITION;
	float3 v_normal Se_NORMAL(0);
	float3 v_binormal Se_BINORMAL(0);
	float3 v_tangent Se_TANGENT(0);
	float4 color Se_COLOR(0);
	uint2 animationCount Se_TEXCOORD(0);
	uint displayFlg  Se_TEXCOORD(1);
};

struct In_Pixel
{
	float4 pos SV_POSITION;
	float4 color Se_COLOR(0);
	float3 worldNormal Se_NORMAL(0);
	float2 uv Se_TEXCOORD(0);
	float4 worldPos Se_TEXCOORD(1);
	float4 proPos Se_TEXCOORD(2);
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
	float luminescencePower = 1.0f;
	float alphaTestNum = 0.01f;
	uint blendFlg = 0;
	uint lightFlg = 0;
	float3 specularPower = float3(0.0f, 0.0f, 0.0f);
	float tmp = 0.0f;
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