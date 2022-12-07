
#include"../BaseIncluder.h"

#include"GameScript.h"


void GameScript::CreateAllScript(const std::string& _text)
{
	ChCpp::TextObject texts;
	texts.SetText(_text);

	for (auto&& text : texts)
	{
		SetScript(text);
	}
}

void GameScript::SetScript(const std::string& _text)
{
	scripts.SetTextLine(_text);
}

void GameScript::SetScript(const std::string& _type, const std::string& _text)
{
	scripts.SetTextLine(_type + " " + _text);
}

void GameScript::SetFunction(const std::string& _type,const std::function<void(const std::string&)>& _function)
{
	scriptFunctions[_type] = _function;
}

void GameScript::UpdateScript()
{
	if (IsStop())return;

	auto scriptArgs = ChStr::Split(scripts[nowScriptCount], " ");
	unsigned long argsPos = scripts[nowScriptCount].find(scriptArgs[0]);
	argsPos += scriptArgs[0].size() + 1;
	scriptFunctions[scriptArgs[0]](scripts[nowScriptCount].substr(argsPos));
}
