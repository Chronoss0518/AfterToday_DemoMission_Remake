#pragma once

#include"MenuBase.h"

struct ResultStructure;

class ResultFrame : public ChCpp::BaseFrame,public MenuBase
{
public:

	void Init(ChPtr::Shared<ChCpp::SendDataClass> _sendData)override;

	void Release()override;

	void Update()override;

private:

	static constexpr unsigned long EXPLANATION_TYPE_COUNT = 2;
	enum class ExplanationType
	{
		AdditionalCompensation,
		SpecialPayReduction
	};

	static constexpr unsigned long BLOCK_TYPE_COUNT = 7;
	enum class BlockType
	{
		SuccessFee,
		AdditionalCompensation,
		SpecialPayReduction,
		RepairCosts,
		AmmunitionExpense,
		EnergyCosts,
		FinalIncome,
	};


	static constexpr unsigned long SELECT_BUTTON_TYPE_COUNT = 2;
	enum class SelectButtonType
	{
		Left,
		Right,
		None
	};

private:

	struct DrawValueBlock
	{
		ImageSprite back;
		ImageSprite value;
	};

	struct DrawExplanationImage
	{
		ChD3D11::Texture11 backGround;
		ChD3D11::Texture11 text;
	};

	struct ButtonImage : public ImageSprite
	{
		ChD3D11::Texture11 select;
	};

protected:

	void UpdateAction(ActionType _type)override;

	void UpdateMouse()override;

private:

	void DrawFunction();

	void CreateBlockData(ID3D11Device* _device, const ResultStructure& _result);

	void CreateExplanationData(ID3D11Device* _device, const ResultStructure& _result);

	void CreateTextImageFromBlock(
		ID3D11Device* _device,
		const std::wstring& _value,
		ChD3D11::Texture11& _outImage,
		ChD3D::DirectFontBase& _drawer,
		ChD3D::WICBitmapObject& _bitmap, 
		ChD3D::BrushObject& _brush,
		ChD3D::TextFormatObject& _format);

	void CreateTextImageFromExplanation(
		ID3D11Device* _device,
		const std::wstring& _value,
		ChD3D11::Texture11& _outImage,
		ChD3D::DirectFontBase& _drawer,
		ChD3D::WICBitmapObject& _bitmap,
		ChD3D::BrushObject& _brush,
		ChD3D::TextFormatObject& _format);

private:

	ChPtr::Shared<ResultStructure> resultData = nullptr;
	long finalIncome = 0;

	ChD3D::XInputController controller;

	ChD3D11::Shader::BaseDrawSprite11 spriteShader;

	ImageSprite resultTitle;

	DrawValueBlock block[BLOCK_TYPE_COUNT];

	int exprlationType = 0;
	ChD3D11::Sprite11 explanationBackGroundSprite;
	ChD3D11::Sprite11 explanationTextSprite;
	DrawExplanationImage explanation[EXPLANATION_TYPE_COUNT];

	SelectButtonType buttonSelectType = SelectButtonType::None;
	ButtonImage button[SELECT_BUTTON_TYPE_COUNT];
	ChWin::MouseController* mouse = &ChWin::Mouse();
};