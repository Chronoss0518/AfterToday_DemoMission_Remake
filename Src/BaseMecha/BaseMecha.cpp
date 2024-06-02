
#include"../BaseIncluder.h"

#include"../AllStruct.h"

#include"../Attack/Attack.h"
#include"MechaParts.h"
#include"MechaPartsObject.h"
#include"BaseMecha.h"
#include"../Frames/GameFrame.h"
#include"FunctionComponent/WeaponComponents.h"
#include"FunctionComponent/BoostComponent.h"
#include"../Attack/AttackObject.h"
#include"Controller/ControllerBase.h"

#include"CPU/CPULooker.h"

#define CAMERA_Y_POS 4.0f

#define CENTER_LEN 5.0f

#define HIT_EFFECT_DRAW_FRAME static_cast<long>(BASE_FPS * 2.0f)


#define JSON_MECHA_NAME "Name"
#define JSON_CORE "Core"

static const std::string partsTypeName[]
{
	"Body","Head","Foot","RightArm","LeftArm","Boost","Weapon","Extra"
};

static const std::string weaponTypeName[]
{
	"R*","L*"
};

BaseMecha::BaseMecha()
{
	physics->SetLeftRightWallLen(3.0f);
	physics->SetFrontBackWallLen(3.0f);
}

BaseMecha::~BaseMecha()
{

}

void BaseMecha::Create(const ChVec2& _viewSize, ChD3D11::Shader::BaseDrawMesh11& _drawer, GameFrame* _frame)
{
	viewSize = _viewSize;
	drawer = &_drawer;
	frame = _frame;
	physics->Init();
	if (ChPtr::NullCheck(_frame))return;
	mechasNo = frame->GetMechas().size();
}

void BaseMecha::Load(ID3D11Device* _device, const std::string& _fileName)
{

	std::string text = "";

	ChCpp::CharFile file;
	file.FileOpen(_fileName);
	text = file.FileReadText();
	file.FileClose();

	auto&& jsonObject = ChPtr::SharedSafeCast<ChCpp::JsonObject>(ChCpp::JsonBaseType::GetParameter(text));

	LoadPartsList(_device, jsonObject);
}

void BaseMecha::LoadPartsList(ID3D11Device* _device, ChPtr::Shared<ChCpp::JsonObject> _jsonObject)
{
	if (_jsonObject == nullptr) return;
	SetComponent<LookAnchor>();

	mechaName = *_jsonObject->GetJsonString(JSON_MECHA_NAME);

	auto&& coreObject = _jsonObject->GetJsonObject(JSON_CORE);

	core = MechaParts::LoadParts(*this, _device, drawer, frame, coreObject);

	testCollider.SetScalling(baseHitSize);

	nowEnelgy = maxEnelgy;
	nowDurable = durable;
	physics->SetMass(mass);


	auto boostComponent = GetComponent<BoostComponent>();

	if (boostComponent != nullptr)boostComponent->BoostDrawEnd();

}

void BaseMecha::Save(const std::string& _fileName)
{

	ChPtr::Shared<ChCpp::JsonObject> res = SavePartsList();

	ChCpp::CharFile file;
	file.FileOpen(_fileName);
	file.FileWriteText(ChCpp::JsonBaseType::FormatDocument(res->GetRawData()));
	file.FileClose();

}

ChPtr::Shared<ChCpp::JsonObject> BaseMecha::SavePartsList()
{
	auto&& res = ChPtr::Make_S<ChCpp::JsonObject>();
	res->Set(JSON_MECHA_NAME, ChCpp::JsonString::CreateObject(mechaName));
	res->Set(JSON_CORE, core->Serialize());

	return res;
}

void BaseMecha::Release()
{
	core->Release();
}

void BaseMecha::Update()
{
	core->Update();
}

void BaseMecha::UpdateEnd()
{
	unsigned long fps = ChSystem::SysManager().GetFPS();

	nowEnelgy += (chargeEnelgy * (120 / fps));

	nowEnelgy = nowEnelgy > maxEnelgy ? maxEnelgy : nowEnelgy;


	physics->Update();

	float moveSize = physics->GetAddMovePowerVector().Len();

	testCollider.SetScalling(moveSize + baseHitSize);

}

void BaseMecha::Move()
{

	BaseMove();

	inputFlgs.SetAllDownFlg();

}

void BaseMecha::MoveEnd()
{

	damageDir = ChVec3();

	viewHorizontal = physics->GetRotation().y;

	auto viewPos = GetViewPos();
	auto viewLookPos = GetViewLookPos();

	ChMat_11 tmpMat;
	tmpMat.CreateViewMatLookTarget(viewPos, viewLookPos, ChVec3(0.0f, 1.0f, 0.0f));

	viewMat = tmpMat;

	auto&& objectLooker = GetComponent<CPUObjectLooker>();

	if (objectLooker == nullptr)return;

	objectLooker->SetViewMatrix(viewMat);

	if (hitEffectDrawFrame < 0)return;
	hitEffectDrawFrame--;
}

void BaseMecha::AddAnchorData(const ChVec3& _size, const ChLMat& _drawMat)
{
	auto&& anchor = GetComponent<LookAnchor>();
	if (anchor == nullptr)return;
	anchor->AddLookAnchorPosition(_size, _drawMat);
}

void BaseMecha::AddLeftWeaponData(ChPtr::Shared<MechaPartsObject>_partsObject)
{
	if (_partsObject == nullptr)return;
	auto&& weap = GetComponentObject<LeftWeaponComponent>();

	weap->AddWeapon(_partsObject);
	_partsObject->SetLWeapon(true);
}

void BaseMecha::AddRightWeaponData(ChPtr::Shared<MechaPartsObject>_partsObject)
{
	if (_partsObject == nullptr)return;
	auto&& weap = GetComponentObject<RightWeaponComponent>();

	weap->AddWeapon(_partsObject);
	_partsObject->SetRWeapon(true);
}

ChVec3 BaseMecha::GetViewPos()
{

	ChLMat camYMat, camXMat;

	camYMat.SetRotationYAxis(ChMath::ToRadian(viewHorizontal));
	camXMat.SetRotationXAxis(-ChMath::ToRadian(viewVertical));
	camYMat = camXMat * camYMat;
	camYMat.SetPosition(centerPos + ChVec3(0.0f, CAMERA_Y_POS, 0.0f));

	return camYMat.Transform(ChVec3(0.0f, 0.0f, -15.0f));
}

ChVec3 BaseMecha::GetViewLookPos()
{

	ChLMat camYMat, camXMat;

	camYMat.SetRotationYAxis(ChMath::ToRadian(viewHorizontal));
	camXMat.SetRotationXAxis(-ChMath::ToRadian(viewVertical));
	camYMat = camXMat * camYMat;
	camYMat.SetPosition(centerPos + ChVec3(0.0f, CAMERA_Y_POS - 2.0f, 0.0f));

	return camYMat.Transform(ChVec3(0.0f, 0.0f, 5.0f));
}

void BaseMecha::BaseMove()
{
	physics->SetPosition(physics->GetPosition() + physics->GetAddMovePowerVector());
	physics->SetRotation(physics->GetRotation() + physics->GetAddRotatePowerVector());

	ChVec3 pos = physics->GetPosition();
	ChVec3 normal = (pos - centerPos);
	normal.y = 0.0f;
	centerPos.y = pos.y + CAMERA_Y_POS;

	testCollider.SetPosition(pos);

	float tmp = normal.Len() - CENTER_LEN;
	if (tmp < 0)return;
	normal.Normalize();
	normal.val.SetLen(tmp);
	centerPos += normal;
}

void BaseMecha::Draw3D()
{
	ChLMat drawMat;
	drawMat.SetRotationYAxis(ChMath::ToRadian(physics->GetRotation().y));
	drawMat.SetPosition(physics->GetPosition());

	auto boostComponent = GetComponent<BoostComponent>();

	if (boostComponent != nullptr)boostComponent->BoostDrawBegin();

	core->Draw(drawMat);

	if (boostComponent != nullptr)boostComponent->BoostDrawEnd();

}

void BaseMecha::Draw2D()
{
	if (ChPtr::NullCheck(frame))return;

	


}

void BaseMecha::Deserialize(const std::string& _fileName)
{

}

std::string BaseMecha::Serialize()
{
	return "";
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

unsigned long BaseMecha::GetTeamNo()
{
	auto controller = GetComponent<ControllerBase>();
	if (controller == nullptr)return 0;
	return controller->GetTeamNo();
}

long BaseMecha::GetHitEffectDrawStartFrame()
{
	return HIT_EFFECT_DRAW_FRAME;
}

unsigned long BaseMecha::GetAnchorRegistNum()
{
	auto&& anchor = GetComponent<LookAnchor>();
	if (anchor == nullptr)return 0;

	return anchor->GetPositionListCount();
}

void BaseMecha::UpdateAnchor(unsigned long _no, const ChLMat& _drawMat)
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
	if (_obj.IsUseMechaTest(mechasNo))return;
	if (_obj.IsHit())return;
	if (_obj.IsUseMechaTeamTest(GetTeamNo()))
	{
		if (!frame->IsFriendryFireFlg())return;
	}
	if (IsBreak())return;

	ChVec3 dir = _obj.GetMovePower();

	float moveLen = dir.Len();

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
	
	Destroy();

	breakFlg = true;
}

void BaseMecha::Break()
{
	frame->BreakMecha(this);
}