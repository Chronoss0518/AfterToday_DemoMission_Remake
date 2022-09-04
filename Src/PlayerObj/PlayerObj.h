#ifndef _PlayerObj
#define _PlayerObj

class PlayerObj:public BRobot
{
public:

	PlayerObj(){};

	~PlayerObj() { mouse = nullptr; }

	void Init(unsigned short _PartsNum)override;

	void Input()override;

private:

	ChStd::Bool InputFlg = true;

};

#endif
