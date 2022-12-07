#pragma once

#include<stack>

class GameScript
{
public:

	void CreateAllScript(const std::string& _text);

	void ResetNowScriptCount() { nowScriptCount = 0; }

	void SetScript(const std::string& _text);

	void SetScript(const std::string& _type,const std::string& _text);

	void SetFunction(const std::string& _type,const std::function<void(const std::string&)>& _function);

	void SetNowScriptCount(unsigned long _scriptCount) { nowScriptCount = _scriptCount; }

	ChCpp::TextObject GetScripts() { return scripts; }

	unsigned long GetScriptCount() { return nowScriptCount; }

	void UpdateScript();

	ChStd::Bool IsStop() { return nowScriptCount >= scripts.Count(); };

private:

	unsigned long nowScriptCount = 0;
	
	ChCpp::TextObject scripts;

	std::map<std::string, std::function<void(const std::string&)>>scriptFunctions;
};