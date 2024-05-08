
#include"../BaseIncluder.h"

#include"ResultFrame.h"
#include"../BaseMecha/MechaParts.h"
#include"../BaseMecha/MechaPartsObject.h"
#include"StageDataStructure.h"

#define RESULT_TEXTURE_DIRECTORY(current_path) TEXTURE_DIRECTORY("Result/") current_path

#define RESULT_TITLE_TOP 42.0f
#define RESULT_TITLE_LEFT 422.0f
#define RESULT_TITLE_BOTTOM 60.0f + RESULT_TITLE_TOP
#define RESULT_TITLE_RIGHT 436.0f + RESULT_TITLE_LEFT

#define BLOCK_TOP 140.0f
#define BLOCK_LEFT 60.0f
#define BLOCK_WIDTH 520.0f
#define BLOCK_HEIGHT 50.0f
#define BLOCK_LINE_BETWEEN 20.0f

#define BLOCK_TEXT_LEFT 185.0f
#define BLOCK_TEXT_PADDING 5.0f
#define BLOCK_TEXT_SIZE 30.0f


#define EXPLANATION_TOP 140.0f
#define EXPLANATION_LEFT 699.0f
#define EXPLANATION_WIDTH 520.0f
#define EXPLANATION_HEIGHT 470.0f

#define EXPLANATION_TEXT_TOP 99.0f
#define EXPLANATION_TEXT_PADDING 5.0f
#define EXPLANATION_TEXT_SIZE 20.0f


#define BUTTON_TOP 162.0f
#define BUTTON_BOTTOM BUTTON_TOP + 77.0f
#define BUTTON_WIDTH 70.0f

#define BUTTON_LEFT_LEFT 750.0f

#define BUTTON_RIGHT_LEFT 1106.0f

void ResultFrame::Init(ChPtr::Shared<ChCpp::SendDataClass> _sendData)
{
	float color = (0xaa * 1.0f) / (0xff * 1.0f);

	ChD3D11::Shader11().SetBackColor(ChVec4::FromColor(color, color, color, 1.0f));

	resultData = ChPtr::SharedSafeCast<ResultStructure>(_sendData);

	auto&& device = ChD3D11::D3D11Device();

	resultTitle.image.CreateTexture(ChStr::UTF8ToWString(RESULT_TEXTURE_DIRECTORY("Result_Title_JP.png")));
	SPRITE_INIT(resultTitle.sprite, RectToGameWindow(ChVec4::FromRect(RESULT_TITLE_LEFT,RESULT_TITLE_TOP, RESULT_TITLE_RIGHT, RESULT_TITLE_BOTTOM)));
	
	SPRITE_INIT(button[ChStd::EnumCast(SelectButtonType::Left)].sprite, RectToGameWindow(ChVec4::FromRect(BUTTON_LEFT_LEFT, BUTTON_TOP, BUTTON_LEFT_LEFT + BUTTON_WIDTH, BUTTON_BOTTOM)));
	button[ChStd::EnumCast(SelectButtonType::Left)].image.CreateTexture(ChStr::UTF8ToWString(RESULT_TEXTURE_DIRECTORY("ChangeLeftButton.png")));
	button[ChStd::EnumCast(SelectButtonType::Left)].select.CreateTexture(ChStr::UTF8ToWString(RESULT_TEXTURE_DIRECTORY("ChangeLeftButton_Select.png")));

	SPRITE_INIT(button[ChStd::EnumCast(SelectButtonType::Right)].sprite, RectToGameWindow(ChVec4::FromRect(BUTTON_RIGHT_LEFT, BUTTON_TOP, BUTTON_RIGHT_LEFT + BUTTON_WIDTH, BUTTON_BOTTOM)));
	button[ChStd::EnumCast(SelectButtonType::Right)].image.CreateTexture(ChStr::UTF8ToWString(RESULT_TEXTURE_DIRECTORY("ChangeRightButton.png")));
	button[ChStd::EnumCast(SelectButtonType::Right)].select.CreateTexture(ChStr::UTF8ToWString(RESULT_TEXTURE_DIRECTORY("ChangeRightButton_Select.png")));

	float top = BLOCK_TOP;
	float bottom = top + BLOCK_HEIGHT;

	for (unsigned long i = 0; i < BLOCK_TYPE_COUNT; i++)
	{
		SPRITE_INIT(block[i].back.sprite, RectToGameWindow(ChVec4::FromRect(
			BLOCK_LEFT,
			top, 
			BLOCK_LEFT + BLOCK_WIDTH,
			bottom)));

		SPRITE_INIT(block[i].value.sprite, RectToGameWindow(ChVec4::FromRect(
			BLOCK_LEFT + BLOCK_TEXT_LEFT + BLOCK_TEXT_PADDING,
			top + BLOCK_TEXT_PADDING,
			BLOCK_LEFT + BLOCK_WIDTH - BLOCK_TEXT_PADDING,
			bottom - BLOCK_TEXT_PADDING)));

		top = bottom + BLOCK_LINE_BETWEEN;
		bottom = top + BLOCK_HEIGHT;
	}

	SPRITE_INIT(explanationBackGroundSprite, RectToGameWindow(ChVec4::FromRect(
		EXPLANATION_LEFT,
		EXPLANATION_TOP,
		EXPLANATION_LEFT + EXPLANATION_WIDTH,
		EXPLANATION_TOP + EXPLANATION_HEIGHT)));

	SPRITE_INIT(explanationTextSprite, RectToGameWindow(ChVec4::FromRect(
		EXPLANATION_LEFT + EXPLANATION_TEXT_PADDING,
		EXPLANATION_TOP + EXPLANATION_TEXT_TOP + EXPLANATION_TEXT_PADDING,
		EXPLANATION_LEFT + EXPLANATION_WIDTH - EXPLANATION_TEXT_PADDING,
		EXPLANATION_TOP + EXPLANATION_HEIGHT - EXPLANATION_TEXT_PADDING)));

	CreateBlockData(device, *resultData);

	CreateExplanationData(device, *resultData);

	spriteShader.Init(device);

	controller.Init();

	MenuBase::InitMenu(&controller);
}

void ResultFrame::Release()
{
	controller.Release();
}

void ResultFrame::Update()
{

	UpdateFunction();

	DrawFunction();
}

void ResultFrame::UpdateAction(ActionType _type)
{
	if (_type == ActionType::Left || _type == ActionType::Right)
	{
		exprlationType = (exprlationType + 1)% EXPLANATION_TYPE_COUNT;
	}

	if (_type == ActionType::Decision)
	{
		if (buttonSelectType == SelectButtonType::Left)
		{
			AddActionType(ActionType::Left);
			return;
		}
		if (buttonSelectType == SelectButtonType::Right)
		{
			AddActionType(ActionType::Right);
			return;
		}
		ChangeFrame(ChStd::EnumCast(FrameNo::Select));
		SetLoopBreakTrue();
	}


}

void ResultFrame::DrawFunction()
{
	auto&& dc = ChD3D11::D3D11DC();

	ChD3D11::Shader11().DrawStart();

	spriteShader.DrawStart(dc);

	spriteShader.Draw(resultTitle.image, resultTitle.sprite);

	for (unsigned long i = 0; i < BLOCK_TYPE_COUNT; i++)
	{
		spriteShader.Draw(block[i].value.image, block[i].value.sprite);
		spriteShader.Draw(block[i].back.image, block[i].back.sprite);
	}

	for (unsigned long i = 0; i < SELECT_BUTTON_TYPE_COUNT; i++)
	{
		if (buttonSelectType == static_cast<SelectButtonType>(i))
		{
			spriteShader.Draw(button[i].select, button[i].sprite);
			continue;
		}

		spriteShader.Draw(button[i].image, button[i].sprite);
	}

	spriteShader.Draw(explanation[exprlationType].text, explanationTextSprite);
	spriteShader.Draw(explanation[exprlationType].backGround, explanationBackGroundSprite);

	spriteShader.DrawEnd();

	ChD3D11::Shader11().DrawEnd();
}

void ResultFrame::CreateTextImageFromBlock(
	ID3D11Device* _device,
	const std::wstring& _value,
	ChD3D11::Texture11& _outImage,
	ChD3D::DirectFontBase& _drawer,
	ChD3D::WICBitmapObject& _bitmap,
	ChD3D::BrushObject& _brush,
	ChD3D::TextFormatObject& _format)
{
	_drawer.DrawStart();

	_drawer.DrawToScreen(_value, _format, _brush, ChVec4::FromRect(0.0f, 0.0f, BLOCK_WIDTH - BLOCK_TEXT_LEFT - (BLOCK_TEXT_PADDING * 2), BLOCK_HEIGHT - (BLOCK_TEXT_PADDING * 2)));

	_drawer.DrawEnd();

	_outImage.CreateColorTexture(_device, _bitmap.GetBitmap());
}

void ResultFrame::CreateTextImageFromExplanation(
	ID3D11Device* _device,
	const std::wstring& _value,
	ChD3D11::Texture11& _outImage,
	ChD3D::DirectFontBase& _drawer,
	ChD3D::WICBitmapObject& _bitmap,
	ChD3D::BrushObject& _brush,
	ChD3D::TextFormatObject& _format)
{
	_drawer.DrawStart();

	_drawer.DrawToScreen(_value, _format, _brush, ChVec4::FromRect(0.0f, 0.0f, EXPLANATION_WIDTH - (EXPLANATION_TEXT_PADDING * 2), EXPLANATION_HEIGHT - EXPLANATION_TEXT_TOP - (EXPLANATION_TEXT_PADDING * 2)));

	_drawer.DrawEnd();

	_outImage.CreateColorTexture(_device, _bitmap.GetBitmap());
}

void ResultFrame::CreateBlockData(ID3D11Device* _device, const ResultStructure& _result)
{
	ChD3D::DirectFontFromWICBitmap drawer;

	unsigned long w = static_cast<unsigned long>(BLOCK_WIDTH - BLOCK_TEXT_LEFT - (BLOCK_TEXT_PADDING * 2));
	unsigned long h = static_cast<unsigned long>(BLOCK_HEIGHT - (BLOCK_TEXT_PADDING * 2));

	ChD3D::WICBitmapObject bitmap = ChD3D::WICBitmapCreatorObj().CreateBitmapObject(w, h);
	drawer.Init(w, h, bitmap, ChD3D::DirectFontBase::LocaleNameId::Japanese);

	drawer.SetClearDisplayColor(ChVec4::FromColor(0.0f, 0.0f, 0.0f, 0.0f));
	drawer.SetClearDisplayFlg(true);

	ChD3D::BrushObject brush = drawer.CreateBrush(ChVec4::FromColor(0.0f, 0.0f, 0.0f, 1.0f));
	ChD3D::TextFormatObject format = drawer.CreateTextFormat(L"メイリオ", nullptr, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, BLOCK_TEXT_SIZE);
	format.SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);


	block[ChStd::EnumCast(BlockType::SuccessFee)].back.image.CreateTexture(ChStr::UTF8ToWString(RESULT_TEXTURE_DIRECTORY("Success_Fee_JP.png")));
	CreateTextImageFromBlock(
		_device,
		CreateMoneyText(std::to_wstring(_result.successFee)),
		block[ChStd::EnumCast(BlockType::SuccessFee)].value.image,
		drawer,
		bitmap,
		brush,
		format
	);

	finalIncome += _result.successFee;

	block[ChStd::EnumCast(BlockType::AdditionalCompensation)].back.image.CreateTexture(ChStr::UTF8ToWString(RESULT_TEXTURE_DIRECTORY("Additional_Compensation_JP.png")));
	CreateTextImageFromBlock(
		_device,
		CreateMoneyText(std::to_wstring(_result.additionalCompensation)),
		block[ChStd::EnumCast(BlockType::AdditionalCompensation)].value.image,
		drawer,
		bitmap,
		brush,
		format
	);

	finalIncome += _result.additionalCompensation;

	block[ChStd::EnumCast(BlockType::SpecialPayReduction)].back.image.CreateTexture(ChStr::UTF8ToWString(RESULT_TEXTURE_DIRECTORY("Special_Pay_Reduction_JP.png")));
	CreateTextImageFromBlock(
		_device,
		CreateMoneyText(std::to_wstring(_result.specialPayReduction)),
		block[ChStd::EnumCast(BlockType::SpecialPayReduction)].value.image,
		drawer,
		bitmap,
		brush,
		format
	);

	finalIncome -= _result.specialPayReduction;

	block[ChStd::EnumCast(BlockType::RepairCosts)].back.image.CreateTexture(ChStr::UTF8ToWString(RESULT_TEXTURE_DIRECTORY("Repair_Costs_JP.png")));
	CreateTextImageFromBlock(
		_device,
		CreateMoneyText(std::to_wstring(_result.repairCosts)),
		block[ChStd::EnumCast(BlockType::RepairCosts)].value.image,
		drawer,
		bitmap,
		brush,
		format
	);

	finalIncome -= _result.repairCosts;

	block[ChStd::EnumCast(BlockType::AmmunitionExpense)].back.image.CreateTexture(ChStr::UTF8ToWString(RESULT_TEXTURE_DIRECTORY("Ammunition_Expense_JP.png")));
	CreateTextImageFromBlock(
		_device,
		CreateMoneyText(std::to_wstring(_result.ammunitionExpense)),
		block[ChStd::EnumCast(BlockType::AmmunitionExpense)].value.image,
		drawer,
		bitmap,
		brush,
		format
	);

	finalIncome -= _result.ammunitionExpense;

	block[ChStd::EnumCast(BlockType::EnergyCosts)].back.image.CreateTexture(ChStr::UTF8ToWString(RESULT_TEXTURE_DIRECTORY("Energy_Costs_JP.png")));
	CreateTextImageFromBlock(
		_device,
		CreateMoneyText(std::to_wstring(_result.energyCosts)),
		block[ChStd::EnumCast(BlockType::EnergyCosts)].value.image,
		drawer,
		bitmap,
		brush,
		format
	);

	finalIncome -= _result.energyCosts;

	block[ChStd::EnumCast(BlockType::FinalIncome)].back.image.CreateTexture(ChStr::UTF8ToWString(RESULT_TEXTURE_DIRECTORY("Final_Income_JP.png")));
	CreateTextImageFromBlock(
		_device,
		CreateMoneyText(std::to_wstring(finalIncome)),
		block[ChStd::EnumCast(BlockType::FinalIncome)].value.image,
		drawer,
		bitmap,
		brush,
		format
	);

}

void ResultFrame::CreateExplanationData(ID3D11Device* _device, const ResultStructure& _result)
{
	ChD3D::DirectFontFromWICBitmap drawer;

	unsigned long w = static_cast<unsigned long>(EXPLANATION_WIDTH - (EXPLANATION_TEXT_PADDING * 2));
	unsigned long h = static_cast<unsigned long>(EXPLANATION_HEIGHT - EXPLANATION_TEXT_TOP - (EXPLANATION_TEXT_PADDING * 2));

	ChD3D::WICBitmapObject bitmap = ChD3D::WICBitmapCreatorObj().CreateBitmapObject(w, h);
	drawer.Init(w, h, bitmap, ChD3D::DirectFontBase::LocaleNameId::Japanese);

	drawer.SetClearDisplayColor(ChVec4::FromColor(0.0f, 0.0f, 0.0f, 0.0f));
	drawer.SetClearDisplayFlg(true);

	ChD3D::BrushObject brush = drawer.CreateBrush(ChVec4::FromColor(0.0f, 0.0f, 0.0f, 1.0f));
	ChD3D::TextFormatObject format = drawer.CreateTextFormat(L"メイリオ", nullptr, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, EXPLANATION_TEXT_SIZE);
	format.SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	format.SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	explanation[ChStd::EnumCast(ExplanationType::AdditionalCompensation)].backGround.CreateTexture(ChStr::UTF8ToWString(RESULT_TEXTURE_DIRECTORY("Additional_Compensation_Explanation_JP.png")));
	CreateTextImageFromExplanation(
		_device,
		ChStr::UTF8ToWString(_result.additionalCompensationExplanation),
		explanation[ChStd::EnumCast(ExplanationType::AdditionalCompensation)].text,
		drawer,
		bitmap,
		brush,
		format
	);

	explanation[ChStd::EnumCast(ExplanationType::SpecialPayReduction)].backGround.CreateTexture(ChStr::UTF8ToWString(RESULT_TEXTURE_DIRECTORY("Special_Pay_Reduction_Explanation_JP.png")));
	CreateTextImageFromExplanation(
		_device,
		ChStr::UTF8ToWString(_result.specialPayReductionExplanation),
		explanation[ChStd::EnumCast(ExplanationType::SpecialPayReduction)].text,
		drawer,
		bitmap,
		brush,
		format
	);

}

void ResultFrame::UpdateMouse()
{
	auto&& manager = ChSystem::SysManager();

	InputTest(ActionType::Decision, manager.IsPushKeyNoHold(VK_LBUTTON));

	mouse->Update();

	auto&& mouseMove = mouse->GetMoveValue();
	if (std::abs(mouseMove.x) <= 1 && std::abs(mouseMove.y) <= 1)return;

	for (int i = 0; i < SELECT_BUTTON_TYPE_COUNT; i++)
	{
		if (!IsMoucePosOnSprite(button[i].sprite))continue;
		buttonSelectType = static_cast<SelectButtonType>(i);
		return;
	}

	buttonSelectType = SelectButtonType::None;
}
