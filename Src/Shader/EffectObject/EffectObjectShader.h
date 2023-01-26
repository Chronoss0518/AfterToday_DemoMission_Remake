#pragma once

#include"HLSL/EffectObjectShader.hlsli"

class EffectObjectShader:public ChD3D11::Shader::SampleShaderBase11
{
public:

	void Init(ID3D11Device* _device, const unsigned long _effectNum);

protected:

	void InitVertexShader()override;

	void InitPixelShader()override;

	void InitGeometryShader()override;

public:

	void SetViewMatrix(const ChLMat& _viewMat);

	void SetProjectionMatrix(const ChLMat& _viewMat);

	//animationCountÇÕ 1à»è„ÇÃêîílÇ≈Ç†ÇÈïKóvÇ™Ç†ÇÈ//
	void SetEffectTexture(ChPtr::Shared<ChD3D11::TextureBase11> _effectTexture,const unsigned long _animationCount = 1);

public:

	void DrawStart(ID3D11DeviceContext* _dc)override;

	void Draw(ID3D11DeviceContext* _dc);

private:

	In_Vertex *effectPosList = nullptr;
	ChPtr::Shared<ChD3D11::TextureBase11> effetTexture = nullptr;
	float2 textureSize;
	GSEffectObjectData gsData;
	ChD3D11::ConstantBuffer11<GSEffectObjectData>gsBuf;
};