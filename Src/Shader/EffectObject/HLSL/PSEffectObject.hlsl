
#define __SHADER__
#define _SM5_0_

#include"EffectObjectShader.hlsli"


float4 main(In_Pixel _pixel) : SV_TARGET
{

	float4 res = float4(1.0f, 1.0f, 1.0f, 1.0f);

	res = baseTex.Sample(baseSmp, _pixel.uv);
	res.rgb *= luminescencePower.rgb;

	return res;
}