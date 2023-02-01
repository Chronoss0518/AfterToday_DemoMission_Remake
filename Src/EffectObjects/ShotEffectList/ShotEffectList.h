#pragma once


class EffectObjectShader;

class ShotEffectList
{

public:

	void Init(ID3D11Device* _device, const unsigned long _maxCount);

	void Release();

public:

	inline void SetUpdateFlg(const ChStd::Bool _flg) { updateFlg = _flg; }

	void SetProjectionMatrix(const ChLMat& _projection);

	void SetViewMatrix(const ChLMat& _view);

public:

	ChStd::Bool IsUpdateFlg() { return updateFlg; }

public:

	void AddShotEffect(const ChVec3& _pos);

public:

	void Draw(ID3D11DeviceContext* _dc);

private:

	ChStd::Bool gameEndFlg = false;
	ChStd::Bool updateFlg = false;
	ChCpp::MultiThread updater;
	ChPtr::Shared<EffectObjectShader> effectShader = nullptr;
	unsigned long nowCount = 0;
};
