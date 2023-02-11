
#include"../../BaseIncluder.h"

#include"../../Shader/EffectObject/EffectObjectShader.h"

#include"SmokeEffectList.h"


#define OBJECT_SIZE 1.0f
#define RANDOM_ALPHA_POWER_SIZE 1000

void SmokeEffectList::Init(ID3D11Device* _device, const unsigned long _maxCount, const unsigned long _width, const unsigned long _height)
{
	Release();

	gameEndFlg = false;
	effectShader = ChPtr::Make_S<EffectObjectShader>();

	effectShader->Init(ChD3D11::D3D11Device(), _maxCount);
	effectMoveDataList.resize(_maxCount);

	effectShader->SetEffectTexture(TEXTURE_DIRECTORY("SircleTexture.png"), 1, 1);

	effectShader->SetObjectSize(ChVec2(OBJECT_SIZE));
	effectShader->SetBlendFlg(false);
	effectShader->SetLightFlg(true);
	effectShader->SetUseDepthStencilTestFlg(true);
	effectShader->SetAlphaBlendTestFlg(false);
	effectShader->SetAlphaTestNum(0.01f);

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

	for (unsigned long i = 0; i < effectShader->GetMaxEffectCount(); i++)
	{
		effectMoveDataList[i] = ChPtr::Make_S<EffectMoveData>();
	}


	updater.Init([&]() {
		while (!gameEndFlg)
		{
			if (updateFlg)continue;

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
				effectMoveDataList[i]->dispersal += dispersalPower;
				tmpMat.SetScalling(ChVec3(effectMoveDataList[i]->dispersal));
				effectShader->SetEffectColor(effectObject.color, i);
				effectShader->SetEffectVertexRotation(tmpMat, i);
				effectShader->SetEffectPosition(effectObject.pos, i);

			}
			updateFlg = true;
		}
	});


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

void SmokeEffectList::SetDispersalPower(const float _power)
{
	dispersalPower = _power;
}

void SmokeEffectList::SetDownSpeedOnAlphaValue(const float _speed)
{
	if (_speed > 1.0f)return;
	if (_speed < 0.0f)return;

	downSpeedOnAlphaValue = _speed;
}

void SmokeEffectList::AddSmokeEffect(const ChVec3& _pos, const ChVec3& _moveVector)
{
	if (effectShader == nullptr)return;

	float smokePower = static_cast<float>(std::rand() % RANDOM_ALPHA_POWER_SIZE) / RANDOM_ALPHA_POWER_SIZE;

	smokePower *= (maxColPower - minColPower);
	smokePower += minColPower;

	ChVec4 color = ChVec4(smokePower);

	color.a = 1.0f;

	effectShader->SetEffectColor(color, nowCount);
	effectShader->SetEffectPosition(_pos, nowCount);
	effectShader->SetEffectDisplayFlg(true, nowCount);
	effectMoveDataList[nowCount]->moveVector = _moveVector;
	effectMoveDataList[nowCount]->dispersal = 1.0f;

	nowCount = (1 + nowCount) % effectShader->GetMaxEffectCount();
}

void SmokeEffectList::Draw(ID3D11DeviceContext* _dc)
{
	if (effectShader == nullptr)return;
	ID3D11RenderTargetView* tmpRTView = nullptr;
	ID3D11DepthStencilView* tmpDSView = nullptr;

	renderTarget.SetBackColor(_dc, ChVec4(0.0f, 0.0f, 0.0f, 0.0f));

	ID3D11RenderTargetView* rtView = renderTarget.GetRTView();

	_dc->OMGetRenderTargets(1, &tmpRTView, &tmpDSView);
	_dc->OMSetRenderTargets(1, &rtView, tmpDSView);

	effectShader->DrawStart(_dc);

	effectShader->Draw(_dc);

	effectShader->DrawEnd();

	_dc->OMSetRenderTargets(1, &tmpRTView, nullptr);

	spriteShader.DrawStart(_dc);

	spriteShader.Draw(_dc, renderTarget, sprite);

	spriteShader.DrawEnd();

	_dc->OMSetRenderTargets(1, &tmpRTView, tmpDSView);

	tmpRTView->Release();
	tmpDSView->Release();
}
