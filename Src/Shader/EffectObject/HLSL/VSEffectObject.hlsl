
#define __SHADER__
#define _SM5_0_

#include"EffectObjectShader.hlsli"

In_Geometry main(
	EOS_In_Vertex _vertex
)
{
	In_Geometry res;
	res.pos = float4(_vertex.pos, 1.0f);
	res.animationCount = _vertex.animationCount;
	res.displayFlg = _vertex.displayFlg;
	res.v_normal = _vertex.v_normal;
	res.v_binormal = _vertex.v_binormal;
	res.v_tangent = _vertex.v_tangent;
	res.color = _vertex.color;
	return res;
}