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

	void UpdateScript();

	ChStd::Bool IsStop() { return nowScriptCount >= scripts.size(); };

private:

	struct Script
	{
		std::string type = "";
		std::string text = "";
	};

	unsigned long nowScriptCount = 0;

	std::stack<unsigned long>loopStartPos;
	
	std::vector<ChPtr::Shared<Script>>scripts;

	std::map<std::string, std::function<void(const std::string&)>>scriptFunctions;


};