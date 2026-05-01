#include"../../BaseIncluder.h"

#include<math.h>

#include"../../Frames/GameFrame.h"
#include"../BaseMecha.h"
#include"../FunctionComponent/CameraComponent.h"
#include"../Controller/ControllerBase.h"
#include"CPULooker.h"

#include"../../Application/Application.h"

#define FEATURE false
#define MATH_MATRIX true

unsigned long resetNum = -1;

void CPUObjectLooker::CPUObjectLookerThreadUpdate::Update()
{
	if (looker == nullptr)
	{
		Destroy();
		return;
	}

	if (!looker->updateFlg)return;
	looker->FindMecha();

}

unsigned long CPUObjectLooker::GetLookTypeMechas(MemberType _member, DistanceType _distance, DamageSizeType _damageSize)
{
	return lookMechaTypes[ChStd::EnumCast(_member)][ChStd::EnumCast(_distance)][ChStd::EnumCast(_damageSize)];
}

void CPUObjectLooker::Init()
{
	auto device = ChD3D11::D3D11Device();

	spriteDrawer.Init(device);

	spriteDrawer.SetAlphaBlendFlg(true);

	ChVec4 tmp = ChVec4(1.0f, 1.0f, 1.0f, 1.0f);

	whiteTexture.CreateColorTexture(&tmp, 1, 1);

	sprite.Init();
	sprite.SetInitPosition();

#if USE_CPU_THREAD

	updateFlg = false;
	threadObject = ChPtr::Make_S<CPUObjectLookerThreadUpdate>();

	threadObject->SetCPUObjectLooker(this);

	AppIns().AddCPUThread(threadObject);

#endif

}

void CPUObjectLooker::DrawBegin()
{
	nowUpdateCount = (nowUpdateCount + 1) % updateCount;

	if (nowUpdateCount <= 0)
	{

		if (!lookMechaList.empty())lookMechaList.clear();
		for (unsigned char memberType = 0; memberType < MEMBER_TYPE_COUNT; memberType++)
		{
			for (unsigned char distanceType = 0; distanceType < DISTANCE_TYPE_COUNT; distanceType++)
			{
				for (unsigned char damageSizeType = 0; damageSizeType < DAMAGE_SIZE_TYPE_COUNT; damageSizeType++)
				{
					lookMechaTypes[memberType][distanceType][damageSizeType] = resetNum;
				}
			}
		}

#if USE_CPU_THREAD
		updateFlg = true;
#else

		FindMecha();
#endif
	}


}

void CPUObjectLooker::Draw2D()
{

}

void CPUObjectLooker::DrawEnd()
{
	auto&& mecha = LookObj<BaseMecha>();

	while (!IsEndUpdate() && !mecha->IsBreak())continue;
}

void CPUObjectLooker::Release()
{
	if (threadObject != nullptr)
	{
		threadObject->SetCPUObjectLooker(nullptr);
		threadObject->Destroy();
		threadObject = nullptr;
	}
}

void CPUObjectLooker::FindMecha()
{
	if (ChPtr::NullCheck(frame))return;

	auto mecha = LookObj<BaseMecha>();

	if (ChPtr::NullCheck(mecha))
	{
		Destroy();
		updateFlg = false;
		return;
	}

	if (mecha->IsBreak())
	{
		updateFlg = false;
		return;
	}

	ChVec3 pos = mecha->GetPosition();

	auto&& hitTestMap = frame->GetHitMapList();

	auto&& baseMechaList = frame->GetMechas();

	float nearLength[3] = { 10e+37f,10e+37f ,10e+37f };
	float farLength[3] = { 0.0f ,0.0f ,0.0f };

	bool lookFlg = true;

	ray.SetPosition(LookObj<BaseMecha>()->GetPosition());

	auto&& camCom = mecha->GetComponentObject<CameraComponent>();

	ChLMat vpMat = camCom->GetViewMat() * camCom->GetProMat();

	for (unsigned long i = 0; i < baseMechaList.size(); i++)
	{
		auto&& tmpMecha = baseMechaList[i];
		auto&& otherMecha = tmpMecha.lock();
		if (otherMecha == nullptr)continue;
		if (otherMecha->GetMechaNo() == LookObj<BaseMecha>()->GetMechaNo())continue;
		unsigned long memberType = ChStd::EnumCast(MemberType::Enemy);
		auto controller = otherMecha->GetComponent<ControllerBase>();
		if (controller == nullptr)continue;
		if (controller->GetTeamNo() == mecha->GetTeamNo()) memberType = ChStd::EnumCast(MemberType::Member);

		ChVec3 targetPos = otherMecha->GetPosition();

		{
			ChVec4 tmp = vpMat.Transform(targetPos);

			tmp /= tmp.w;

			if (tmp.z < 0.0f || tmp.z > 1.0f)continue;

			tmp *= tmp;

			if (tmp.x > 1.0f || tmp.y > 1.0f)continue;
		}

		ChVec3 direction = targetPos - pos;

		float tmpLength = direction.GetLen();

		direction.Normalize();

		ray.SetRayDir(direction);
		
		lookFlg = true;

		for (auto&& map : hitTestMap)
		{
			auto&& collider = map->GetComponent<MapCollider>();

			if (collider == nullptr)continue;

			if (!collider->GetCollider().IsHit(&ray))continue;

			if (ray.GetHitVectol().GetLen() > tmpLength)continue;

			lookFlg = false;
			break;
		}

		if (!lookFlg)continue;

		if (memberType == ChStd::EnumCast(MemberType::Enemy))
		{
			lookMechaList.push_back(i);
		}

		if (nearLength[memberType] > tmpLength)
		{
			nearLength[memberType] = tmpLength;

			MenyDamageTest(lookMechaTypes[memberType][ChStd::EnumCast(DistanceType::Near)][ChStd::EnumCast(DamageSizeType::Many)], i, baseMechaList);
			FewDamageTest(lookMechaTypes[memberType][ChStd::EnumCast(DistanceType::Near)][ChStd::EnumCast(DamageSizeType::Few)], i, baseMechaList);
		}

		if (farLength[memberType] < tmpLength)
		{
			farLength[memberType] = tmpLength;

			MenyDamageTest(lookMechaTypes[memberType][ChStd::EnumCast(DistanceType::Far)][ChStd::EnumCast(DamageSizeType::Many)], i, baseMechaList);
			FewDamageTest(lookMechaTypes[memberType][ChStd::EnumCast(DistanceType::Far)][ChStd::EnumCast(DamageSizeType::Few)], i, baseMechaList);
		}

	}

	updateFlg = false;
}

void CPUObjectLooker::MenyDamageTest(unsigned long& _base, unsigned long _target, std::vector<ChPtr::Weak<BaseMecha>>& _mechas)
{
	if (_target >= _mechas.size())return;

	auto target = _mechas[_target].lock();
	if (target == nullptr)return;

	if (_base >= _mechas.size())
	{
		_base = _target;
		return;
	}

	auto base = _mechas[_base].lock();
	if (base == nullptr)
	{
		_base = _target;
		return;
	}

	if (base->GetDamage() >= target->GetDamage())return;
	_base = _target;
}

void CPUObjectLooker::FewDamageTest(unsigned long& _base, unsigned long _target, std::vector<ChPtr::Weak<BaseMecha>>& _mechas)
{
	if (_target >= _mechas.size())return;

	auto target = _mechas[_target].lock();
	if (target == nullptr)return;

	if (_base >= _mechas.size())
	{
		_base = _target;
		return;
	}

	auto base = _mechas[_base].lock();
	if (base == nullptr)
	{
		_base = _target;
		return;
	}

	if (base->GetDamage() <= target->GetDamage())return;
	_base = _target;
}
