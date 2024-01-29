
#define __SHADER__
#define _SM5_0_

#include"EffectSpriteShader.hlsli"

In_Geometry main(
	ESS_In_Vertex _vertex
)
{
	In_Geometry res;
	res.pos = float4(_vertex.pos, 1.0f);
	res.animationCount = _vertex.animationCount;
	res.displayFlg = _vertex.displayFlg;
	res.color = _vertex.color;
	return res;
}