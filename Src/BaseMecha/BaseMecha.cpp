
#include"../BaseIncluder.h"

#include"../AllStruct.h"

#include"../Attack/Attack.h"
#include"MechaParts.h"
#include"MechaPartsObject.h"
#include"BaseMecha.h"
#include"../Frames/GameFrame.h"

#include"MechaPartsData/NextPosData.h"

#include"FunctionComponent/WeaponComponent.h"
#include"FunctionComponent/BoostComponent.h"
#include"FunctionComponent/EnergyComponent.h"
#include"FunctionComponent/MoveComponent.h"
#include"FunctionComponent/CameraComponent.h"

#include"../EditFrame/PartsParameters.h"

#include"MechaPartsObjectFunction/WeaponFunction.h"
#include"../Attack/AttackObject.h"
#include"Controller/ControllerBase.h"

#include"CPU/CPULooker.h"

#include"../Application/Application.h"

#define CENTER_LEN 5.0f

#define HIT_EFFECT_DRAW_FRAME static_cast<long>(BASE_FPS * 2.0f)

#define OBJECT_DESTROY_COUNT 2 * AppIns().GetFPS()

#define JSON_MECHA_NAME L"Name"
#define JSON_CORE L"Core"

#define CREATE_SMOKE_EFFECT_TIME 3

ChCpp::BitBool noHoldMaskFlgs = ChCpp::BitBool(((unsigned char)BaseMecha::InputName::None / 8) + 1);
bool isNoHoldMaskFlgInit = false;

void InitNoHoldMaskFlgs()
{
	if (isNoHoldMaskFlgInit)return;
	noHoldMaskFlgs.SetBitTrue(ChStd::EnumCast(BaseMecha::InputName::MapOnOff));
	noHoldMaskFlgs.SetBitTrue(ChStd::EnumCast(BaseMecha::InputName::Release));
	noHoldMaskFlgs.SetBitTrue(ChStd::EnumCast(BaseMecha::InputName::UseTargetLooker));
	//noHoldMaskFlgs.SetBitTrue(ChStd::EnumCast(BaseMecha::InputName::WeaponUpChange));
	noHoldMaskFlgs.SetBitTrue(ChStd::EnumCast(BaseMecha::InputName::RWUChange));
	noHoldMaskFlgs.SetBitTrue(ChStd::EnumCast(BaseMecha::InputName::LWUChange));
	//noHoldMaskFlgs.SetBitTrue(ChStd::EnumCast(BaseMecha::InputName::WeaponDownChange));
	noHoldMaskFlgs.SetBitTrue(ChStd::EnumCast(BaseMecha::InputName::RWDChange));
	noHoldMaskFlgs.SetBitTrue(ChStd::EnumCast(BaseMecha::InputName::LWDChange));
	noHoldMaskFlgs.SetBitTrue(ChStd::EnumCast(BaseMecha::InputName::MoveUpChange));
	noHoldMaskFlgs.SetBitTrue(ChStd::EnumCast(BaseMecha::InputName::MoveDownChange));

	isNoHoldMaskFlgInit = true;
}


BaseMecha::BaseMecha()
{
	InitNoHoldMaskFlgs();

	physics->SetLeftRightWallLen(3.0f);
	physics->SetFrontBackWallLen(3.0f);
}

BaseMecha::~BaseMecha()
{

}

void BaseMecha::Create(const ChVec2& _viewSize, ChD3D11::Shader::BaseDrawMesh11<wchar_t>& _drawer, GameFrame* _frame)
{
	viewSize = _viewSize;
	drawer = &_drawer;
	frame = _frame;
	physics->Init();
	if (ChPtr::NullCheck(_frame))return;
	mechasNo = frame->GetMechas().size();
}

void BaseMecha::Load(ID3D11Device* _device, const std::wstring& _fileName)
{
	std::wstring text = L"";

	ChCpp::CharFile file;
	file.FileOpen(_fileName, false);
	text = ChStr::GetUTF16FromUTF8(file.FileRead());
	file.FileClose();

	auto&& json = ChCpp::JsonBaseType<wchar_t>::GetParameter(text);
	auto&& jsonObject = ChPtr::SharedSafeCast<ChCpp::JsonObject<wchar_t>>(json);

	if (jsonObject == nullptr)return;

	GetComponentObject<EnergyComponent>();
	GetComponentObject<MoveComponent>();
	GetComponentObject<CameraComponent>();
	GetComponentObject<WeaponComponent>();

	LoadPartsList(_device, jsonObject);
}

void BaseMecha::LoadPartsList(ID3D11Device* _device, ChPtr::Shared<ChCpp::JsonObject<wchar_t>> _jsonObject)
{
	if (_jsonObject == nullptr) return;
	SetComponent<LookAnchor>();

	auto&& jsonName = _jsonObject->GetJsonString(JSON_MECHA_NAME);
	if (jsonName != nullptr)
		mechaName = jsonName->GetString();

	auto&& coreObject = _jsonObject->GetJsonObject(JSON_CORE);

	if (coreObject == nullptr)return;

	core = MechaParts::LoadParts(*this, _device, drawer, frame, coreObject);

	testCollider.SetScalling(baseHitSize);

	nowDurable = durable;
	physics->SetMass(mass);

}

void BaseMecha::LoadEnd()
{
	auto&& com = GetComponentObject<CameraComponent>();
	com->SetViewHorizontal(physics->GetRotation().y);
}

void BaseMecha::Save(const std::wstring& _fileName)
{

	ChPtr::Shared<ChCpp::JsonObject<wchar_t>> res = SavePartsList();

	ChCpp::CharFile file;
	file.FileOpen(_fileName, true);
	std::wstring testFromJson = ChCpp::JsonBaseType<wchar_t>::FormatDocument(res->GetRawData());
	std::string textFromComverter = ChStr::GetUTF8FromUTF16(testFromJson);
	file.FileWrite(textFromComverter);
	file.FileClose();

}

ChPtr::Shared<ChCpp::JsonObject<wchar_t>> BaseMecha::SavePartsList()
{
	auto&& res = ChPtr::Make_S<ChCpp::JsonObject<wchar_t>>();
	res->Set(JSON_MECHA_NAME, ChCpp::JsonString<wchar_t>::CreateObject(mechaName));
	res->Set(JSON_CORE, core->Serialize());

	return res;
}

void BaseMecha::Release()
{
	if (core == nullptr)return;
	core->Release();
}

void BaseMecha::Update()
{
	if (breakFlg)
	{
		nowObjectDestroyCount++;
		if (nowObjectDestroyCount < OBJECT_DESTROY_COUNT)return;

		Destroy();
		return;
	}

	if (core == nullptr)return;
	core->UpdateFunction();
}

void BaseMecha::UpdateEnd()
{
	if (breakFlg)return;
	if (core == nullptr)return;

	core->UpdateEndFunction();

	physics->Update();

	float moveSize = physics->GetAddMovePowerVector().GetLen();

	testCollider.SetScalling(moveSize + baseHitSize);

}

void BaseMecha::Move()
{
	if (breakFlg)return;
	if (core == nullptr)return;
	core->MoveFunction();

	BaseMove();
}

void BaseMecha::MoveEnd()
{
	for (size_t i = 0; i < inputFlgs.GetSize(); i++)
	{
		beforeInputFlgs.SetValue(inputFlgs.GetValue(i), i);
	}

	inputFlgs.SetAllDownFlg();

	damageDir = ChVec3();

	auto&& camera = GetComponentObject<CameraComponent>();
	if (!isSelfViewHorizontalFlg)
	{
		auto&& rotate = physics->GetRotation();
		rotate.y = camera->GetViewHorizontal();
		physics->SetRotation(rotate);
	}

	camera->SetCenterPos(centerPos);

	if (hitEffectDrawFrame < 0)return;
	hitEffectDrawFrame--;
}

void BaseMecha::AddAnchorData(const ChVec3& _size, const ChLMat& _drawMat)
{
	auto&& anchor = GetComponent<LookAnchor>();
	if (anchor == nullptr)return;
	anchor->AddLookAnchorPosition(_size, _drawMat);
}

void BaseMecha::BaseMove()
{
	physics->SetPosition(physics->GetPosition() + physics->GetAddMovePowerVector());
	physics->SetRotation(physics->GetRotation() + physics->GetAddRotatePowerVector());

	physics->SetAddRotatePowerVector(0.0f);

	ChVec3 pos = physics->GetPosition();
	ChVec3 normal = (pos - centerPos);
	normal.y = 0.0f;
	centerPos.y = pos.y + CameraComponent::CAMERA_Y_POS;

	testCollider.SetPosition(pos);

	float tmp = normal.GetLen();

	float tmpDis = tmp > CENTER_LEN ? CENTER_LEN - tmp : 0.0f;

	normal.val.SetLen(tmp * 0.15f - tmpDis * 0.8f);
	centerPos += normal;
}

void BaseMecha::Draw3D()
{
	if (core == nullptr)return;
	beforeDrawMat.SetRotationYAxis(ChMath::ToRadian(physics->GetRotation().y));
	beforeDrawMat.SetPosition(physics->GetPosition());

	core->SetFrameTransform(beforeDrawMat);
	core->Draw3DFunction();
}

void BaseMecha::Draw2D()
{
	if (core == nullptr)return;
	core->Draw2DFunction();
}

void BaseMecha::DrawEnd()
{
	CreateDamageSmoke();
}

void BaseMecha::Deserialize(const std::wstring& _fileName)
{

}

std::wstring BaseMecha::Serialize()
{
	return L"";
}

void BaseMecha::SetTeamNo(const unsigned long _team)
{
	auto controller = GetComponent<ControllerBase>();
	if (controller == nullptr)return;
	controller->SetTeamNo(_team);
}

void BaseMecha::SetGroundHeight(const float _height)
{
	if (physics->GetGroundHeight() < _height)return;

	physics->SetGroundHeight(_height);
}

void BaseMecha::SetHitEffectDrawFrame()
{
	//if (hitEffectDrawFrame >= 0)return;
	hitEffectDrawFrame = HIT_EFFECT_DRAW_FRAME;
}

void BaseMecha::AddMoveVector(const ChVec3& _moveVecAdd)
{
	physics->AddMovePowerVector(_moveVecAdd);
}

void BaseMecha::AddRotateVector(const ChVec3& _rotateVecAdd)
{
	physics->AddRotatePowerVector(_rotateVecAdd);
}

void BaseMecha::SetPosition(const ChVec3& _pos)
{
	physics->SetPosition(_pos);
	centerPos = _pos;
}

void BaseMecha::SetRotation(const ChVec3& _rot)
{
	physics->SetRotation(_rot);
}

void BaseMecha::SetPushFlg(const InputName _inputFlgName)
{
	inputFlgs.SetBitTrue(ChStd::EnumCast(_inputFlgName));
}

ChVec3 BaseMecha::GetPosition()
{
	return physics->GetPosition();
}

ChVec3 BaseMecha::GetRotation()
{
	return physics->GetRotation();
}

size_t BaseMecha::GetTeamNo()
{
	auto controller = GetComponent<ControllerBase>();
	if (controller == nullptr)return ControllerBase::EMPTY_TEAM_NO;
	return controller->GetTeamNo();
}

long BaseMecha::GetHitEffectDrawStartFrame()
{
	return HIT_EFFECT_DRAW_FRAME;
}

size_t BaseMecha::GetAnchorRegistNum()
{
	auto&& anchor = GetComponent<LookAnchor>();
	if (anchor == nullptr)return 0;

	return anchor->GetPositionListCount();
}

ChPtr::Shared<PartsParameters> BaseMecha::GetAllParameters()
{
	auto res = ChPtr::Make_S<PartsParameters>();

	AddChildParameters(*res, core);

	return res;
}

void BaseMecha::AddChildParameters(PartsParameters& _parameter, ChPtr::Shared<MechaPartsObject> _nowParts)
{
	if (_nowParts == nullptr)return;
	auto&& baseParts = _nowParts->GetBaseObject();
	if (ChPtr::NullCheck(baseParts))return;
	baseParts->SetParameters(*_nowParts);
	auto&& parameter = _nowParts->GetPartsParameters();
	if (parameter == nullptr)return;
	_parameter += (*parameter);

	auto&& nextPosList = baseParts->GetComponents<NextPosData>();

	for (auto&& nextPos : nextPosList)
	{
		std::wstring nextPosName = nextPos->GetConnectionName();
		AddChildParameters(_parameter, _nowParts->GetChildParts(nextPosName));
	}
}

void BaseMecha::RemoveCore()
{
	if (core == nullptr)return;
	core->Destroy();
	core = nullptr;
}

bool BaseMecha::IsPushFlg(InputName _name)
{
	auto inputFlg = ChStd::EnumCast(_name);
	return !noHoldMaskFlgs.GetBitFlg(inputFlg) ?
		inputFlgs.GetBitFlg(inputFlg) :
		inputFlgs.GetBitFlg(inputFlg) && !beforeInputFlgs.GetBitFlg(inputFlg);
}

void BaseMecha::UpdateAnchor(size_t _no, const ChLMat& _drawMat)
{
	auto&& anchor = GetComponent<LookAnchor>();
	if (anchor == nullptr)return;

	anchor->UpdateLookAnchorPosition(_no, _drawMat);
}


void BaseMecha::AddSmokeCreatePos(ChPtr::Shared<ChCpp::TransformObject<wchar_t>> _pos, MechaPartsObject* _parts)
{
	auto pos = ChPtr::Make_S<DamageSmokePosData>();
	pos->pos = _pos;
	pos->haveParts = _parts;
	damageSmokeCreatePos.push_back(pos);
}

void BaseMecha::SubSmokeCreatePos(MechaPartsObject* _parts)
{
	for (size_t i = 0; i < damageSmokeCreatePos.size(); i)
	{
		if (damageSmokeCreatePos[i]->haveParts == _parts)
		{
			damageSmokeCreatePos.erase(damageSmokeCreatePos.begin() + i);
			continue;
		}
		i++;
	}
}

void BaseMecha::CreateDamageSmoke()
{
	if (ChPtr::NullCheck(frame))return;

	if (nowDurable < 25.0f &&
		(damageSmokePosNum.size() <= 1 &&
		damageSmokePosNum.size() < damageSmokeCreatePos.size()))
	{
		for (size_t i = 0; damageSmokePosNum.size() < damageSmokeCreatePos.size() && i < 2; i++)
		{
			AddSmokePosNum();
		}
	}

	if (nowDurable < 50.0f &&
		(damageSmokePosNum.size() <= 0))
	{
		AddSmokePosNum();
	}

	if (damageSmokePosNum.size() <= 0)return;

	createDamageSmokeTime++;

	if (createDamageSmokeTime <= (CREATE_SMOKE_EFFECT_TIME))return;
	createDamageSmokeTime = 0;

	for (size_t i = 0; i < damageSmokePosNum.size(); i++)
	{
		ChVec3 pos;

		ChLMat tmpMat
			= damageSmokeCreatePos[damageSmokePosNum[i]]->pos->GetDrawLHandMatrix()
			* damageSmokeCreatePos[damageSmokePosNum[i]]->haveParts->GetDrawLHandMatrix();

		pos = tmpMat.Transform(ChVec3());

		frame->AddSmokeEffectObject(pos, ChVec3(0.0f, 1.0f, 0.0f));

	}
}
void BaseMecha::AddSmokePosNum()
{
	bool addFlg = false;
	bool continueFlg = false;
	do
	{
		continueFlg = false;
		int num = rand() % damageSmokeCreatePos.size();
		for (size_t i = 0; i < damageSmokePosNum.size(); i++)
		{
			if (damageSmokePosNum[i] == num)
			{
				continueFlg = true;
				break;
			}
		}

		if (continueFlg)continue;


		damageSmokePosNum.push_back(num);
		addFlg = true;

	} while (!addFlg);

}

void BaseMecha::SetTestHitSize(const ChVec3& _hitSize)
{
	ChVec3 tmpHitSize = _hitSize;

	tmpHitSize.Abs();
	if (baseHitSize < tmpHitSize.x)baseHitSize = _hitSize.x;
	if (baseHitSize < tmpHitSize.y)baseHitSize = _hitSize.y;
	if (baseHitSize < tmpHitSize.z)baseHitSize = _hitSize.z;
}

void BaseMecha::TestBulletHit(AttackObject& _obj)
{
	if (core == nullptr)return;
	if (_obj.IsUseMechaTest(mechasNo))return;
	if (_obj.IsHit())return;
	if (_obj.IsUseMechaTeamTest(GetTeamNo()))
	{
		if (!frame->IsFriendryFireFlg())return;
	}
	if (IsBreak())return;

	ChVec3 dir = _obj.GetMovePower();

	float moveLen = dir.GetLen();

	float hitSize = _obj.GetHitSize();

	ChVec3 pos = _obj.GetPosition();

	testAttackCollider.SetScalling(hitSize + moveLen);

	testAttackCollider.SetPosition(pos);

	if (!testCollider.IsHit(&testAttackCollider))return;

	dir.Normalize();

	unsigned long cutCount = static_cast<unsigned long>(moveLen / hitSize) + 1;

	testAttackCollider.SetScalling(hitSize);

	ChVec3 nowVector = dir;
	float damage = 0.0f;
	float nowPos = 0.0f;
	for (unsigned long i = 0; i < cutCount; i++)
	{
		nowPos = hitSize * i;
		if (nowPos <= 0.0f)nowPos = 1.0f;
		nowVector.val.SetLen(nowPos);

		testAttackCollider.SetPosition(pos + nowVector);

		damage = core->GetDamage(testAttackCollider, _obj);

		if (damage <= 0.0f)continue;
		nowDurable -= damage;
		//damageDir = testAttackCollider.GetPos() - physics->GetPosition();
		damageDir = dir * -1.0f;
		damageDir.Normalize();
		_obj.SetMovePower(nowVector);
		_obj.SetIsHitTrue();

		break;
	}

	if (breakFlg)return;
	if (nowDurable > 0)return;

	Break();

}

void BaseMecha::Break()
{
	frame->BreakMecha(this);
	breakFlg = true;

	DestroyComponent();
}
