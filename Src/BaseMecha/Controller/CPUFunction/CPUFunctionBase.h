#pragma once

#include"../ControllerBase.h"

class CPUFunctionBase
{
public:

	void SetUnStopFlg(const ChStd::Bool _flg) { unStopFlg = _flg; }

	ChStd::Bool IsUnStopFlg() { return unStopFlg; }

	virtual ChStd::Bool Update(CPUController& _controller) = 0;

private:

	//この命令を行ってもスクリプトを抜けないフラグ//
	ChStd::Bool unStopFlg = false;

};
