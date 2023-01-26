#include"../../BaseIncluder.h"

#include"EffectObjectShader.h"


void EffectObjectShader::Init(ID3D11Device* _device, const unsigned long _maxEffectNum)
{
	if (_maxEffectNum <= 0)return;
	if (ChPtr::NullCheck(_device))return;

	Release();

	ChD3D11::Shader::SampleShaderBase11::Init(ChD3D11::Shader::SampleShaderBase11::GetDevice());

	maxEffectCount = _maxEffectNum;
	effectPosList = new In_Vertex[maxEffectCount];
	indexNum = new unsigned long[maxEffectCount];

	for (unsigned long i = 0; i < maxEffectCount; i++)
	{
		effectPosList[i].displayFlg = false;
		indexNum[i] = i;
	}

	vb.CreateBuffer(_device, effectPosList, maxEffectCount);
	ib.CreateBuffer(_device, indexNum, maxEffectCount);
	gsBuf.CreateBuffer(_device, EFFECT_OBJECT_GEOMETRY_DATA);
	psBuf.CreateBuffer(_device, EFFECT_OBJECT_PIXEL_DATA);

	D3D11_RASTERIZER_DESC desc
	{
		D3D11_FILL_MODE::D3D11_FILL_SOLID,
		D3D11_CULL_MODE::D3D11_CULL_NONE,
		true,
		0,
		0.0f,
		0.0f,
		false,
		false,
		true,
		false
	};

	ChD3D11::Shader::SampleShaderBase11::CreateRasteriser(desc);
}

void EffectObjectShader::Release()
{
	ChD3D11::Shader::SampleShaderBase11::Release();

	if (ChPtr::NotNullCheck(effectPosList))
	{
		delete[] effectPosList;
		effectPosList = nullptr;
	}
	effectTexture = nullptr;
	vb.Release();
	ib.Release();
	gsBuf.Release();
	psBuf.Release();
	gsUpdateFlg = true;

}

void EffectObjectShader::InitVertexShader()
{
#include"VSEffectObject.inc"


	D3D11_INPUT_ELEMENT_DESC decl[3];

	decl[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA };
	decl[1] = { "BLENDINDICES", 0, DXGI_FORMAT_R32G32_UINT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA };
	decl[2] = { "BLENDINDICES", 1, DXGI_FORMAT_R32_UINT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA };

	ChD3D11::Shader::SampleShaderBase11::CreateVertexShader(decl, sizeof(decl) / sizeof(D3D11_INPUT_ELEMENT_DESC), main, sizeof(main));
}

void EffectObjectShader::InitPixelShader()
{
#include"PSEffectObject.inc"


	ChD3D11::Shader::SampleShaderBase11::CreatePixelShader(main, sizeof(main));
}

void EffectObjectShader::InitGeometryShader()
{
#include"GSEffectObject.inc"

	ChD3D11::Shader::SampleShaderBase11::CreateGeometryShader(main, sizeof(main));

}

void EffectObjectShader::SetViewMatrix(const ChLMat& _viewMat)
{
	gsData.viewMatrix = _viewMat;
	gsData.viewMatrix.SetPosition(0.0f);
	gsData.viewMatrix.Inverse();
	gsData.viewMatrix = _viewMat * gsData.viewMatrix;

	gsUpdateFlg = true;
}

void EffectObjectShader::SetProjectionMatrix(const ChLMat& _projectionMat)
{
	gsData.viewMatrix = _projectionMat;

	gsUpdateFlg = true;
}

void EffectObjectShader::SetLuminescencePower(const float _power)
{
	SetLuminescencePower(ChVec4(_power));
}

void EffectObjectShader::SetLuminescencePower(const ChVec4& _power)
{
	psData.luminescencePower = _power;

	psUpdateFlg = true;
}

void EffectObjectShader::SetEffectTexture(ChPtr::Shared<ChD3D11::TextureBase11> _effectTexture, const ChMath::Vector2Base<unsigned long>& _animationCount)
{
	if (_effectTexture == nullptr)return;
	if (!_effectTexture->IsTex())return;

	gsData.animationSize.val = _animationCount.val;
	if (gsData.animationSize.x < 1)gsData.animationSize.x = 1;
	if (gsData.animationSize.y < 1)gsData.animationSize.y = 1;
	effectTexture = _effectTexture;

	{
		D3D11_SAMPLER_DESC desc = effectTexture->GetSamplerDesc();
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		effectTexture->SetSampler(desc);
	}

	gsData.animationSize.x = static_cast<float>(1.0f / gsData.animationSize.x);
	gsData.animationSize.y = static_cast<float>(1.0f / gsData.animationSize.y);

}

void EffectObjectShader::SetEffectTexture(ChPtr::Shared<ChD3D11::TextureBase11> _effectTexture, const unsigned long _horizontalAnimationCount, const unsigned long _verticalAnimationCount)
{
	SetEffectTexture(_effectTexture, ChMath::Vector2Base<unsigned long>(_horizontalAnimationCount, _verticalAnimationCount));
}

void EffectObjectShader::SetEffectTexture(const std::string& _texturePath, const ChMath::Vector2Base<unsigned long>& _animationCount)
{
	ChPtr::Shared<ChD3D11::Texture11> texture = ChPtr::Make_S<ChD3D11::Texture11>();
	texture->CreateTexture(_texturePath, ChD3D11::Shader::SampleShaderBase11::GetDevice());

	SetEffectTexture(texture, _animationCount);
}

void EffectObjectShader::SetEffectTexture(const std::string& _texturePath, const unsigned long _horizontalAnimationCount, const unsigned long _verticalAnimationCount)
{
	SetEffectTexture(_texturePath, ChMath::Vector2Base<unsigned long>(_horizontalAnimationCount, _verticalAnimationCount));
}

void EffectObjectShader::SetEffectTexture(const std::wstring& _texturePath, const ChMath::Vector2Base<unsigned long>& _animationCount)
{
	ChPtr::Shared<ChD3D11::Texture11> texture = ChPtr::Make_S<ChD3D11::Texture11>();
	texture->CreateTexture(_texturePath, ChD3D11::Shader::SampleShaderBase11::GetDevice());

	SetEffectTexture(texture, _animationCount);

}

void EffectObjectShader::SetEffectTexture(const std::wstring& _texturePath, const unsigned long _horizontalAnimationCount, const unsigned long _verticalAnimationCount)
{
	SetEffectTexture(_texturePath, ChMath::Vector2Base<unsigned long>(_horizontalAnimationCount, _verticalAnimationCount));
}

void EffectObjectShader::SetEffectPosition(const ChVec3& _pos, const unsigned long _effectCount)
{
	if (maxEffectCount <= _effectCount)return;

	effectPosList[_effectCount].pos = _pos;

	vbUpdateFlg = true;
}

void EffectObjectShader::SetEffectAnimationCount(const ChMath::Vector2Base<unsigned long>& _animationCount, const unsigned long _effectCount)
{
	if (maxEffectCount <= _effectCount)return;

	effectPosList[_effectCount].animationCount.val = _animationCount.val;

	vbUpdateFlg = true;
}

void EffectObjectShader::SetEffectHorizontalAnimationCount(const unsigned long _animationCount, const unsigned long _effectCount)
{
	if (maxEffectCount <= _effectCount)return;

	effectPosList[_effectCount].animationCount.x = _animationCount;

	vbUpdateFlg = true;
}

void EffectObjectShader::SetEffectVerticalAnimationCount(const unsigned long _animationCount, const unsigned long _effectCount)
{
	if (maxEffectCount <= _effectCount)return;

	effectPosList[_effectCount].animationCount.y = _animationCount;

	vbUpdateFlg = true;
}

void EffectObjectShader::SetEffectDisplayFlg(const ChStd::Bool& _flg, const unsigned long _effectCount)
{
	if (maxEffectCount <= _effectCount)return;

	effectPosList[_effectCount].displayFlg = _flg;

	vbUpdateFlg = true;
}

void EffectObjectShader::Draw(ID3D11DeviceContext* _dc)
{
	if (ChPtr::NullCheck(_dc))return;

	ChD3D11::Shader::SampleShaderBase11::SetShaderBlender(_dc);

	gsBuf.SetToVertexShader(_dc);
	effectTexture->SetDrawData(_dc, EFFECT_OBJECT_PIXEL_TEXTURE);

	vb.SetVertexBuffer(_dc, 0);
	ib.SetIndexBuffer(_dc);
	_dc->DrawIndexed(maxEffectCount, 0, 0);

	ChD3D11::Shader::SampleShaderBase11::SetShaderDefaultBlender(_dc);
}

void EffectObjectShader::Update(ID3D11DeviceContext* _dc)
{
	if (vbUpdateFlg)
	{
		vb.UpdateResouce(_dc, effectPosList);
		vbUpdateFlg = false;
	}

	if (gsUpdateFlg)
	{
		gsBuf.UpdateResouce(_dc, &gsData);

		gsUpdateFlg = false;
	}

	if (psUpdateFlg)
	{
		gsBuf.UpdateResouce(_dc, &psData);

		psUpdateFlg = false;
	}

}
