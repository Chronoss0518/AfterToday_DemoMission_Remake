
#include"../../BaseIncluder.h"

#include"../../Shader/EffectObject/EffectObjectShader.h"

#include"SmokeEffectList.h"

#include"../../Application/Application.h"

#define OBJECT_SIZE 1.0f
#define RANDOM_ALPHA_POWER_SIZE 1000

#define DISPERSAL_POWER_DOWN_PARCEC 0.9f

void SmokeEffectList::SmokeEffectThreadUpdate::Update()
{
	if (effectList == nullptr)
	{
		Destroy();
		return;
	}

	if (effectList->updateFlg)return;

	effectList->Update();
}

void SmokeEffectList::Init(ID3D11Device* _device, const unsigned long _maxCount, const unsigned long _width, const unsigned long _height)
{
	Release();

	effectShader = ChPtr::Make_S<EffectObjectShader>();

	effectShader->Init(_device, _maxCount);
	effectMoveDataList.resize(_maxCount);

	effectShader->SetEffectTexture(TEXTURE_DIRECTORY(L"SircleTexture.png"), 1, 1);

	effectShader->SetObjectSize(ChVec2(OBJECT_SIZE));
	effectShader->SetBlendFlg(true);
	effectShader->SetLightFlg(false);
	effectShader->SetUseDepthStencilTestFlg(false);
	effectShader->SetAlphaBlendTestFlg(true);
	effectShader->SetAlphaTestNum(0.01f);

	for (unsigned long i = 0; i < effectShader->GetMaxEffectCount(); i++)
	{
		effectMoveDataList[i] = ChPtr::Make_S<EffectMoveData>();
	}

#if USE_THREAD
	updateFlg = true;
	threadObject = ChPtr::Make_S<SmokeEffectThreadUpdate>();

	threadObject->SetSmokeEffectList(this);

	AppIns().GetThreadList().AddObject(threadObject);

#endif 

}

void SmokeEffectList::Release()
{
	if (threadObject != nullptr)
	{
		threadObject->SetSmokeEffectList(nullptr);
		threadObject->Destroy();
		threadObject = nullptr;
	}
	effectShader = nullptr;
	if(!effectMoveDataList.empty())effectMoveDataList.clear();
}

void SmokeEffectList::SetProjectionMatrix(const ChLMat& _projection)
{
	if (effectShader == nullptr)return;

	effectShader->SetProjectionMatrix(_projection);
}

void SmokeEffectList::SetViewMatrix(const ChLMat& _view)
{
	if (effectShader == nullptr)return;

	effectShader->SetViewMatrix(_view);
}

void SmokeEffectList::SetMaxColorPower(const float _power)
{
	if (_power <= minColPower)return;
	if (_power > 1.0f)return;

	maxColPower = _power;
}

void SmokeEffectList::SetMinColorPower(const float _power)
{
	if (_power >= maxColPower)return;
	if (_power < 0.0f)return;

	minColPower = _power;
}

void SmokeEffectList::SetInitialDispersalPower(const float _power)
{
	initialDispersalPower = _power;
}

void SmokeEffectList::SetDownSpeedOnAlphaValue(const float _speed)
{
	if (_speed > 1.0f)return;
	if (_speed < 0.0f)return;

	downSpeedOnAlphaValue = _speed;
}

bool SmokeEffectList::IsUpdateFlg()
{
	return updateFlg;
}

void SmokeEffectList::AddSmokeEffect(const ChVec3& _pos, const ChVec3& _moveVector)
{
	AddSmokeEffect(_pos, _moveVector, initialDispersalPower);
}

void SmokeEffectList::AddSmokeEffect(const ChVec3& _pos, const ChVec3& _moveVector, const float _initDispersalpower)
{
	AddSmokeEffect(_pos, _moveVector, _initDispersalpower, 1.0f);
}

void SmokeEffectList::AddSmokeEffect(const ChVec3& _pos, const ChVec3& _moveVector, const float _initDispersalpower, const float _initAlphaPow)
{
	if (effectShader == nullptr)return;

	float smokePower = static_cast<float>(std::rand() % RANDOM_ALPHA_POWER_SIZE) / RANDOM_ALPHA_POWER_SIZE;

	smokePower *= (maxColPower - minColPower);
	smokePower += minColPower;

	ChVec4 color = ChVec4(smokePower);

	color.a = _initAlphaPow;

	effectShader->SetEffectColor(color, nowCount);
	effectShader->SetEffectPosition(_pos, nowCount);
	effectShader->SetEffectDisplayFlg(true, nowCount);
	effectMoveDataList[nowCount]->moveVector = _moveVector;
	effectMoveDataList[nowCount]->dispersal = 0.0f;
	effectMoveDataList[nowCount]->dispersalPower = _initDispersalpower;

	nowCount = (1 + nowCount) % effectShader->GetMaxEffectCount();
}

void SmokeEffectList::Update()
{
	if (effectShader == nullptr)return;

	for (unsigned long i = 0; i < effectShader->GetMaxEffectCount(); i++)
	{
		auto effectObject = effectShader->GetEffectPos(i);
		if (!effectObject.displayFlg)continue;
		if (effectObject.color.a <= 0.0f) {
			effectShader->SetEffectDisplayFlg(false, i);
			effectShader->SetEffectColor(ChVec4(1.0f), i);
			continue;
		}

		ChLMat tmpMat;
		effectObject.color.a -= downSpeedOnAlphaValue;
		effectObject.pos += effectMoveDataList[i]->moveVector;
		effectMoveDataList[i]->dispersal += effectMoveDataList[i]->dispersalPower;
		effectMoveDataList[i]->dispersalPower *= DISPERSAL_POWER_DOWN_PARCEC;
		tmpMat.SetScalling(ChVec3(effectMoveDataList[i]->dispersal));
		effectShader->SetEffectColor(effectObject.color, i);
		effectShader->SetEffectVertexRotation(tmpMat, i);
		effectShader->SetEffectPosition(effectObject.pos, i);

	}

	updateFlg = true;
}

void SmokeEffectList::Draw(ID3D11DeviceContext* _dc)
{
	if (effectShader == nullptr)return;

	effectShader->DrawStart(_dc);

	effectShader->Draw(_dc);

	effectShader->DrawEnd();

}
