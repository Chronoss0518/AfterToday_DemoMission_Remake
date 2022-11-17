#pragma once

#include"FunctionComponent.h"

class WeaponComponent:public FunctionComponent
{
public:

	

protected:

	InputName attack = InputName::None;
	InputName msChange = InputName::None;
	InputName mwChange = InputName::None;

};