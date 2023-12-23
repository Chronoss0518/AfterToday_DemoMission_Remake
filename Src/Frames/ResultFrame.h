#pragma once

class ResultFrame : public ChCpp::BaseFrame
{
public:

	void Init(ChPtr::Shared<ChCpp::SendDataClass> _sendData)override;

	void Release()override;

	void Update()override;

private:

	struct DrawBlock
	{

		ChD3D11::Texture11 block;
		ChD3D11::Sprite11 drawPosition;
	};

	struct DrawValueBlock : public DrawBlock
	{

		ChPtr::Shared<ChD3D11::Texture11> value = nullptr;
	};

	void UpdateFunction();

	void DrawFunction();

	ChPtr::Shared<ChD3D11::Texture11> CreateTextImageFromPayment(
		unsigned long _value,
		ChD3D::DirectFontBase& _drawer,
		ChD3D::WICBitmapObject& _bitmap, 
		ChD3D::BrushObject& _brush,
		ChD3D::TextFormatObject& _format);

	DrawBlock resultTitle;

	DrawValueBlock successFee;
	DrawValueBlock additionalCompensation;
	DrawValueBlock specialPayReduction;
	DrawValueBlock repairCosts;
	DrawValueBlock ammunitionExpense;
	DrawValueBlock energyCosts;
	DrawValueBlock finalIncome;

	DrawValueBlock additionalCompensationExplanation;
	DrawValueBlock specialPayReductionExplanation;

};