#pragma once

class MechaParts;
class MechaPartsObject;



#include"MenuBase.h"


class EditFrame :public ChCpp::BaseFrame, public MenuBase
{
public:

	void Init(ChPtr::Shared<ChCpp::SendDataClass> _sendData)override;

	void Release()override;

	void Update()override;

private:

	void DrawFunction();

	void UpdateAction(ActionType _type)override;

	void UpdateMouse()override;

private:

	ChD3D::XInputController controller;

};
