#pragma once


class EffectObjectShader;

class SmokeEffectList
{

public:

	void Init(ID3D11Device* _device, const unsigned long _maxCount, const unsigned long _width, const unsigned long _height);

	void Release();

public:

	inline void SetUpdateFlg(const ChStd::Bool _flg) { updateFlg = _flg; }

	void SetProjectionMatrix(const ChLMat& _projection);

	void SetViewMatrix(const ChLMat& _view);

	void SetMaxColorPower(const float _power);

	void SetMinColorPower(const float _power);

	void SetInitialDispersalPower(const float _power);

	void SetDownSpeedOnAlphaValue(const float _speed);

public:

	ChStd::Bool IsUpdateFlg() { return updateFlg; }

public:

	void AddSmokeEffect(const ChVec3& _pos,const ChVec3& _moveVector);

	void AddSmokeEffect(const ChVec3& _pos,const ChVec3& _moveVector, const float _initDispersalpower);

public:

	void Draw(ID3D11DeviceContext* _dc);

private:

	struct EffectMoveData
	{
		float dispersal = 1.0f;
		float dispersalPower = 0.0f;
		ChVec3 moveVector = 0.0f;
	};

	float maxColPower = 1.0f;
	float minColPower = 0.0f;

	//���U��//
	float initialDispersalPower = 5.0f;

	float downSpeedOnAlphaValue = 0.05f;

	ChStd::Bool gameEndFlg = false;
	ChStd::Bool updateFlg = false;

	ChCpp::MultiThread updater;

#if 0

	ChD3D11::RenderTarget11 renderTarget;
	ChD3D11::Sprite11 sprite;
	ChD3D11::Shader::BaseDrawSprite11 spriteShader;
#endif

	std::vector<ChPtr::Shared<EffectMoveData>>effectMoveDataList;
	ChPtr::Shared<EffectObjectShader> effectShader = nullptr;
	unsigned long nowCount = 0;
};
