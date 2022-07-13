#ifndef _PlayerObj
#define _PlayerObj

class PlayerObj:public BRobot
{
public:

	PlayerObj(){};

	~PlayerObj() { Mouse = nullptr; }

	void Init(unsigned short _PartsNum)override;

	void Input()override;

private:

	ChStd::Bool InputFlg = ChStd::True;

};

#endif
