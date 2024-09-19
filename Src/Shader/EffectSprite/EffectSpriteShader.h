#pragma once

#include"HLSL/EffectSpriteShader.hlsli"

class EffectSpriteShader:public ChD3D11::Shader::SampleShaderBase11
{
public:

	void Init(ID3D11Device* _device, const unsigned long _maxEffectNum);

	void Release()override;

protected:

	void InitVertexShader()override;

	void InitPixelShader()override;

	void InitGeometryShader()override;

public:

	void SetObjectSize(const ChVec2& _objectSize);

	void SetEffectTexture(ChPtr::Shared<ChD3D11::TextureBase11> _effectTexture, const ChMath::Vector2Base<unsigned long>& _animationCount);

	void SetEffectTexture(ChPtr::Shared<ChD3D11::TextureBase11> _effectTexture, const unsigned long _horizontalAnimationCount, const unsigned long _verticalAnimationCount);

	void SetEffectTexture(const std::wstring& _texturePath, const ChMath::Vector2Base<unsigned long>& _animationCount);

	void SetEffectTexture(const std::wstring& _texturePath, const unsigned long _horizontalAnimationCount, const unsigned long _verticalAnimationCount);

	void SetEffectPosition(const ChVec2& _pos, const unsigned long _effectCount);

	void SetEffectColor(const ChVec4& _effectColor, const unsigned long _effectCount);

	void SetEffectColor(const ChVec3& _effectColor, const unsigned long _effectCount);

	void SetEffectAnimationCount(const ChMath::Vector2Base<unsigned long>& _animationCount, const unsigned long _effectCount);

	void SetEffectHorizontalAnimationCount(const unsigned long _animationCount, const unsigned long _effectCount);

	void SetEffectVerticalAnimationCount(const unsigned long _animationCount, const unsigned long _effectCount);

	void SetEffectDisplayFlg(const bool& _flg, const unsigned long _effectCount);

public:

	unsigned long GetMaxEffectCount() { return effectPosList.size(); }

	ESS_In_Vertex GetEffectPos(unsigned long _num)
	{ 
		ESS_In_Vertex res;
		if (_num < effectPosList.size())
		{
			res.pos = effectPosList[_num].pos;
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

	bool rasterizerUpdateFlg = true;
	bool vbUpdateFlg = true;
	std::vector<ESS_In_Vertex> effectPosList;
	ChD3D11::VertexBuffer11<ESS_In_Vertex> vb;

	ChPtr::Shared<ChD3D11::TextureBase11> effectTexture = nullptr;

	bool gsUpdateFlg = true;
	GSEffectSpriteData gsData;
	ChD3D11::ConstantBuffer11<GSEffectSpriteData>gsBuf;

	bool psUpdateFlg = true;
	PSEffectSpriteData psData;
	ChD3D11::ConstantBuffer11<PSEffectSpriteData>psBuf;

};