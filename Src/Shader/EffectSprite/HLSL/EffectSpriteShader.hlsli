#ifndef Effect_Sprite_Shader
#define Effect_Sprite_Shader

#include"../../../External/ChCppLibrary/ChCppLibrary/ShaderHeaderFiles/HLSL/ShaderPublicInclude.hlsli"
#include"../../../External/ChCppLibrary/ChCppLibrary/ShaderHeaderFiles/HLSL/5.0/Texture/BaseTexture.hlsli"

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

struct ESS_In_Vertex
{
	float3 pos Se_POSITION(0);
	float4 color Se_COLOR(0);
	uint2 animationCount Se_TEXCOORD(0);
	uint displayFlg  Se_TEXCOORD(1);
};

#ifdef __SHADER__

struct In_Geometry
{
	float4 pos SV_POSITION;
	float4 color Se_COLOR(0);
	uint2 animationCount Se_TEXCOORD(0);
	uint displayFlg  Se_TEXCOORD(1);
};

struct In_Pixel
{
	float4 pos SV_POSITION;
	float4 color Se_COLOR(0);
	float2 uv Se_TEXCOORD(0);
};

#endif

#ifndef EFFECT_SPRITE_GEOMETRY_DATA
#define EFFECT_SPRITE_GEOMETRY_DATA 0
#endif

#ifndef EFFECT_SPRITE_PIXEL_DATA
#define EFFECT_SPRITE_PIXEL_DATA 1
#endif

#ifdef __SHADER__
cbuffer GSEffectSpriteData :register(CHANGE_CBUFFER(EFFECT_SPRITE_GEOMETRY_DATA))
#else
struct GSEffectSpriteData
#endif
{
	float2 objectSize;
	float2 animationSize;
    float alphaTestNum = 0.1f;
    float3 EODtmp;
};


#ifdef __SHADER__
cbuffer PSEffectSpriteData :register(CHANGE_CBUFFER(EFFECT_SPRITE_PIXEL_DATA))
#else
struct PSEffectSpriteData
#endif
{
    float4 baseColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
};

#endif