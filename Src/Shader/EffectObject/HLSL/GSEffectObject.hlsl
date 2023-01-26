
#define __SHADER__
#define _SM5_0_

#include"EffectObjectShader.hlsli"

[maxvertexcount(6)]
void main(
	point In_Vertex input[1],
	inout TriangleStream<In_Pixel> res
)
{
	if (input[0].displayFlg == 0)
	{
		return;
	}

	float4x4 mat =  mul(projectionMatrix, viewMatrix);

	In_Pixel vertex[4];

	vertex[0].pos = mul(float4(-objectSize.x, objectSize.y, 0.0f, 0.0f) + input[0].pos, mat);
	vertex[1].pos = mul(float4(objectSize.x, objectSize.y, 0.0f, 0.0f) + input[0].pos, mat);
	vertex[2].pos = mul(float4(objectSize.x, -objectSize.y, 0.0f, 0.0f) + input[0].pos, mat);
	vertex[3].pos = mul(float4(-objectSize.x, -objectSize.y, 0.0f, 0.0f) + input[0].pos, mat);

	vertex[0].uv = float2(animationSize.x * input[0].animationCount.x, animationSize.y * input[0].animationCount.y);
	vertex[1].uv = float2(animationSize.x * input[0].animationCount.x, animationSize.y * (input[0].animationCount.y + 1));
	vertex[2].uv = float2(animationSize.x * (input[0].animationCount.x + 1), animationSize.y * (input[0].animationCount.y + 1));
	vertex[3].uv = float2(animationSize.x * (input[0].animationCount.x + 1), animationSize.y * input[0].animationCount.y);

	res.Append(vertex[0]);
	res.Append(vertex[1]);
	res.Append(vertex[2]);
	res.RestartStrip();

	res.Append(vertex[0]);
	res.Append(vertex[2]);
	res.Append(vertex[3]);
	res.RestartStrip();

}