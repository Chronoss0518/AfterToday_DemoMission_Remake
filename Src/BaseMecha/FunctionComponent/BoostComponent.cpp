
#include"../../BaseIncluder.h"

#include"../../AllStruct.h"

#include"BoostComponent.h"


BoostComponent::BoostComponent()
{
	data[ChStd::EnumCast(BoostDirection::Front)].direction = ChVec3(0.0f, 0.0f, 1.0f);
	data[ChStd::EnumCast(BoostDirection::Front)].boost.name = InputName::FrontBoost;
	data[ChStd::EnumCast(BoostDirection::Front)].avoid.name = InputName::FrontAvo;

	data[ChStd::EnumCast(BoostDirection::Back)].direction = ChVec3(0.0f, 0.0f, -1.0f);
	data[ChStd::EnumCast(BoostDirection::Back)].boost.name = InputName::BackBoost;
	data[ChStd::EnumCast(BoostDirection::Back)].avoid.name = InputName::BackAvo;

	data[ChStd::EnumCast(BoostDirection::Right)].direction = ChVec3(1.0f, 0.0f, 0.0f);
	data[ChStd::EnumCast(BoostDirection::Right)].boost.name = InputName::RightBoost;
	data[ChStd::EnumCast(BoostDirection::Right)].avoid.name = InputName::RightAvo;

	data[ChStd::EnumCast(BoostDirection::Left)].direction = ChVec3(-1.0f, 0.0f, 0.0f);
	data[ChStd::EnumCast(BoostDirection::Left)].boost.name = InputName::LeftBoost;
	data[ChStd::EnumCast(BoostDirection::Left)].avoid.name = InputName::LeftAvo;

	data[ChStd::EnumCast(BoostDirection::Up)].direction = ChVec3(0.0f, 1.0f, 0.0f);
	data[ChStd::EnumCast(BoostDirection::Up)].boost.name = InputName::UpBoost;
	data[ChStd::EnumCast(BoostDirection::Up)].avoid.name = InputName::UpAvo;

	data[ChStd::EnumCast(BoostDirection::Down)].direction = ChVec3(0.0f, -1.0f, 1.0f);
	data[ChStd::EnumCast(BoostDirection::Down)].boost.name = InputName::DownBoost;
	data[ChStd::EnumCast(BoostDirection::Down)].avoid.name = InputName::DownAvo;
}

void BoostComponent::ClearBoostAvoidWait(InputName _avoidType)
{
	long num = ChStd::EnumCast(_avoidType) - ChStd::EnumCast(InputName::FrontAvo);

	if (num < 0 || num >= 6)return;

	data[num].avoid.wait = 0;
}

void BoostComponent::Update()
{
	unsigned long useEnelgy = 0;

	for (unsigned char i = 0; i < 6; i++)
	{
		useEnelgy += GetUseEnelgy(data[i].avoid);
		useEnelgy += GetUseEnelgy(data[i].boost, data[i].avoid.testUseFlg);
	}

	unsigned long nowEnelgy = GetNowEnelgy();

	for (unsigned char i = 0; i < 6; i++)
	{
		data[i].boost.testUseFlg = (nowEnelgy > useEnelgy) && data[i].boost.testUseFlg;
		data[i].avoid.testUseFlg = (nowEnelgy > useEnelgy) && data[i].avoid.testUseFlg;

		UpdateInputFunction(data[i]);
		UpdateModelFunction(data[i]);
	}

	boostRotation += 5;
	boostRotation = boostRotation < 360.0f ? boostRotation : boostRotation - 360.0f;

}

void BoostComponent::BoostDrawBegin()
{

	ChLMat tmp;
	tmp.SetRotationYAxis(boostRotation);

	for (unsigned char i = 0; i < 6; i++)
	{
		auto&& boostData = data[i];
		for (auto&& boostObject : boostData.boostObject)
		{
			float testScl = boostObject->GetOutSizdTransformLMat().GetScalling().x;
			testScl = testScl >= boostData.nowBoostPow ? testScl : boostData.nowBoostPow;
			tmp.SetScalling(ChVec3(testScl));
			boostObject->SetOutSizdTransform(tmp);

			auto texture = boostObject->GetComponent<ChD3D11::FrameComponent11<wchar_t>>()->GetPrimitives()[0]->ValueIns().textures[Ch3D::TextureType::Diffuse];
		}
	}


}

void BoostComponent::BoostDrawEnd()
{

	ChLMat tmp;
	tmp.SetScalling(ChVec3(0.001f));

	for (unsigned char i = 0; i < 6; i++)
	{
		auto&& boostData = data[i];
		for (auto&& boostObject : boostData.boostObject)
		{
			boostObject->SetOutSizdTransform(tmp);
		}
	}

}

void BoostComponent::UpdateInputFunction(Data& _data)
{
	ChLMat tmp;

	tmp.SetRotationYAxis(ChMath::ToRadian(GetRotation().y));

	UpdateAvoid(_data, tmp);

	UpdateBoost(_data, tmp);

}
void BoostComponent::UpdateModelFunction(Data& _data)
{
	float regist = _data.nowBoostPow * GetBoostUpPowerRegister();
	_data.nowBoostPow -= regist;

	_data.nowBoostPow = _data.nowBoostPow < 0.0f ? 0.0f : _data.nowBoostPow;
}

void BoostComponent::BoostComponent::UpdateBoost(Data& _data, const ChLMat& _nowTargetPoster)
{
	if (!_data.boost.testUseFlg)return;

	AddMoveVector(_nowTargetPoster.TransformCoord(_data.direction) * _data.boost.pow);

	SubNowEnelgy(_data.boost.useEnelgy);

	_data.nowBoostPow += 1.5f;
}


void BoostComponent::UpdateAvoid(Data& _data, const ChLMat& _nowTargetPoster)
{
	if (_data.avoid.wait >= _data.avoid.nowWaitTime)
	{
		_data.avoid.nowWaitTime++;
	}

	if (!_data.avoid.testUseFlg)
	{
		_data.avoid.useFlg = false;
		return;
	}

	if (_data.avoid.nowWaitTime < _data.avoid.wait)return;
	if (_data.avoid.useFlg)return;

	_data.avoid.nowWaitTime = 0;

	_data.avoid.useFlg = true;

	AddMoveVector(_nowTargetPoster.TransformCoord(_data.direction) * _data.avoid.pow);

	SubNowEnelgy(_data.avoid.useEnelgy);

	_data.nowBoostPow = 10.0f;
}

void BoostComponent::SetBoostAvoidWait(const unsigned long _avoidWait, InputName _avoidType)
{
	long num = ChStd::EnumCast(_avoidType) - ChStd::EnumCast(InputName::FrontAvo);

	if (num < 0 || num >= 6)return;

	data[num].avoid.wait = data[num].avoid.wait < _avoidWait ? _avoidWait : data[num].avoid.wait;
}

unsigned long BoostComponent::GetUseEnelgy(AvoidData& _data)
{
	_data.testUseFlg = false;

	if (!IsPushFlg(_data.name))return 0;

	if (_data.nowWaitTime < _data.wait)return 0;
	if (_data.useFlg)return 0;

	_data.testUseFlg = true;

	return _data.useEnelgy;
}

unsigned long BoostComponent::GetUseEnelgy(BoostData& _data, bool _avoidFlg)
{
	_data.testUseFlg = false;
	if (_avoidFlg)return 0;
	if (!IsPushFlg(_data.name))return 0;

	_data.testUseFlg = true;

	return _data.useEnelgy;
}

void BoostComponent::AddBoostWhereAvoidName(ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _boost, InputName _avoidType)
{
	long num = ChStd::EnumCast(_avoidType) - ChStd::EnumCast(InputName::FrontAvo);

	if (num < 0 || num >= 6)return;

	data[num].boostObject.push_back(_boost);
}

void BoostComponent::AddBoostWhereBoostName(ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _boost, InputName _boostType)
{
	long num = ChStd::EnumCast(_boostType) - ChStd::EnumCast(InputName::FrontBoost);

	if (num < 0 || num >= 6)return;

	data[num].boostObject.push_back(_boost);
}

void BoostComponent::AddBoostPow(const float _boostPow, InputName _boostType)
{
	long num = ChStd::EnumCast(_boostType) - ChStd::EnumCast(InputName::FrontBoost);

	if (num < 0 || num >= 6)return;

	data[num].boost.pow += _boostPow;
}

void BoostComponent::AddBoostUseEnelgy(const unsigned long _boostUseEnelgy, InputName _boostType)
{
	long num = ChStd::EnumCast(_boostType) - ChStd::EnumCast(InputName::FrontBoost);

	if (num < 0 || num >= 6)return;

	data[num].boost.useEnelgy += _boostUseEnelgy;
}

void BoostComponent::AddBoostAvoidPow(const float _avoidPow, InputName _avoidType)
{
	long num = ChStd::EnumCast(_avoidType) - ChStd::EnumCast(InputName::FrontAvo);

	if (num < 0 || num >= 6)return;

	data[num].avoid.pow += _avoidPow;
}

void BoostComponent::AddBoostAvoidUseEnelgy(const unsigned long _avoidUseEnelgy, InputName _avoidType)
{
	long num = ChStd::EnumCast(_avoidType) - ChStd::EnumCast(InputName::FrontAvo);

	if (num < 0 || num >= 6)return;

	data[num].avoid.useEnelgy += _avoidUseEnelgy;
}

void BoostComponent::SubBoostWhereAvoidName(ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _boost, InputName _avoidType)
{
	long num = ChStd::EnumCast(_avoidType) - ChStd::EnumCast(InputName::FrontAvo);

	if (num < 0 || num >= 6)return;

	auto&& it = std::find(data[num].boostObject.begin(), data[num].boostObject.end(),_boost);
	data[num].boostObject.erase(it);
}

void BoostComponent::SubBoostWhereBoostName(ChPtr::Shared<ChCpp::FrameObject<wchar_t>> _boost, InputName _boostType)
{
	long num = ChStd::EnumCast(_boostType) - ChStd::EnumCast(InputName::FrontBoost);

	if (num < 0 || num >= 6)return;

	auto&& it = std::find(data[num].boostObject.begin(), data[num].boostObject.end(), _boost);
	data[num].boostObject.erase(it);
}

void BoostComponent::SubBoostPow(const float _boostPow, InputName _boostType)
{
	long num = ChStd::EnumCast(_boostType) - ChStd::EnumCast(InputName::FrontBoost);

	if (num < 0 || num >= 6)return;

	data[num].boost.pow -= _boostPow;
}

void BoostComponent::SubBoostUseEnelgy(const unsigned long _boostUseEnelgy, InputName _boostType)
{
	long num = ChStd::EnumCast(_boostType) - ChStd::EnumCast(InputName::FrontBoost);

	if (num < 0 || num >= 6)return;

	data[num].boost.useEnelgy -= _boostUseEnelgy;
}

void BoostComponent::SubBoostAvoidPow(const float _avoidPow, InputName _avoidType)
{
	long num = ChStd::EnumCast(_avoidType) - ChStd::EnumCast(InputName::FrontAvo);

	if (num < 0 || num >= 6)return;

	data[num].avoid.pow -= _avoidPow;
}

void BoostComponent::SubBoostAvoidUseEnelgy(const unsigned long _avoidUseEnelgy, InputName _avoidType)
{
	long num = ChStd::EnumCast(_avoidType) - ChStd::EnumCast(InputName::FrontAvo);

	if (num < 0 || num >= 6)return;

	data[num].avoid.useEnelgy -= _avoidUseEnelgy;
}
