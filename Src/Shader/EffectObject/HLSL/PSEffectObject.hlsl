
#define __SHADER__
#define _SM5_0_

#include"EffectObjectShader.hlsli"
#include"../../../External/ChCppLibrary/ChCppLibrary/ShaderHeaderFiles/HLSL/5.0/Light.hlsli"


struct OutColor
{
	float4 color : SV_Target0;
};

void FrustumCulling(float4 _pos);

float Blending(float _alpha);

float3 GetLightColor(OutColor _res, In_Pixel _pixel);

OutColor main(In_Pixel _pixel)
{
	OutColor res;

#if 0
	float test = _pixel.proPos.z / _pixel.proPos.w;

	res.color = float4(test, test, test, 1.0f);

	return res;
#endif
	
	FrustumCulling(_pixel.proPos);

	res.color = baseTex.Sample(baseSmp, _pixel.uv) * _pixel.color;
	clip(res.color.a - alphaTestNum);
	res.color.rgb *= luminescencePower;

	res.color.rgb = GetLightColor(res, _pixel);

	return res;
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

void FrustumCulling(float4 _pos)
{
	float x = ((_pos.x / _pos.w) + 1.0f) * 0.5f;
	float y = ((_pos.y / _pos.w) + 1.0f) * 0.5f;
	float z = (_pos.z / _pos.w);
	clip(1.0f - x);
	clip(1.0f - y);
	clip(1.0f - z);
}