
#include"../../BaseIncluder.h"

#include"../../Shader/EffectObject/EffectObjectShader.h"

#include"SmokeEffectList.h"

#define USE_RENDER_TARGET 0

#define OBJECT_SIZE 1.0f
#define RANDOM_ALPHA_POWER_SIZE 1000

#define DISPERSAL_POWER_DOWN_PARCEC 0.9f

void SmokeEffectList::Init(ID3D11Device* _device, const unsigned long _maxCount, const unsigned long _width, const unsigned long _height)
{
	Release();

	gameEndFlg = false;
	effectShader = ChPtr::Make_S<EffectObjectShader>();

	effectShader->Init(ChD3D11::D3D11Device(), _maxCount);
	effectMoveDataList.resize(_maxCount);

	effectShader->SetEffectTexture(TEXTURE_DIRECTORY(L"SircleTexture.png"), 1, 1);

	effectShader->SetObjectSize(ChVec2(OBJECT_SIZE));
	effectShader->SetBlendFlg(false);
	effectShader->SetLightFlg(true);
	effectShader->SetUseDepthStencilTestFlg(USE_RENDER_TARGET);
	effectShader->SetAlphaBlendTestFlg(!USE_RENDER_TARGET);
	effectShader->SetAlphaTestNum(0.01f);

#if USE_RENDER_TARGET

	renderTarget.CreateRenderTarget(_device, _width, _height);
	sprite.Init(_device);

	sprite.SetPos(0, ChVec2(-1.0f, 1.0f));
	sprite.SetPos(1, ChVec2(1.0f, 1.0f));
	sprite.SetPos(2, ChVec2(1.0f, -1.0f));
	sprite.SetPos(3, ChVec2(-1.0f, -1.0f));

	sprite.SetUVPos(0, ChVec2(0.0f, 0.0f));
	sprite.SetUVPos(1, ChVec2(1.0f, 0.0f));
	sprite.SetUVPos(2, ChVec2(1.0f, 1.0f));
	sprite.SetUVPos(3, ChVec2(0.0f, 1.0f));

	spriteShader.Init(_device);
	spriteShader.SetAlphaBlendFlg(true);

#endif

	for (unsigned long i = 0; i < effectShader->GetMaxEffectCount(); i++)
	{
		effectMoveDataList[i] = ChPtr::Make_S<EffectMoveData>();
	}

#if USE_THREAD
	updater.Init([&]() {
		while (!gameEndFlg)
		{
			if (updateFlg)
			{
				std::this_thread::yield();
				continue;
			}
			Update();
			updateFlg = true;
		}
	});
#endif 

}

void SmokeEffectList::Release()
{
	gameEndFlg = true;
	updater.Release();
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
}

void SmokeEffectList::Draw(ID3D11DeviceContext* _dc)
{
	if (effectShader == nullptr)return;

#if USE_RENDER_TARGET

	ID3D11RenderTargetView* tmpRTView = nullptr;
	ID3D11DepthStencilView* tmpDSView = nullptr;

	renderTarget.SetBackColor(_dc, ChVec4(0.0f, 0.0f, 0.0f, 0.0f));

	ID3D11RenderTargetView* rtView = renderTarget.GetRTView();

	_dc->OMGetRenderTargets(1, &tmpRTView, &tmpDSView);
	_dc->OMSetRenderTargets(1, &rtView, tmpDSView);
#endif

	effectShader->DrawStart(_dc);

	effectShader->Draw(_dc);

	effectShader->DrawEnd();

#if USE_RENDER_TARGET
	_dc->OMSetRenderTargets(1, &tmpRTView, nullptr);

	spriteShader.DrawStart(_dc);

	spriteShader.Draw(_dc, renderTarget, sprite);

	spriteShader.DrawEnd();

	_dc->OMSetRenderTargets(1, &tmpRTView, tmpDSView);

	tmpRTView->Release();
	tmpDSView->Release();
#endif
}
