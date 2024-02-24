#include"../../../BaseIncluder.h"

#include"../../../AllStruct.h"

#include"../../BaseMecha.h"

#include"../../../Frames/GameFrame.h"

#include"../../CPU/CPULooker.h"

#include"CPUMoveInput.h"
#include"CPUTargetSelector.h"
#include"CPUMovePositionSelector.h"

ChPtr::Shared<ChCpp::JsonObject> CPUMoveInput::Serialize()
{
	auto&& res = ChPtr::Make_S<ChCpp::JsonObject>();

	res->Set("LookTweakSize", ChCpp::JsonNumber::CreateObject(lookTweakSize));

	res->Set("TargetPositionLength", ChCpp::JsonNumber::CreateObject(targetPositionLength));

	res->Set("UseBoostLengthFromBattle", ChCpp::JsonNumber::CreateObject(useBoostLengthFromBattle));

	res->Set("UseBoostLengthFromUnBattle", ChCpp::JsonNumber::CreateObject(useBoostLengthFromUnBattle));

	return res;
}

void CPUMoveInput::Deserialize(const ChPtr::Shared<ChCpp::JsonObject>& _jsonObject)
{
	if (_jsonObject == nullptr)return;

	auto&& lookTweakSizeObject = _jsonObject->GetJsonNumber("LookTweakSize");
	if (lookTweakSizeObject != nullptr)lookTweakSize = *lookTweakSizeObject;

	auto&& targetPositionLengthObject = _jsonObject->GetJsonNumber("TargetPositionLength");
	if (targetPositionLengthObject != nullptr)targetPositionLength = *targetPositionLengthObject;

	auto&& useBoostLengthFromBattleObject = _jsonObject->GetJsonNumber("UseBoostLengthFromBattle");
	if (useBoostLengthFromBattleObject != nullptr)useBoostLengthFromBattle = *useBoostLengthFromBattleObject;

	auto&& useBoostLengthFromUnBattleObject = _jsonObject->GetJsonNumber("UseBoostLengthFromUnBattle");
	if (useBoostLengthFromUnBattleObject != nullptr)useBoostLengthFromUnBattle = *useBoostLengthFromUnBattleObject;
}

void CPUMoveInput::Update(
	CPUController& _controller, 
	CPUTargetSelector& _targetSelector,
	CPUMovePositionSelector& _movePositionSelector, 
	GameFrame& _frame)
{

	auto&& baseMecha = _controller.GetBaseMecha();

	if (ChPtr::NullCheck(baseMecha))return;

	bool isBattleFlg = _movePositionSelector.IsBattleFlg();

	//OutputDebugString("CPUMoveInput Update Start\n");

	auto moveDir = _movePositionSelector.GetMovePoint() - baseMecha->GetPosition();

	auto targetDir = moveDir;

	moveDir.Normalize();

	ChLMat tmpMat;

	auto lookDir = baseMecha->GetViewLookPos() - baseMecha->GetViewPos();

	lookDir.Normalize();

	auto sideCross = ChVec3::GetCross(moveDir, lookDir);
	float sideCos = ChVec3::GetCos(moveDir, lookDir);

	bool inputHoldFlg = true;

	if (sideCos > lookTweakSize)inputHoldFlg = false;

	if (sideCross.y > 0.0f)
	{
		_controller.Input(ControllerBase::InputName::CameraLeftRotation, inputHoldFlg);
		beforeCameraRotationInputName = ControllerBase::InputName::CameraLeftRotation;
	}

	if (sideCross.y < 0.0f)
	{
		_controller.Input(ControllerBase::InputName::CameraRightRotation, inputHoldFlg);
		beforeCameraRotationInputName = ControllerBase::InputName::CameraRightRotation;
	}

	if (sideCos < 0.0f)
	{
		if (sideCross.y == 0.0f)
		{
			auto tmp = rand() % 2;
			beforeCameraRotationInputName = tmp == 0 ? ControllerBase::InputName::CameraLeftRotation : ControllerBase::InputName::CameraRightRotation;
			_controller.Input(beforeCameraRotationInputName, inputHoldFlg);
		}
		return;
	}


	//if (moveDir.y < lookDir.y)_controller.Input(ControllerBase::InputName::CameraUpRotation, true);
	//if (moveDir.y > lookDir.y)_controller.Input(ControllerBase::InputName::CameraDownRotation, true);

	//OutputDebugString(("CrossSize x:[" + std::to_string(cross.x)+"] y:[" + std::to_string(cross.y) + "] z:[" + std::to_string(cross.z) + "]\n").c_str());

	//OutputDebugString("CPUMoveInput Update End\n");

	if (!_targetSelector.IsLookTarget())
	{
		MoveUnButtleTarget(_controller, false, targetDir);
		return;
	}

	unsigned long targetNo = _targetSelector.GetTargetNo();
	auto&& mechaList = _frame.GetMechaList().GetObjectList<BaseMecha>();

	if (mechaList.size() <= targetNo)
	{
		MoveUnButtleTarget(_controller, false, targetDir);
		return;
	}

	auto&& targetMecha = mechaList[targetNo];

	if (targetMecha.expired())
	{
		MoveUnButtleTarget(_controller, false, targetDir);
		return;
	}

	auto targetMechaObject = targetMecha.lock();

	auto&& targetPosition = targetMechaObject->GetPosition();

	float moveToTargetPositionLenght = (targetPosition - _movePositionSelector.GetMovePoint()).Len();

	MoveBattleTarget(_controller, _movePositionSelector.IsBattleFlg(), targetDir, _targetSelector.IsLookTarget());

	MoveUnButtleTarget(_controller, _movePositionSelector.IsBattleFlg(), targetDir);
}

void CPUMoveInput::MoveBattleTarget(CPUController& _controller, bool _isBattleFlg, const ChVec3& _dir, bool _isLookTarget)
{
	if (!_isBattleFlg)return;

	float length = _dir.Len();

	ChVec3 tmpDir = _dir;

	if (length > useBoostLengthFromBattle)_controller.Input(ControllerBase::InputName::Boost, true);

	auto moveInput = ControllerBase::InputName::Front;

	if (length < targetPositionLength && _isLookTarget)moveInput = ControllerBase::InputName::Back;

	if (tmpDir.y > 0.0f)
	{
		moveInput = ControllerBase::InputName::Up;
	}

	_controller.Input(moveInput, true);
}

void CPUMoveInput::MoveUnButtleTarget(CPUController& _controller, bool _isBattleFlg, const ChVec3& _dir)
{
	if (_isBattleFlg)return;

	float length = _dir.Len();

	ChVec3 tmpDir = _dir;

	if (length > useBoostLengthFromBattle)_controller.Input(ControllerBase::InputName::Boost, true);

	auto moveInput = ControllerBase::InputName::Front;

	if (length < 10.0f)moveInput = ControllerBase::InputName::Back;

	_controller.Input(moveInput, true);

	if(tmpDir.y > 0.0f)
	{
		_controller.Input(ControllerBase::InputName::Up, true);
	}

}
