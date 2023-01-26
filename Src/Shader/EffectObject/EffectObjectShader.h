#pragma once

#include"HLSL/EffectObjectShader.hlsli"

class EffectObjectShader:public ChD3D11::Shader::SampleShaderBase11
{
public:

	void Init(ID3D11Device* _device, const unsigned long _maxEffectNum);

	void Release()override;

protected:

	void InitVertexShader()override;

	void InitPixelShader()override;

	void InitGeometryShader()override;

public:

	void SetViewMatrix(const ChLMat& _viewMat);

	void SetProjectionMatrix(const ChLMat& _projectionMat);

	void SetLuminescencePower(const float _power);

	void SetLuminescencePower(const ChVec4& _power);

	//animationCountÇÕ 1à»è„ÇÃêîílÇ≈Ç†ÇÈïKóvÇ™Ç†ÇÈ//
	void SetEffectTexture(ChPtr::Shared<ChD3D11::TextureBase11> _effectTexture, const ChMath::Vector2Base<unsigned long>& _animationCount);

	void SetEffectTexture(ChPtr::Shared<ChD3D11::TextureBase11> _effectTexture, const unsigned long _horizontalAnimationCount, const unsigned long _verticalAnimationCount);

	void SetEffectTexture(const std::string& _texturePath, const ChMath::Vector2Base<unsigned long>& _animationCount);

	void SetEffectTexture(const std::string& _texturePath, const unsigned long _horizontalAnimationCount, const unsigned long _verticalAnimationCount);

	void SetEffectTexture(const std::wstring& _texturePath, const ChMath::Vector2Base<unsigned long>& _animationCount);

	void SetEffectTexture(const std::wstring& _texturePath, const unsigned long _horizontalAnimationCount, const unsigned long _verticalAnimationCount);

	void SetEffectPosition(const ChVec3& _pos, const unsigned long _effectCount);

	void SetEffectAnimationCount(const ChMath::Vector2Base<unsigned long>& _animationCount, const unsigned long _effectCount);

	void SetEffectHorizontalAnimationCount(const unsigned long _animationCount, const unsigned long _effectCount);

	void SetEffectVerticalAnimationCount(const unsigned long _animationCount, const unsigned long _effectCount);

	void SetEffectDisplayFlg(const ChStd::Bool& _flg, const unsigned long _effectCount);

public:

	unsigned long GetMaxEffectCount() { return maxEffectCount; }

public:

	void Draw(ID3D11DeviceContext* _dc);

private:

	void Update(ID3D11DeviceContext* _dc);

	ID3D11Device* device = nullptr;
	ChStd::Bool vbUpdateFlg = true;
	unsigned long maxEffectCount = 0;
	In_Vertex *effectPosList = nullptr;
	unsigned long* indexNum = nullptr;
	ChD3D11::VertexBuffer11<In_Vertex> vb;
	ChD3D11::IndexBuffer11 ib;

	ChPtr::Shared<ChD3D11::TextureBase11> effectTexture = nullptr;

	ChStd::Bool gsUpdateFlg = true;
	GSEffectObjectData gsData;
	ChD3D11::ConstantBuffer11<GSEffectObjectData>gsBuf;

	ChStd::Bool psUpdateFlg = true;
	PSEffectObjectData psData;
	ChD3D11::ConstantBuffer11<PSEffectObjectData>psBuf;
};