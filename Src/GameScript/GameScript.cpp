
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
	unsigned long spacePos = _text.find(" ");
	auto script = ChPtr::Make_S<Script>();
	script->type = _text.substr(0, spacePos);
	script->text = _text.substr(spacePos);
	scripts.push_back(script);
}

void GameScript::SetScript(const std::string& _type, const std::string& _text)
{
	auto script = ChPtr::Make_S<Script>();
	script->type =_type;
	script->text = _text;
	scripts.push_back(script);
}

void GameScript::SetFunction(const std::string& _type,const std::function<void(const std::string&)>& _function)
{
	scriptFunctions[_type] = _function;
}

void GameScript::UpdateScript()
{
	if (nowScriptCount >= scripts.size())return;

	scriptFunctions[scripts[nowScriptCount]->type](scripts[nowScriptCount]->text);
}
