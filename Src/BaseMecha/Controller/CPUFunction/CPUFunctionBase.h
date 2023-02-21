#pragma once

#include"../ControllerBase.h"

class CPUFunctionBase
{
public:

	void SetUnStopFlg(const ChStd::Bool _flg) { unStopFlg = _flg; }

	ChStd::Bool IsUnStopFlg() { return unStopFlg; }

	virtual ChStd::Bool Update(CPUController& _controller) = 0;

private:

	//���̖��߂��s���Ă��X�N���v�g�𔲂��Ȃ��t���O//
	ChStd::Bool unStopFlg = false;

};
