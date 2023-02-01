
#include"../../BaseIncluder.h"

#include"../../Shader/EffectObject/EffectObjectShader.h"

#include"SmokeEffectList.h"


#define OBJECT_SIZE 5.0f
#define RANDOM_ALPHA_POWER_SIZE 1000

void SmokeEffectList::Init(ID3D11Device* _device, const unsigned long _maxCount)
{
	Release();

	gameEndFlg = false;
	effectShader = ChPtr::Make_S<EffectObjectShader>();

	effectShader->Init(ChD3D11::D3D11Device(), _maxCount);
	effectObjectList.resize(_maxCount);

	effectShader->SetEffectTexture(TEXTURE_DIRECTORY("SircleTexture.png"), 1, 1);

	effectShader->SetObjectSize(ChVec2(OBJECT_SIZE));

	for (unsigned long i = 0; i < effectShader->GetMaxEffectCount(); i++)
	{
		auto effectObject = ChPtr::Make_S<EffectObject>();
		effectObject->object = effectShader->GetEffectPos(i);
		effectObjectList[i] = effectObject;
	}


	updater.Init([&]() {
		while (!gameEndFlg)
		{
			if (updateFlg)continue;

			for (auto&& effectObject : effectObjectList)
			{

				if (!effectObject->object->displayFlg)continue;
				effectObject->object->color.a -= downSpeedOnAlphaValue;
				if (effectObject->object->color.a <= 0.0f) {
					effectObject->object->displayFlg = false;
					effectObject->object->color.a = 1.0f;
					continue;
				}
				effectObject->object->pos += effectObject->moveVector;
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
	if(!effectObjectList.empty())effectObjectList.clear();
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

void SmokeEffectList::SetDownSpeedOnAlphaValue(const float _speed)
{
	if (_speed > 1.0f)return;
	if (_speed < 0.0f)return;

	downSpeedOnAlphaValue = _speed;
}

void SmokeEffectList::AddShotEffect(const ChVec3& _pos, const ChVec3& _moveVector)
{
	if (effectShader == nullptr)return;

	float smokePower = static_cast<float>(std::rand() % RANDOM_ALPHA_POWER_SIZE) / RANDOM_ALPHA_POWER_SIZE;

	smokePower *= (maxColPower - minColPower);
	smokePower += minColPower;

	ChVec4 color = ChVec4(smokePower);

	color.a = 1.0f;

	effectObjectList[nowCount]->object->color = color;
	effectObjectList[nowCount]->object->pos = _pos;
	effectObjectList[nowCount]->object->displayFlg = true;
	effectObjectList[nowCount]->moveVector = _moveVector;

	nowCount = (1 + nowCount) % effectObjectList.size();
}

void SmokeEffectList::Draw(ID3D11DeviceContext* _dc)
{
	if (effectShader == nullptr)return;

	effectShader->DrawStart(_dc);

	effectShader->Draw(_dc);

	effectShader->DrawEnd();
}
