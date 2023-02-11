#include"../../BaseIncluder.h"

#include"TextureEffectShader.h"


void TextureEffectShader::Init(ID3D11Device* _device)
{
	if (ChPtr::NullCheck(_device))return;

	Release();

	ChD3D11::Shader::SampleShaderBase11::Init(_device);

	ChD3D11::Shader::SampleShaderBase11::SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	D3D11_RASTERIZER_DESC desc
	{
		D3D11_FILL_MODE::D3D11_FILL_SOLID,
		D3D11_CULL_MODE::D3D11_CULL_NONE,
		true,
		0,
		0.0f,
		0.0f,
		true,
		false,
		true,
		false
	};

	ChD3D11::Shader::SampleShaderBase11::CreateRasteriser(desc);


}

void TextureEffectShader::Release()
{


}

void TextureEffectShader::InitVertexShader()
{

}

void TextureEffectShader::InitPixelShader()
{

}

void TextureEffectShader::DrawAdd(ID3D11DeviceContext* _dc, ChD3D11::TextureBase11& _base, ChD3D11::TextureBase11& _addTexture)
{

	if (ChPtr::NullCheck(_dc))return;
	if (ChPtr::NullCheck(&_base))return;
	if (ChPtr::NullCheck(&_addTexture))return;
	if (!_base.IsTex())return;
	if (!_addTexture.IsTex())return;

	ChD3D11::Shader::SampleShaderBase11::SetShaderBlender(_dc);

	ChD3D11::Shader::SampleShaderBase11::SetShaderDefaultBlender(_dc);
}

void TextureEffectShader::DrawSub(ID3D11DeviceContext* _dc, ChD3D11::TextureBase11& _base, ChD3D11::TextureBase11& _subtractionTexture)
{

	if (ChPtr::NullCheck(_dc))return;
	if (ChPtr::NullCheck(&_base))return;
	if (ChPtr::NullCheck(&_subtractionTexture))return;
	if (!_base.IsTex())return;
	if (!_subtractionTexture.IsTex())return;

	ChD3D11::Shader::SampleShaderBase11::SetShaderBlender(_dc);

	ChD3D11::Shader::SampleShaderBase11::SetShaderDefaultBlender(_dc);
}

void TextureEffectShader::Update(ID3D11DeviceContext* _dc)
{

}
