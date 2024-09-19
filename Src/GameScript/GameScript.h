#pragma once

#include<stack>

class GameScript
{
public:

	void CreateAllScript(const std::wstring& _text);

	void ResetNowScriptCount() { nowScriptCount = 0; }

	void SetScript(const std::wstring& _text);

	void SetScript(const std::wstring& _type,const std::wstring& _text);

	void SetFunction(const std::wstring& _type,const std::function<void(const std::wstring&)>& _function);

	void SetSkipCount(unsigned long _skipCount) { skipCount = _skipCount; }

	void SetNowScriptCount(unsigned long _scriptCount) { nowScriptCount = _scriptCount; }

	void SetLoopPos(const std::wstring& _name) { loopPosList[_name] = nowScriptCount; }

	void SetPosToLoopStart(const std::wstring& _name);

	static void SetDecimalPoint(const unsigned char _size) { if(_size < 37)DecimalPoint() = _size; }

	ChCpp::TextObject<wchar_t> GetScripts() { return scripts; }

	unsigned long GetScriptCount() { return nowScriptCount; }

	static long GetRand(long _min = 1, long _max = 0xffffffff);

	static float GetRand(float _min = 1, float _max = 10E+37);

	static unsigned char GetDecimalPoint() { return DecimalPoint(); }

	void UpdateScript();

	bool IsStop() { return nowScriptCount >= scripts.Count(); };

private:

	//è¨êîì_à»â∫ÇÃ
	static unsigned char& DecimalPoint()
	{
		static unsigned char ins = 1;
		return ins;
	}

	unsigned long nowScriptCount = 0;
	unsigned long skipCount = 0;
	std::map<std::wstring, unsigned long>loopPosList;

	ChCpp::TextObject<wchar_t> scripts;

	std::map<std::wstring, std::function<void(const std::wstring&)>>scriptFunctions;
};