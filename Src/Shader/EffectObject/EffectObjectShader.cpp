#include"../../BaseIncluder.h"

#include"EffectObjectShader.h"


void EffectObjectShader::Init(ID3D11Device* _device, const unsigned long _effectNum)
{



}

void EffectObjectShader::InitVertexShader()
{
#include"VSEffectObject.inc"


}

void EffectObjectShader::InitPixelShader()
{
#include"PSEffectObject.inc"


}

void EffectObjectShader::InitGeometryShader()
{
#include"GSEffectObject.inc"


}

void EffectObjectShader::SetViewMatrix(const ChLMat& _viewMat)
{

}

void EffectObjectShader::SetProjectionMatrix(const ChLMat& _viewMat)
{

}

void EffectObjectShader::SetEffectTexture(ChPtr::Shared<ChD3D11::TextureBase11> _effectTexture, const unsigned long _animationCount)
{
	if (_animationCount < 1)return;


	effectTexture = _effectTexture;
}

void EffectObjectShader::DrawStart(ID3D11DeviceContext* _dc)
{

}

void EffectObjectShader::Draw(ID3D11DeviceContext* _dc)
{
	gsBuf.SetToVertexShader(_dc);
}
