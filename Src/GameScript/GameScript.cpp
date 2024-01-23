
#include"../BaseIncluder.h"

#include"GameScript.h"


void GameScript::CreateAllScript(const std::string& _text)
{
	scripts.SetText(_text);
}

void GameScript::SetScript(const std::string& _text)
{
	scripts.SetTextLine(_text, scripts.Count());
}

void GameScript::SetScript(const std::string& _type, const std::string& _text)
{
	scripts.SetTextLine(_type + " " + _text, scripts.Count());
}

void GameScript::SetFunction(const std::string& _type,const std::function<void(const std::string&)>& _function)
{
	scriptFunctions[_type] = _function;
}

void GameScript::SetPosToLoopStart(const std::string& _name)
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
	
	std::string script = "";

	do{
		script = scripts[nowScriptCount];
		nowScriptCount++;
	} while (script[0] == '/' && script[1] == '/');

	if (skipCount > 0)
	{
		skipCount--;
		return;
	}
	
	auto scriptArgs = ChStr::Split(script, " ");
	auto function = scriptFunctions.find(scriptArgs[0]);
	if (function == scriptFunctions.end())return;

	unsigned long argsPos = scriptArgs[0].size() + 1;
	std::string args = "";
	if(script.size() > argsPos)args = script.substr(argsPos);
	function->second(args);
}
