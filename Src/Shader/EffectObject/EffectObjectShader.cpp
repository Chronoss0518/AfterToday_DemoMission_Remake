#include"../../BaseIncluder.h"

#include"EffectObjectShader.h"


void EffectObjectShader::Init(ID3D11Device* _device, const unsigned long _maxEffectNum)
{
	if (_maxEffectNum <= 0)return;
	if (ChPtr::NullCheck(_device))return;

	Release();

	ChD3D11::Shader::SampleShaderBase11::Init(_device);

	gsData.objectSize = ChVec2(1.0f);
	psData.luminescencePower = 1.0f;
	effectPosList.resize(_maxEffectNum);

	{
		ChLMat mat = ChLMat();

		for (unsigned long i = 0; i < effectPosList.size(); i++)
		{
			effectPosList[i].displayFlg = false;
			effectPosList[i].v_normal = mat.GetZAxisDirection();
			effectPosList[i].v_binormal = mat.GetYAxisDirection();
			effectPosList[i].v_tangent = mat.GetXAxisDirection();
			effectPosList[i].color = ChVec4(1.0f);

		}
	}

	vb.Init();
	vb.CreateBuffer(_device, &effectPosList[0], effectPosList.size());
	gsBuf.Init();
	gsBuf.CreateBuffer(_device, EFFECT_OBJECT_GEOMETRY_DATA);
	psBuf.Init();
	psBuf.CreateBuffer(_device, EFFECT_OBJECT_PIXEL_DATA);

	psData.blendFlg = false;

	ChD3D11::Shader::SampleShaderBase11::SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	{

		D3D11_DEPTH_STENCIL_DESC desc = {
			true,
			D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO ,
			D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS,
			false,
			D3D11_DEFAULT_STENCIL_READ_MASK,
			D3D11_DEFAULT_STENCIL_WRITE_MASK,
			D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP,
			D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP
		};

		ChD3D11::Shader::SampleShaderBase11::CreateDepthStencilTester(desc);
	}
}

void EffectObjectShader::Release()
{
	ChD3D11::Shader::SampleShaderBase11::Release();

	effectPosList.clear();
	effectTexture = nullptr;
	vb.Release();
	gsBuf.Release();
	psBuf.Release();
}

void EffectObjectShader::InitVertexShader()
{
#include"VSEffectObject.inc"

	D3D11_INPUT_ELEMENT_DESC decl[7];

	decl[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA };
	decl[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA };
	decl[2] = { "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA };
	decl[3] = { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA };
	decl[4] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA };
	decl[5] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_UINT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA };
	decl[6] = { "TEXCOORD", 1, DXGI_FORMAT_R32_UINT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA };

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

void EffectObjectShader::SetUseDepthStencilTestFlg(bool _flg)
{
	useDepthStencilTestFlg = _flg;
	rasterizerUpdateFlg = true;
}

void EffectObjectShader::SetAlphaBlendTestFlg(bool _flg)
{
	alphaBlendTestFlg = _flg;
}

void EffectObjectShader::SetViewMatrix(const ChLMat& _viewMat)
{
	gsData.viewMatrix = _viewMat;
	gsUpdateFlg = true;
}

void EffectObjectShader::SetProjectionMatrix(const ChLMat& _projectionMat)
{
	gsData.projectionMatrix = _projectionMat;
	gsUpdateFlg = true;
}

void EffectObjectShader::SetObjectSize(const ChVec2& _objectSize)
{
	gsData.objectSize = _objectSize;
	gsUpdateFlg = true;
}

void EffectObjectShader::SetLuminescencePower(const float _power)
{
	psData.luminescencePower = _power;
	psUpdateFlg = true;
}

void EffectObjectShader::SetAlphaTestNum(const float _num)
{
	psData.alphaTestNum = _num;
	psUpdateFlg = true;
}

void EffectObjectShader::SetBlendFlg(const bool& _flg)
{
	psData.blendFlg = _flg;
	psUpdateFlg = true;
}

void EffectObjectShader::SetSpecularColor(const ChVec3& _color)
{
	psData.specularPower = _color;
	psUpdateFlg = true;
}

void EffectObjectShader::SetLightFlg(const bool& _flg)
{
	psData.lightFlg = _flg;
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
	if (effectPosList.size() <= _effectCount)return;

	effectPosList[_effectCount].pos = _pos;

	vbUpdateFlg = true;
}

void EffectObjectShader::SetEffectVertexRotation(const ChLMat& _rotateMatrix, const unsigned long _effectCount)
{
	if (effectPosList.size() <= _effectCount)return;


	effectPosList[_effectCount].v_normal = _rotateMatrix.GetZAxisDirection();
	effectPosList[_effectCount].v_binormal = _rotateMatrix.GetYAxisDirection();
	effectPosList[_effectCount].v_tangent = _rotateMatrix.GetXAxisDirection();

	vbUpdateFlg = true;
}

void EffectObjectShader::SetEffectColor(const ChVec4& _effectColor, const unsigned long _effectCount)
{
	if (effectPosList.size() <= _effectCount)return;

	effectPosList[_effectCount].color = _effectColor;

	vbUpdateFlg = true;
}

void EffectObjectShader::SetEffectColor(const ChVec3& _effectColor, const unsigned long _effectCount)
{
	if (effectPosList.size() <= _effectCount)return;

	ChVec4 color = _effectColor;
	color.a = effectPosList[_effectCount].color.a;

	SetEffectColor(color, _effectCount);
}

void EffectObjectShader::SetEffectAnimationCount(const ChMath::Vector2Base<unsigned long>& _animationCount, const unsigned long _effectCount)
{
	if (effectPosList.size() <= _effectCount)return;

	effectPosList[_effectCount].animationCount.val = _animationCount.val;

	vbUpdateFlg = true;
}

void EffectObjectShader::SetEffectHorizontalAnimationCount(const unsigned long _animationCount, const unsigned long _effectCount)
{
	if (effectPosList.size() <= _effectCount)return;

	effectPosList[_effectCount].animationCount.x = _animationCount;

	vbUpdateFlg = true;
}

void EffectObjectShader::SetEffectVerticalAnimationCount(const unsigned long _animationCount, const unsigned long _effectCount)
{
	if (effectPosList.size() <= _effectCount)return;

	effectPosList[_effectCount].animationCount.y = _animationCount;

	vbUpdateFlg = true;
}

void EffectObjectShader::SetEffectDisplayFlg(const bool& _flg, const unsigned long _effectCount)
{
	if (effectPosList.size() <= _effectCount)return;

	effectPosList[_effectCount].displayFlg = _flg;

	vbUpdateFlg = true;
}

void EffectObjectShader::Draw(ID3D11DeviceContext* _dc)
{
	if (ChPtr::NullCheck(_dc))return;
	if (effectTexture == nullptr)return;

	if (alphaBlendTestFlg)
	{
		float factor[] = { 0.1f,0.1f,0.1f,0.1f };
		ChD3D11::Shader::SampleShaderBase11::SetShaderBlender(_dc, factor);
	}
	if (!useDepthStencilTestFlg)ChD3D11::Shader::SampleShaderBase11::SetShaderDepthStencilTester(_dc);

	gsBuf.SetToGeometryShader(_dc);
	psBuf.SetToPixelShader(_dc);
	effectTexture->SetDrawData(_dc, EFFECT_OBJECT_PIXEL_TEXTURE);

	vb.SetVertexBuffer(_dc, 0);
	_dc->Draw(effectPosList.size(), 0);

	if (!useDepthStencilTestFlg)ChD3D11::Shader::SampleShaderBase11::SetShaderDefaultDepthStencilTester(_dc);

	if (alphaBlendTestFlg)ChD3D11::Shader::SampleShaderBase11::SetShaderDefaultBlender(_dc);

}

void EffectObjectShader::Update(ID3D11DeviceContext* _dc)
{
	if (vbUpdateFlg)
	{
		vb.UpdateResouce(_dc, &effectPosList[0]);
		vbUpdateFlg = false;
	}

	if (gsUpdateFlg)
	{
		gsBuf.UpdateResouce(_dc, &gsData);
		gsUpdateFlg = false;
	}

	if (psUpdateFlg)
	{
		psBuf.UpdateResouce(_dc, &psData);
		psUpdateFlg = false;
	}


	if(rasterizerUpdateFlg){

		D3D11_RASTERIZER_DESC desc
		{
			D3D11_FILL_MODE::D3D11_FILL_SOLID,
			D3D11_CULL_MODE::D3D11_CULL_NONE,
			true,
			0,
			0.0f,
			0.0f,
			useDepthStencilTestFlg,
			false,
			true,
			false
		};

		ChD3D11::Shader::SampleShaderBase11::CreateRasteriser(desc);
		rasterizerUpdateFlg = false;
	}

}
