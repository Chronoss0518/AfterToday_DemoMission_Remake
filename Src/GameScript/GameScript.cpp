
#include"../BaseIncluder.h"

#include"GameScript.h"


void GameScript::CreateAllScript(const std::wstring& _text)
{
	scripts.SetText(_text.c_str());
}

void GameScript::SetScript(const std::wstring& _text)
{
	scripts.SetTextLine(_text.c_str(), scripts.Count());
}

void GameScript::SetScript(const std::wstring& _type, const std::wstring& _text)
{
	std::wstring tmp = _type + L" " + _text;
	scripts.SetTextLine(tmp.c_str(), scripts.Count());
}

void GameScript::SetFunction(const std::wstring& _type,const std::function<void(const std::wstring&)>& _function)
{
	scriptFunctions[_type] = _function;
}

void GameScript::SetPosToLoopStart(const std::wstring& _name)
{
	auto&& loopPos = loopPosList.find(_name);
	if (loopPos == loopPosList.end())return;
	nowScriptCount = loopPos->second;
}

long GameScript::GetRand(long _min, long _max)
{
	if (_min >= _max)return 0;
	return (rand() % (_max - _min)) + _min;
}

float GameScript::GetRand(float _min, float _max)
{
	unsigned char decimalPoint = DecimalPoint();
	long min = static_cast<long>(_min * decimalPoint);
	long max = static_cast<long>(_max * decimalPoint);

	float res = static_cast<float>(GetRand(min, max));

	res *= static_cast<float>(decimalPoint);
	return res;
}

void GameScript::UpdateScript()
{
	if (IsStop())return;
	
	std::wstring script = L"";

	do{
		script = scripts[nowScriptCount];
		nowScriptCount++;
	} while (script[0] == '/' && script[1] == '/');

	if (skipCount > 0)
	{
		skipCount--;
		return;
	}
	
	auto scriptArgs = ChStr::Split<wchar_t>(script, L" ");
	auto function = scriptFunctions.find(scriptArgs[0]);
	if (function == scriptFunctions.end())return;

	unsigned long argsPos = scriptArgs[0].size() + 1;
	std::wstring args = L"";
	if(script.size() > argsPos)args = script.substr(argsPos);
	function->second(args);
}
