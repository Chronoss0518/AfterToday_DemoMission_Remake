#include"../BaseIncluder.h"

#include"NowLoadingUpdater.h"

#include"../Application/Application.h"

#define NOW_LOADING_ANIMATION_MOVE_SPEED 0.5f
#define NOW_LOADING_ANIMATION_WAIT_TIME 0.2f

void NowLoadingUpdater::Init()
{
	auto&& device = AppIns().GetDirect3D11().GetDevice();

	nowLoadingSprite.Init();
	nowLoadingSprite.SetInitPosition();
	nowLoading.CreateTexture(TEXTURE_DIRECTORY(L"NowLoading.png"), device);
	InitNowLoadingRect();
}

void NowLoadingUpdater::Update()
{
	UpdateUpper();
	UppdateDowner();
}

void NowLoadingUpdater::UpdateUpper()
{
	if (!upFlg)return;

	nowLoadingPosRect.right += animationMoveSpeed;
	nowLoadingUVRect.right += (animationMoveSpeed * 0.5f);

	if (nowLoadingPosRect.right < 1.0f)return;

	upFlg = false;
	nowLoadingPosRect.right = 1.0f;
	nowLoadingUVRect.right = 1.0f;
}

void NowLoadingUpdater::UppdateDowner()
{
	if (upFlg)return;

	if ((nowAnimationWaitTime -= animationWaitTime) > 0.0f)return;

	nowLoadingPosRect.left += animationMoveSpeed;
	nowLoadingUVRect.left += (animationMoveSpeed * 0.5f);

	if (nowLoadingPosRect.left <= 1.0f)return;
	InitNowLoadingRect();
}

void NowLoadingUpdater::Draw(ChD3D11::Shader::BaseDrawSprite11& _drawer)
{
	nowLoadingSprite.SetPosRect(nowLoadingPosRect);
	nowLoadingSprite.SetUVPosRect(nowLoadingUVRect);

	_drawer.Draw(nowLoading, nowLoadingSprite);
}

void NowLoadingUpdater::InitNowLoadingRect()
{
	animationMoveSpeed = 1.0f / (NOW_LOADING_ANIMATION_MOVE_SPEED * static_cast<float>(AppIns().GetFPS()));
	animationWaitTime = 1.0f / (NOW_LOADING_ANIMATION_WAIT_TIME * static_cast<float>(AppIns().GetFPS()));
	nowLoadingPosRect = ChVec4::FromRect(-1.0f, 1.0f, -1.0f, -1.0f);
	nowLoadingUVRect = ChVec4::FromRect(0.0f, 0.0f, 0.0f, 1.0f);
	nowAnimationWaitTime = 1.0f;
	upFlg = true;
}
