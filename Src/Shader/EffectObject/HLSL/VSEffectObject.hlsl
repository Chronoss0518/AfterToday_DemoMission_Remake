
#define __SHADER__
#define _SM5_0_

#include"EffectObjectShader.hlsli"

In_Geometry main(
	In_Vertex _vertex
)
{
	In_Geometry res;
	res.pos = float4(_vertex.pos, 1.0f);
	res.animationCount = _vertex.animationCount;
	res.displayFlg = _vertex.displayFlg;
	return res;
}