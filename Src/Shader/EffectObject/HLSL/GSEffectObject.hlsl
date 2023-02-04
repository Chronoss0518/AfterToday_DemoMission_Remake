
#define __SHADER__
#define _SM5_0_

#include"EffectObjectShader.hlsli"

float3 VertexRotation(float3 _pos, float3x3 _mat);

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

		float3x3 vertexRotateMatrix = float3x3
		(
			input[0].v_tangent.x,input[0].v_binormal.x,input[0].v_normal.x,
			input[0].v_tangent.y,input[0].v_binormal.y,input[0].v_normal.y,
			input[0].v_tangent.z,input[0].v_binormal.z,input[0].v_normal.z
		);

		float3 basePos[4] =
		{
			float3(-objectSize.x, objectSize.y, 0.0f),
			float3(objectSize.x, objectSize.y, 0.0f),
			float3(objectSize.x, -objectSize.y, 0.0f),
			float3(-objectSize.x, -objectSize.y, 0.0f)
		};

		input[0].pos = mul(input[0].pos, viewMatrix);


		[unroll]
		for (i = 0; i < 4; i++)
		{
			num = i;
			vertex[num].worldPos = float4(VertexRotation(basePos[num], vertexRotateMatrix), 0.0f);
			vertex[num].pos = vertex[num].worldPos += input[0].pos;
			vertex[num].proPos = vertex[num].pos = mul(vertex[num].pos, projectionMatrix);
			vertex[num].color = input[0].color;
		}

		int tmpNum1 = 0;
		int tmpNum2 = 0;
		[unroll]
		for (i = 0; i < 4; i++)
		{
			num = i;
			tmpNum1 = (num + 1) % 4;
			tmpNum2 = (num + 3) % 4;
			float3 normal = float3(vertex[tmpNum1].worldPos.xyz - vertex[num].worldPos.xyz) + float3(vertex[tmpNum2].worldPos.xyz - vertex[num].worldPos.xyz);
			vertex[num].worldNormal = normalize(-normal) ;
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

}

float3 VertexRotation(float3 _pos, float3x3 _mat)
{
	return mul(_pos, _mat);
}
