#pragma once

class TextureEffectShader:public ChD3D11::Shader::SampleShaderBase11
{

public:

	void Init(ID3D11Device* _device);

	void Release();

protected:
	
	void InitVertexShader()override;

	void InitPixelShader()override;
	
protected:

	void Update(ID3D11DeviceContext* _dc)override;

public:

	void DrawAdd(ID3D11DeviceContext* _dc,ChD3D11::TextureBase11& _base, ChD3D11::TextureBase11& _addTexture);

	void DrawSub(ID3D11DeviceContext* _dc, ChD3D11::TextureBase11& _base, ChD3D11::TextureBase11& _subtractionTexture);
};