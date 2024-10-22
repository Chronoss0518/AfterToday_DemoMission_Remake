#pragma once

struct FromStageSelectFrameData : public ChCpp::SendDataClass
{
	size_t selectStage = 0;
	size_t selectDrawTop = 0;
	size_t selectMenu = 0;
};