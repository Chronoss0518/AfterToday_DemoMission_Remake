
#define __SHADER__
#define _SM5_0_

#include"EffectObjectShader.hlsli"
#include"../../../External/ChCppLibrary/ChCppLibrary/ShaderHeaderFiles/HLSL/5.0/Light.hlsli"


struct OutColor
{
	float4 color : SV_Target0;
	//float depth : SV_DepthGreaterEqual;
};

float Blending(float _alpha);

float3 GetLightColor(OutColor _res, In_Pixel _pixel);

OutColor main(In_Pixel _pixel)
{
	OutColor res;
	//float test = _pixel.proPos.z / _pixel.proPos.w;

	//res.color = float4(test, test, test, 1.0f);

	res.color = baseTex.Sample(baseSmp, _pixel.uv) * _pixel.color;
	clip(res.color.a - alphaTestNum);
	res.color.rgb *= luminescencePower;

	res.color.rgb = GetLightColor(res, _pixel);

	//res.depth = Blending(res.color.a);
	return res;
}

float Blending(float _alpha)
{
	float depth = 0.0f;

	if (blendFlg > 0)
	{
		depth = _alpha > 0.99f ? 0.0f : 1.0f;
	}

	return depth;
}

float3 GetLightColor(OutColor _res, In_Pixel _pixel)
{
	if (lightFlg <= 0)
	{
		return _res.color.rgb;
	}

	L_BaseColor lightStruct;
	lightStruct.color = _res.color.rgb;
	lightStruct.wPos = _pixel.worldPos.xyz;
	lightStruct.wfNormal = _pixel.worldNormal;
	lightStruct.specular = float4(specularPower, 1.0f);

	return GetDirectionalLightColor(lightStruct);

}