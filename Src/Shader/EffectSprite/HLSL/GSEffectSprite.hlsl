
#define __SHADER__
#define _SM5_0_

#include"EffectSpriteShader.hlsli"

[maxvertexcount(6)]
void main(
	point In_Geometry input[1],
	inout TriangleStream<In_Pixel> res
)
{
	if (input[0].displayFlg < 1) 
	{
		return;
	}

	if (input[0].color.a <= alphaTestNum)
	{
		return;
	}

	In_Pixel vertex[4];

	int num = 0;
	uint i = 0;
	uint j = 0;
	
	float3 basePos[4] =
	{
        float3(-objectSize.x + input[0].pos.x, objectSize.y + input[0].pos.y, 0.0f),
		float3(objectSize.x + input[0].pos.x, objectSize.y + input[0].pos.y, 0.0f),
		float3(objectSize.x + input[0].pos.x, -objectSize.y + input[0].pos.y, 0.0f),
		float3(-objectSize.x + input[0].pos.x, -objectSize.y + input[0].pos.y, 0.0f)
    };

	[unroll]
	for (i = 0; i < 4; i++)
	{
		num = i;
        vertex[num].pos = float4(basePos[num],1.0f);
		vertex[num].color = input[0].color;
	}

	vertex[0].uv = float2(animationSize.x * input[0].animationCount.x, animationSize.y * (input[0].animationCount.y + 1));
	vertex[1].uv = float2(animationSize.x * (input[0].animationCount.x + 1), animationSize.y * (input[0].animationCount.y + 1));
	vertex[2].uv = float2(animationSize.x * (input[0].animationCount.x + 1), animationSize.y * input[0].animationCount.y);
	vertex[3].uv = float2(animationSize.x * input[0].animationCount.x, animationSize.y * input[0].animationCount.y);

	[unroll]
	for (i = 0; i < 2; i++)
	{
		res.Append(vertex[0]);
		[unroll]
		for (j = 0; j < 2; j++)
		{
			num = i + j + 1;
			res.Append(vertex[num]);
		}
		res.RestartStrip();
	}

}
