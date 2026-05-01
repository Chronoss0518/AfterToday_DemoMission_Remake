
#include"../../BaseIncluder.h"

#include"../../Shader/EffectObject/EffectObjectShader.h"

#include"ShotEffectList.h"

#include"../../Application/Application.h"

#define OBJECT_SIZE 5.0f
#define ANIMATION_COUNT 5


void ShotEffectList::ShotEffectThreadUpdate::Update()
{
	if (effectList == nullptr)
	{
		Destroy();
		return;
	}

	if (effectList->updateFlg)return;

	effectList->Update();
}

void ShotEffectList::Init(ID3D11Device* _device, const unsigned long _maxCount)
{
	Release();

	effectShader = ChPtr::Make_S<EffectObjectShader>();

	effectShader->Init(_device, _maxCount);

	effectShader->SetEffectTexture(TEXTURE_DIRECTORY(L"ShotEffectTexture.png"), ANIMATION_COUNT, 1);

	effectShader->SetAlphaBlendTestFlg(true);

	effectShader->SetLuminescencePower(4.0f);

	effectShader->SetObjectSize(ChVec2(OBJECT_SIZE));

	effectShader->SetAlphaTestNum(0.5f);

#if USE_THREAD
	updateFlg = true;
	threadObject = ChPtr::Make_S<ShotEffectThreadUpdate>();

	threadObject->SetShotEffectList(this);

	AppIns().GetThreadList().AddObject(threadObject);

#endif
}

bool ShotEffectList::IsUpdateFlg() 
{
	return updateFlg; 
}

void ShotEffectList::Release()
{
	if (threadObject != nullptr)
	{
		threadObject->SetShotEffectList(nullptr);
		threadObject->Destroy();
		threadObject = nullptr;
	}
	effectShader = nullptr;
}

void ShotEffectList::SetProjectionMatrix(const ChLMat& _projection)
{
	if (effectShader == nullptr)return;

	effectShader->SetProjectionMatrix(_projection);
}

void ShotEffectList::SetViewMatrix(const ChLMat& _view)
{
	if (effectShader == nullptr)return;

	effectShader->SetViewMatrix(_view);
}

void ShotEffectList::AddShotEffect(const ChVec3& _pos)
{
	if (effectShader == nullptr)return;


	effectShader->SetEffectPosition(_pos, nowCount);

	effectShader->SetEffectDisplayFlg(true, nowCount);

	nowCount = (1 + nowCount) % effectShader->GetMaxEffectCount();
}

void ShotEffectList::Update()
{
	if (effectShader == nullptr)return;

	for (unsigned long i = 0; i < effectShader->GetMaxEffectCount(); i++)
	{
		auto effectObject = effectShader->GetEffectPos(i);
		if (!effectObject.displayFlg)continue;
		effectObject.animationCount.w += 1;
		if (effectObject.animationCount.w > ANIMATION_COUNT) {
			effectShader->SetEffectDisplayFlg(false, i);
			effectShader->SetEffectHorizontalAnimationCount(0, i);
			continue;
		}
		effectShader->SetEffectHorizontalAnimationCount(effectObject.animationCount.w, i);
	}

	updateFlg = true;
}

void ShotEffectList::Draw(ID3D11DeviceContext* _dc)
{
	if (effectShader == nullptr)return;

	effectShader->DrawStart(_dc);

	effectShader->Draw(_dc);

	effectShader->DrawEnd();
}
