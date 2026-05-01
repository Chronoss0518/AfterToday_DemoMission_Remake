#pragma once


class EffectObjectShader;

class ShotEffectList
{
private:

	class ShotEffectThreadUpdate : public ChCpp::ThreadObject
	{
	public:

		inline void SetShotEffectList(ShotEffectList* _effectList)
		{
			effectList = _effectList;
		}

	public:

		void Update()override;

	private:

		ShotEffectList* effectList = nullptr;
	};

public:

	void Init(ID3D11Device* _device, const unsigned long _maxCount);

	void Release();

public:

	inline void SetUpdateFlg(const bool _flg) { updateFlg = _flg; }

	void SetProjectionMatrix(const ChLMat& _projection);

	void SetViewMatrix(const ChLMat& _view);

public:

	bool IsUpdateFlg();

public:

	void AddShotEffect(const ChVec3& _pos);

public:

	void Update();

public:

	void Draw(ID3D11DeviceContext* _dc);

private:

	bool updateFlg = false;
	ChPtr::Shared<ShotEffectThreadUpdate>threadObject = nullptr;

	ChPtr::Shared<EffectObjectShader> effectShader = nullptr;
	unsigned long nowCount = 0;
};
