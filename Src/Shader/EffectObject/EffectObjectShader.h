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

	void SetObjectSize(const ChVec2& _objectSize);

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

	void SetEffectVertexRotation(const ChLMat& _rotateMatrix, const unsigned long _effectCount);

	void SetEffectColor(const ChVec4& _effectColor, const unsigned long _effectCount);

	void SetEffectColor(const ChVec3& _effectColor, const unsigned long _effectCount);

	void SetEffectAnimationCount(const ChMath::Vector2Base<unsigned long>& _animationCount, const unsigned long _effectCount);

	void SetEffectHorizontalAnimationCount(const unsigned long _animationCount, const unsigned long _effectCount);

	void SetEffectVerticalAnimationCount(const unsigned long _animationCount, const unsigned long _effectCount);

	void SetEffectDisplayFlg(const ChStd::Bool& _flg, const unsigned long _effectCount);

public:

	unsigned long GetMaxEffectCount() { return effectPosList.size(); }

	In_Vertex* GetEffectPos(unsigned long _num)
	{ 
		In_Vertex* res = nullptr;
		if (_num < effectPosList.size())
		{
			res = &effectPosList[_num];
		}

		return res;
	}

public:

	void Draw(ID3D11DeviceContext* _dc);

protected:

	void Update(ID3D11DeviceContext* _dc)override;

private:

	ChStd::Bool vbUpdateFlg = true;
	std::vector<In_Vertex> effectPosList;
	ChD3D11::VertexBuffer11<In_Vertex> vb;

	ChPtr::Shared<ChD3D11::TextureBase11> effectTexture = nullptr;

	ChStd::Bool gsUpdateFlg = true;
	GSEffectObjectData gsData;
	ChD3D11::ConstantBuffer11<GSEffectObjectData>gsBuf;

	ChStd::Bool psUpdateFlg = true;
	PSEffectObjectData psData;
	ChD3D11::ConstantBuffer11<PSEffectObjectData>psBuf;
};