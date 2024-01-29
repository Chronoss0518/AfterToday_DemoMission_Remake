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

	void SetUseDepthStencilTestFlg(bool _flg);

	void SetAlphaBlendTestFlg(bool _flg);

	void SetViewMatrix(const ChLMat& _viewMat);

	void SetProjectionMatrix(const ChLMat& _projectionMat);

	void SetObjectSize(const ChVec2& _objectSize);

	void SetLuminescencePower(const float _power);

	void SetAlphaTestNum(const float _num);

	void SetBlendFlg(const bool& _flg);

	void SetSpecularColor(const ChVec3& _color);

	void SetLightFlg(const bool& _flg);

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

	void SetEffectDisplayFlg(const bool& _flg, const unsigned long _effectCount);

public:

	unsigned long GetMaxEffectCount() { return effectPosList.size(); }

	EOS_In_Vertex GetEffectPos(unsigned long _num)
	{ 
		EOS_In_Vertex res;
		if (_num < effectPosList.size())
		{
			res.pos = effectPosList[_num].pos;
			res.v_normal = effectPosList[_num].v_normal;
			res.v_binormal = effectPosList[_num].v_binormal;
			res.v_tangent = effectPosList[_num].v_tangent;
			res.color = effectPosList[_num].color;
			res.animationCount.val = effectPosList[_num].animationCount.val;
			res.displayFlg = effectPosList[_num].displayFlg;
		}
		return res;
	}

public:

	void Draw(ID3D11DeviceContext* _dc);

protected:

	void Update(ID3D11DeviceContext* _dc)override;

private:

	bool rasterizerUpdateFlg = true;;
	bool alphaBlendTestFlg = false;
	bool useDepthStencilTestFlg = true;
	bool vbUpdateFlg = true;
	std::vector<EOS_In_Vertex> effectPosList;
	ChD3D11::VertexBuffer11<EOS_In_Vertex> vb;

	ChPtr::Shared<ChD3D11::TextureBase11> effectTexture = nullptr;

	bool gsUpdateFlg = true;
	GSEffectObjectData gsData;
	ChD3D11::ConstantBuffer11<GSEffectObjectData>gsBuf;

	bool psUpdateFlg = true;
	PSEffectObjectData psData;
	ChD3D11::ConstantBuffer11<PSEffectObjectData>psBuf;

};