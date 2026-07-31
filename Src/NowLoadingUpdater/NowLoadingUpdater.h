#pragma once

class NowLoadingUpdater
{
public:

	void Init();

	void Update();

	void Draw(ChD3D11::Shader::BaseDrawSprite11& _drawer);

private:

	void InitNowLoadingRect();

	void UpdateUpper();

	void UppdateDowner();

private:

	ChD3D11::Sprite11 nowLoadingSprite;
	ChD3D11::Texture11 nowLoading;
	ChVec4 nowLoadingPosRect;
	ChVec4 nowLoadingUVRect;
	float animationMoveSpeed = 0.0f;
	float animationWaitTime = 0.0f;
	float nowAnimationWaitTime = 1.0f;
	bool upFlg = true;

};