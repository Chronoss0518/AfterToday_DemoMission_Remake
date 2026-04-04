
#include"../../BaseIncluder.h"

#include"../../AllStruct.h"

#include"EnergyComponent.h"

#include"../../Application/Application.h"

void EnergyComponent::Update()
{
	unsigned long fps = AppIns().GetFPS();

	nowEnergy += (chargeEnergy * (120 / fps));

	nowEnergy = nowEnergy > maxEnergy ? maxEnergy : nowEnergy;
}
