#include"../../../BaseIncluder.h"

#include"../../../AllStruct.h"

#include"../../BaseMecha.h"

#include"../../../Frames/GameFrame.h"

#include"../../CPU/CPULooker.h"

#include"CPUMoveInput.h"
#include"CPUTargetSelector.h"
#include"CPUMovePositionSelector.h"

void CPUMoveInput::Update(
	CPUController& _controller, 
	CPUTargetSelector& _targetSelector,
	CPUMovePositionSelector& _movePositionSelector, 
	GameFrame& _frame)
{

	auto&& baseMecha = _controller.GetBaseMecha();

	if (ChPtr::NullCheck(baseMecha))return;

	//OutputDebugString("CPUMoveInput Update Start\n");

	auto moveDir = _movePositionSelector.GetMovePoint() - baseMecha->GetPosition();

	auto targetDir = moveDir;

	moveDir.Normalize();

	ChLMat tmpMat;

	

	auto lookDir = baseMecha->GetViewLookPos() - baseMecha->GetViewPos();

	lookDir.Normalize();

	auto cross = ChVec3::GetCross(moveDir, lookDir);

	if (cross.y > 0.0f)_controller.Input(ControllerBase::InputName::CameraLeftRotation, true);
	if (cross.y < 0.0f)_controller.Input(ControllerBase::InputName::CameraRightRotation, true);

	OutputDebugString(("CrossSize x:[" + std::to_string(cross.x)+"] y:[" + std::to_string(cross.y) + "] z:[" + std::to_string(cross.z) + "]\n").c_str());

	//OutputDebugString("CPUMoveInput Update End\n");

	if (!_targetSelector.IsLookTarget())
	{
		MoveUnButtleTarget(_controller, true, targetDir);
		return;
	}

	unsigned long targetNo = _targetSelector.GetTargetNo();
	auto&& mechaList = _frame.GetMechaList().GetObjectList<BaseMecha>();

	if (mechaList.size() <= targetNo)
	{
		MoveUnButtleTarget(_controller, true, targetDir);
		return;
	}

	auto&& targetMecha = mechaList[targetNo];

	if (targetMecha.expired())
	{
		MoveUnButtleTarget(_controller, true, targetDir);
		return;
	}

	auto targetMechaObject = targetMecha.lock();

	auto&& targetPosition = targetMechaObject->GetPosition();

	float moveToTargetPositionLenght = (targetPosition - _movePositionSelector.GetMovePoint()).Len();

	MoveBattleTarget(_controller, moveToTargetPositionLenght < nearTestLength, targetDir);

	MoveUnButtleTarget(_controller, moveToTargetPositionLenght > nearTestLength, targetDir);
}

void CPUMoveInput::MoveBattleTarget(CPUController& _controller, bool _isNearThePositionFlg, const ChVec3& _dir)
{
	if (!_isNearThePositionFlg)return;

	float length = _dir.Len();

	ChVec3 tmpDir = _dir;
	tmpDir.Abs();

	if (length > useBoostLengthFromBattle + battleTargetNearLength)_controller.Input(ControllerBase::InputName::Boost, true);

	auto moveInput = ControllerBase::InputName::None;

	if (tmpDir.x + tmpDir.z > tmpDir.y)
	{
		moveInput = ControllerBase::InputName::Front;

		if (length < battleTargetNearLength)moveInput = ControllerBase::InputName::Back;
	}
	else
	{
		if (length > battleTargetNearLength)moveInput = ControllerBase::InputName::Jump;
	}

	_controller.Input(moveInput, true);

}

void CPUMoveInput::MoveUnButtleTarget(CPUController& _controller, bool _isFarThePositionFlg, const ChVec3& _dir)
{
	if (!_isFarThePositionFlg)return;


}
