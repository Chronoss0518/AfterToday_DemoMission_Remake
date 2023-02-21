#pragma once

#include"CPUFunctionBase.h"

class CPUAttackFunction :public CPUFunctionBase
{
public:

	ChStd::Bool Update(CPUController& _controller)override;

private:

};

class CPUAvoidFunction :public CPUFunctionBase
{
public:

	ChStd::Bool Update(CPUController& _controller)override;

private:

};

class CPUMoveFunction :public CPUFunctionBase
{
public:

	ChStd::Bool Update(CPUController& _controller)override;

private:

};

class CPUBoostFunction :public CPUFunctionBase
{
public:

	ChStd::Bool Update(CPUController& _controller)override;

private:

};

