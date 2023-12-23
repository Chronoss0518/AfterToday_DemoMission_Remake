
#include"../BaseIncluder.h"

#include"ResultFrame.h"
#include"../BaseMecha/MechaParts.h"
#include"../BaseMecha/MechaPartsObject.h"
#include"StageDataStructure.h"

#define TEXT_DRAW_HEIGHT 50
#define TEXT_DRAW_WIDTH 335

#define TEXT_DRAW_TOP 1
#define TEXT_DRAW_LEFT 1
#define TEXT_DRAW_BOTTOM 50 - (TEXT_DRAW_TOP * 2)
#define TEXT_DRAW_RIGHT 335 - (TEXT_DRAW_LEFT * 2)

void ResultFrame::Init(ChPtr::Shared<ChCpp::SendDataClass> _sendData)
{
	auto&& resultData = ChPtr::SharedSafeCast<ResultStructure>(_sendData);

	auto&& device = ChD3D11::D3D11Device();

	ChD3D::DirectFontFromWICBitmap drawer;

	ChD3D::WICBitmapObject bitmap = ChD3D::WICBitmapCreatorObj().CreateBitmapObject(TEXT_DRAW_WIDTH, TEXT_DRAW_HEIGHT);
	drawer.Init(TEXT_DRAW_WIDTH, TEXT_DRAW_HEIGHT, bitmap, ChD3D::DirectFontBase::LocaleNameId::Japanese);

	drawer.SetClearDisplayColor(ChVec4::FromColor(0.0f, 0.0f, 0.0f, 0.0f));
	drawer.SetClearDisplayFlg(true);

	ChD3D::BrushObject brush = drawer.CreateBrush(ChVec4(1.0f));
	ChD3D::TextFormatObject format = drawer.CreateTextFormat(L"ÉÅÉCÉäÉI", nullptr, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 38.0f);
	format.SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);

}

void ResultFrame::Release()
{

}

void ResultFrame::Update()
{
	UpdateFunction();

	DrawFunction();
}

void ResultFrame::UpdateFunction()
{

}

void ResultFrame::DrawFunction()
{

}

ChPtr::Shared<ChD3D11::Texture11> ResultFrame::CreateTextImageFromPayment(
	unsigned long _value,
	ChD3D::DirectFontBase& _drawer,
	ChD3D::WICBitmapObject& _bitmap,
	ChD3D::BrushObject& _brush,
	ChD3D::TextFormatObject& _format)
{
	auto&& text = std::to_wstring(_value);

	text += L"\\";

	_drawer.DrawStart();

	_drawer.DrawToScreen(text, _format, _brush, ChVec4::FromRect(TEXT_DRAW_LEFT, TEXT_DRAW_TOP, TEXT_DRAW_RIGHT, TEXT_DRAW_BOTTOM));

	_drawer.DrawEnd();

	auto&& result = ChPtr::Make_S<ChD3D11::Texture11>();

	result->CreateColorTexture(_bitmap.GetBitmap());

	return result;
}
