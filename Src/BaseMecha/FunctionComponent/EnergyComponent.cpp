
#include"../../BaseIncluder.h"

#include"../../AllStruct.h"

#include"EnergyComponent.h"

void EnergyComponent::Update()
{
	unsigned long fps = ChSystem::SysManager().GetFPS();

	nowEnergy += (chargeEnergy * (120 / fps));

	nowEnergy = nowEnergy > maxEnergy ? maxEnergy : nowEnergy;
}
