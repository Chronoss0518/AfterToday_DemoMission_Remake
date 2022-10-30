#pragma once

#include"PhysicsMachine.h"

class Physics_Gravity :public PhysicsFunction
{
public:

	virtual void Update()override;

	void UpdateEnd()override;

public:

	inline void SetGravityAcceleration(float _acceleration)
	{
		acceleration = _acceleration;
	}

private:

	float acceleration = 0.0f;
	float nowPower = 0.0f;
};
