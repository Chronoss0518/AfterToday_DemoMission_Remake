#include"../../BaseIncluder.h"

#include"../../AllStruct.h"

#include"../BaseMecha.h"

#include"../../Frames/GameFrame.h"

#include"ControllerBase.h"

#include"CPUFunction/CPUFunctions.h"

#define DEFAULT_CURSOL_MOVE_SIZE 0.2f
#define MAX_LOOK_LEN  10000.0f

void ControllerBase::Input(const InputName _inputFlgName)
{
	auto targetMecha = LookObj<BaseMecha>();
	if (ChPtr::NullCheck(targetMecha))Destroy();

	targetMecha->SetPushFlg(_inputFlgName);

}

void PlayerController::Init()
{
	auto&& windows = *ChSystem::SysManager().GetSystem<ChSystem::Windows>();
	windSize.h = static_cast<float>(windows.GetWindHeight());
	windSize.w = static_cast<float>(windows.GetWindWidth());
	mouse->Init(windows);
	
	mouse->SetCenterFixedFlg(true);
	mouse->SetVisibleFlg(true);

	keyTypes[VK_SPACE] = InputName::Jump;
	keyTypes[VK_SHIFT] = InputName::Avo;
	keyTypes[VK_CONTROL] = InputName::Boost;

	keyTypes['Q'] = InputName::SetCameraCenter;
	keyTypes['E'] = InputName::MapOnOff;
	keyTypes['W'] = InputName::Front;
	keyTypes['A'] = InputName::Left;
	keyTypes['D'] = InputName::Right;
	keyTypes['S'] = InputName::Back;

	keyTypes[VK_LEFT] = InputName::LeftRotation;
	keyTypes[VK_RIGHT] = InputName::RightRotation;
	
	cursolInput[ChStd::EnumCast(CursolMoveTypeName::Left)] = InputName::LeftRotation;
	cursolInput[ChStd::EnumCast(CursolMoveTypeName::Right)] = InputName::RightRotation;

	cursolInput[ChStd::EnumCast(CursolMoveTypeName::Up)] = InputName::CameraUpRotation;
	cursolInput[ChStd::EnumCast(CursolMoveTypeName::Down)] = InputName::CameraDownRotation;

	keyTypes[VK_LBUTTON] = InputName::LAttack;
	keyTypes[VK_RBUTTON] = InputName::RAttack;

	//keyTypes[VK_CONTROL] = InputName::MSChange;
	keyTypes[VK_MBUTTON] = InputName::WeaponDownChange;
	keyTypes[VK_SCROLL] = InputName::MagnificationUp;
	
	controller.Init();

	controllerTypes[XInputTypeNames::A] = InputName::Attack;
	controllerTypes[XInputTypeNames::B] = InputName::Avo;
	controllerTypes[XInputTypeNames::X] = InputName::MapOnOff;
	controllerTypes[XInputTypeNames::Y] = InputName::Boost;
	controllerTypes[XInputTypeNames::LTop] = InputName::Front;
	controllerTypes[XInputTypeNames::LLeft] = InputName::Left;
	controllerTypes[XInputTypeNames::LDown] = InputName::Back;
	controllerTypes[XInputTypeNames::LRight] = InputName::Right;
	controllerTypes[XInputTypeNames::RTop] = InputName::None;
	controllerTypes[XInputTypeNames::RLeft] = InputName::None;
	controllerTypes[XInputTypeNames::RDown] = InputName::None;
	controllerTypes[XInputTypeNames::RRight] = InputName::None;
	controllerTypes[XInputTypeNames::L1] = InputName::LAttack;
	controllerTypes[XInputTypeNames::L2] = InputName::LWDChange;
	controllerTypes[XInputTypeNames::L3] = InputName::LReload;
	controllerTypes[XInputTypeNames::R1] = InputName::RAttack;
	controllerTypes[XInputTypeNames::R2] = InputName::RWDChange;
	controllerTypes[XInputTypeNames::R3] = InputName::RReload;

}

void PlayerController::UpdateBegin()
{
	auto targetMecha = LookObj<BaseMecha>();
	if (ChPtr::NullCheck(targetMecha))
	{
		Destroy();
		return;
	}

	XInputUpdate();
	
	for (auto&& key : keyTypes)
	{
		if (!ChSystem::SysManager().IsPushKey(key.first))continue;
		targetMecha->SetPushFlg(key.second);
	}

	CursolUpdate();
}

void PlayerController::XInputUpdate()
{
	auto targetMecha = LookObj<BaseMecha>();
	controller.Update();

	if (controller.GetAFlg())targetMecha->SetPushFlg(controllerTypes[XInputTypeNames::A]);
	if (controller.GetBFlg())targetMecha->SetPushFlg(controllerTypes[XInputTypeNames::B]);
	if (controller.GetXFlg())targetMecha->SetPushFlg(controllerTypes[XInputTypeNames::X]);
	if (controller.GetYFlg())targetMecha->SetPushFlg(controllerTypes[XInputTypeNames::Y]);
	if (controller.GetAFlg())targetMecha->SetPushFlg(controllerTypes[XInputTypeNames::A]);
	if (controller.GetAFlg())targetMecha->SetPushFlg(controllerTypes[XInputTypeNames::A]);
	if (controller.GetAFlg())targetMecha->SetPushFlg(controllerTypes[XInputTypeNames::A]);
	if (controller.GetAFlg())targetMecha->SetPushFlg(controllerTypes[XInputTypeNames::A]);
	if (controller.GetAFlg())targetMecha->SetPushFlg(controllerTypes[XInputTypeNames::A]);
	if (controller.GetAFlg())targetMecha->SetPushFlg(controllerTypes[XInputTypeNames::A]);
	

}

void PlayerController::CursolUpdate()
{
	mouse->Update();
	
	nowPos += mouse->GetMoveValueToChVec2();
	
	ChVec2 vector = nowPos;

	CursolFunction(vector.x, windSize.w, CursolMoveTypeName::Right, CursolMoveTypeName::Left);
	CursolFunction(vector.y, windSize.h, CursolMoveTypeName::Up, CursolMoveTypeName::Down);

	nowPos -= vector;
}

void PlayerController::CursolFunction(float& _value, const float _windSize, const CursolMoveTypeName _plus, const CursolMoveTypeName _minus)
{
	float mathMoveSencsitivility = 1.0f - moveSensitivility;

	float mathWindSize = _windSize * mathMoveSencsitivility * DEFAULT_CURSOL_MOVE_SIZE;

	auto targetMecha = LookObj<BaseMecha>();
	if (std::abs(_value) > mathWindSize)
	{
		if (_value > 0)
		{
			targetMecha->SetPushFlg(cursolInput[ChStd::EnumCast(_plus)]);
			_value -= mathWindSize;
		}
		else
		{
			targetMecha->SetPushFlg(cursolInput[ChStd::EnumCast(_minus)]);
			_value += mathWindSize;
		}
	}
}

void CPUController::Init()
{

}

void CPUController::UpdateBegin()
{
	if (ChPtr::NullCheck(frame))
	{
		LookObj()->Destroy();
		return;
	}
	
	FindMecha();

	for (auto&& function : cpuFunctions)
	{
		if (nowActionMoveTime > 0)continue;
		if (function->Update(*this))break;
	}

}

void CPUController::LoadCPUData(const std::string& _fileName)
{
	ChCpp::TextObject textData;
	
	{
		ChCpp::File<> file;
		file.FileOpen(CPU_DIRECTORY(+_fileName));
		textData.SetText(file.FileReadText());
		file.FileClose();
	}

	actionMoveTime = ChStr::GetIntegialFromText<unsigned long>(textData.GetTextLine(0));

	for (unsigned long i = 1; i < textData.Count(); i++)
	{

	}

}

void CPUController::FindMecha()
{
	auto mecha = LookObj<BaseMecha>();
	if (ChPtr::NullCheck(mecha))
	{
		Destroy();
		return;
	}

	float nearLength = MAX_LOOK_LEN;
	float farLength = 0.0f;


	for (auto&& otherMecha : frame->GetMechaList().GetObjectList<BaseMecha>())
	{
		auto otherMechaObject = otherMecha.lock();
		if (otherMechaObject == nullptr)continue;
		if (otherMechaObject.get() == mecha)continue;
		auto controller = otherMechaObject->GetComponent<ControllerBase>();
		if (controller == nullptr)continue;
		if (controller->GetTeamNo() == GetTeamNo())continue;
		ChVec3 targetPos = otherMechaObject->GetPosition();

		ChVec4 targetProPos = viewMat.Transform(targetPos);
		targetProPos = proMat.Transform(targetProPos);

		targetProPos.x = targetProPos.x / targetProPos.w;
		targetProPos.x *= targetProPos.x;
		targetProPos.y = targetProPos.y / targetProPos.w;
		targetProPos.y *= targetProPos.y;
		targetProPos.z = (targetProPos.z / targetProPos.w) * 2.0f - 1.0f;
		targetProPos.z *= targetProPos.z;

		if (1.0f - targetProPos.x < 0.0f)continue;
		if (1.0f - targetProPos.y < 0.0f)continue;
		if (1.0f - targetProPos.z < 0.0f)continue;

		float tmpLength = ChVec3::GetLen(targetProPos, mecha->GetPosition());
		if (nearLength > tmpLength)
		{
			nearLength = nearLength;
			nearEnemy.reset();
			nearEnemy = otherMecha;
			MenyDamageTest(nearManyDamageEnemy, otherMecha);
			FewDamageTest(nearFewDamageEnemy, otherMecha);

		}
		
		if (farLength < tmpLength)
		{
			farLength = nearLength;
			farEnemy.reset();
			farEnemy = otherMecha;
			MenyDamageTest(farManyDamageEnemy, otherMecha);
			FewDamageTest(farFewDamageEnemy, otherMecha);

		}
	}
}

void CPUController::MenyDamageTest(ChPtr::Weak<BaseMecha>& _base, ChPtr::Weak<BaseMecha>& _target)
{
	auto target = _target.lock();
	if (target == nullptr)return;

	auto base = _base.lock();
	if (base == nullptr)
	{
		_base = _target;
		return;
	}

	if (base->GetDamage() >= target->GetDamage())return;
	_base.reset();
	_base = _target;
}

void CPUController::FewDamageTest(ChPtr::Weak<BaseMecha>& _base, ChPtr::Weak<BaseMecha>& _target)
{
	auto target = _target.lock();
	if (target == nullptr)return;

	auto base = _base.lock();
	if (base == nullptr)
	{
		_base = _target;
		return;
	}

	if (base->GetDamage() <= target->GetDamage())return;
	_base.reset();
	_base = _target;
}
