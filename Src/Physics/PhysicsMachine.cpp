
#include"../BaseIncluder.h"
#include"../AllStruct.h"

#include"PhysicsMachine.h"
#include"Physics_Gravity.h"
#include"Physics_AirPower.h"
#include"Physics_WaterPower.h"
#include"Physics_AirPower.h"

void PhysicsFunction::Init()
{

}

void PhysicsFunction::AddEffectMove(const ChVec3& _addMove)
{
	machine->effectMove += _addMove; 
}

inline std::vector<ChPtr::Weak<ChCpp::FrameObject>>PhysicsFunction::GetFieldList() 
{
	return PhysicsMachine::FieldList();
}

void PhysicsMachine::Init()
{
	functionList[ChStd::EnumCast(PhysicsFunctionList::Gravity)] = ChPtr::Make_S<PhysicsFunction_Gravity>();
}
