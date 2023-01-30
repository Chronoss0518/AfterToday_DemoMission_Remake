
#define __SHADER__
#define _SM5_0_

#include"EffectObjectShader.hlsli"

[maxvertexcount(6)]
void main(
	point In_Geometry input[1],
	inout TriangleStream<In_Pixel> res
)
{
	if (input[0].displayFlg >= 1) 
	{

		In_Pixel vertex[4];

		int num = 0;
		uint i = 0;
		uint j = 0;

#if 1

		input[0].pos = mul(input[0].pos, viewMatrix);

		vertex[0].pos = float4(-objectSize.x, objectSize.y, 0.0f, 0.0f);
		vertex[1].pos = float4(objectSize.x, objectSize.y, 0.0f, 0.0f);
		vertex[2].pos = float4(objectSize.x, -objectSize.y, 0.0f, 0.0f);
		vertex[3].pos = float4(-objectSize.x, -objectSize.y, 0.0f, 0.0f);

		[unroll]
		for (i = 0; i < 4; i++)
		{
			num = i;
			vertex[num].pos += input[0].pos;
			//vertex[num].pos = mul(vertex[num].pos, viewMatrixInverse);
			vertex[num].pos = mul(vertex[num].pos, projectionMatrix);
		}

		vertex[0].uv = float2(animationSize.x * input[0].animationCount.x, animationSize.y * (input[0].animationCount.y + 1));
		vertex[1].uv = float2(animationSize.x * (input[0].animationCount.x + 1), animationSize.y * (input[0].animationCount.y + 1));
		vertex[2].uv = float2(animationSize.x * (input[0].animationCount.x + 1), animationSize.y * input[0].animationCount.y);
		vertex[3].uv = float2(animationSize.x * input[0].animationCount.x, animationSize.y * input[0].animationCount.y);

#else

		vertex[0].pos = float4(-objectSize.x, objectSize.y, 0.0f, 1.0f);
		vertex[1].pos = float4(objectSize.x, objectSize.y, 0.0f, 1.0f);
		vertex[2].pos = float4(objectSize.x, -objectSize.y, 0.0f, 1.0f);
		vertex[3].pos = float4(-objectSize.x, -objectSize.y, 0.0f, 1.0f);

		vertex[0].uv = float2(0.0f, 1.0f);
		vertex[1].uv = float2(1.0f, 1.0f);
		vertex[2].uv = float2(1.0f, 0.0f);
		vertex[3].uv = float2(0.0f, 0.0f);

#endif

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

}