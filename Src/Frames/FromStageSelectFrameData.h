#pragma once

struct FromStageSelectFrameData : public ChCpp::SendDataClass
{
	int selectStage = 0;
	int selectDrawTop = 0;
	int selectMenu = 0;
};