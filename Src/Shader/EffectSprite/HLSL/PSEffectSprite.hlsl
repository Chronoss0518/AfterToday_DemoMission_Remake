
#define __SHADER__
#define _SM5_0_

#include"EffectSpriteShader.hlsli"

struct OutColor
{
	float4 color : SV_Target0;
};

OutColor main(In_Pixel _pixel)
{
	OutColor res;
	
#if 0
	float test = _pixel.proPos.z / _pixel.proPos.w;

	res.color = float4(test, test, test, 1.0f);

	return res;
#endif
	
	res.color = GetBaseTextureColor(_pixel.uv) * _pixel.color * baseColor;
	clip(res.color.a - alphaTestNum);
	
	return res;
}
