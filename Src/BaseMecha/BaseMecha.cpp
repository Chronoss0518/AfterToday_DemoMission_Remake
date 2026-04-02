
#include"../BaseIncluder.h"

#include"../AllStruct.h"

#include"../Attack/Attack.h"
#include"MechaParts.h"
#include"MechaPartsObject.h"
#include"BaseMecha.h"
#include"../Frames/GameFrame.h"
#include"FunctionComponent/WeaponComponent.h"
#include"FunctionComponent/BoostComponent.h"
#include"FunctionComponent/EnergyComponent.h"
#include"FunctionComponent/MoveComponent.h"
#include"FunctionComponent/CameraComponent.h"

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

static const std::wstring partsTypeName[]
{
	L"Body",L"Head",L"Foot",L"RightArm",L"LeftArm",L"Boost",L"Weapon",L"Extra"
};

static const std::wstring weaponTypeName[]
{
	L"R*",L"L*"
};

BaseMecha::BaseMecha()
{
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
	//GetComponentObject<RightWeaponComponent>();
	//GetComponentObject<LeftWeaponComponent>();

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
	com->SetViewVerticial(true, physics->GetRotation().y);
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

	inputFlgs.SetAllDownFlg();
}

void BaseMecha::MoveEnd()
{
	damageDir = ChVec3();

	auto&& camera = GetComponentObject<CameraComponent>();
	camera->SetViewVerticial(!isSelfViewHorizontalFlg, physics->GetRotation().y);
	camera->SetCenterPos(centerPos);

	camera->UpdateCamera();

	auto&& objectLooker = GetComponent<CPUObjectLooker>();

	if (objectLooker == nullptr)return;

	objectLooker->SetViewMatrix(camera->GetViewMat());

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
	ChLMat drawMat;
	drawMat.SetRotationYAxis(ChMath::ToRadian(physics->GetRotation().y));
	drawMat.SetPosition(physics->GetPosition());

	core->SetOutSideTransform(drawMat);
	core->Draw3DFunction();

}

void BaseMecha::Draw2D()
{
	if (core == nullptr)return;
	core->Draw2DFunction();
}

void BaseMecha::DrawEnd()
{
	if (core == nullptr)return;
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

ChVec3 BaseMecha::GetPosition()
{
	return physics->GetPosition();
}

ChVec3 BaseMecha::GetRotation()
{
	return physics->GetRotation();
}

ChLMat BaseMecha::GetViewMat()
{
	auto&& camera = GetComponentObject<CameraComponent>();
	return camera->GetViewMat();
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

void BaseMecha::UpdateAnchor(size_t _no, const ChLMat& _drawMat)
{
	auto&& anchor = GetComponent<LookAnchor>();
	if (anchor == nullptr)return;

	anchor->UpdateLookAnchorPosition(_no, _drawMat);
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
