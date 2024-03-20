#pragma once

#include"../MechaPartsObject.h"


class PostureFunction :public ExternalFunction
{
public:

	void Update()override;

private:

	ChLMat posture;
};