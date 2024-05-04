
#include"../BaseIncluder.h"

#include"ParameterList.h"
#include"../BaseMecha/MechaParts.h"
#include"../BaseMecha/MechaPartsObject.h"
#include"PartsParameters.h"
#include"ParameterPanel.h"
#include"ParameterDisplay.h"

#define EDIT_TEXTURE_DIRECTORY(current_path) TEXTURE_DIRECTORY("Edit/") current_path

#define FLOATING_TEST_VALUE 0.0001f
#define TITLE_FONT_SIZE 24.0f
#define VALUE_TITLE_FONT_SIZE 10.0f

#define PANEL_TEXT_SIDE_ALIGH 6.0f

#define PARAMETER_PANEL_TITLE_WIDTH 175.0f

#define PARAMETER_PANEL_VALUE_WIDTH (PARAMETER_PANEL_WIDTH - PARAMETER_PANEL_TITLE_WIDTH)


ChPtr::Shared<ParameterTitlePanel> ParameterTitlePanel::CreatePanel(ID3D11Device* _device, TextDrawerWICBitmap& _textDrawer, const std::string& _title)
{
	auto&& res = ChPtr::Make_S<ParameterTitlePanel>();
	res->CreateTitle(_textDrawer, _device, _title);
	return res;
}

void ParameterTitlePanel::CreateTitle(TextDrawerWICBitmap& _textDrawer, ID3D11Device* _device, const std::string& _title)
{
	if (_title.length() <= 0)return;
	title = nullptr;
	title = ChPtr::Make_S<ChD3D11::Texture11>();

	CreateTitleImage(_textDrawer, _device, _title);
}

void ParameterTitlePanel::CreateTitleImage(TextDrawerWICBitmap& _textDrawer, ID3D11Device* _device, const std::string& _title)
{
	CreateTexture(*title, _textDrawer, _device, _title, ChVec2::FromSize(PARAMETER_PANEL_WIDTH - (PANEL_TEXT_SIDE_ALIGH * 2.0f), PARAMETER_PANEL_HEIGHT));
}

ChPtr::Shared<ParameterValuePanel> ParameterValuePanel::CreatePanel(ID3D11Device* _device, TextDrawerWICBitmap& _titleDrawer, const std::string& _title, TextDrawerWICBitmap& _valueDrawer, unsigned long* _baseValue, unsigned long* _nextValue, bool _inversOperatorFlg)
{
	auto&& res = ChPtr::Make_S<ParameterULValuePanel>();
	res->SetInversOperatorFlg(_inversOperatorFlg);
	res->CreateValue(_valueDrawer, _device, _baseValue, _nextValue);
	res->CreateTitle(_titleDrawer, _device, _title);
	return res;
}

ChPtr::Shared<ParameterValuePanel> ParameterValuePanel::CreatePanel(ID3D11Device* _device, TextDrawerWICBitmap& _titleDrawer, const std::string& _title, TextDrawerWICBitmap& _valueDrawer, float* _baseValue, float* _nextValue, bool _inversOperatorFlg)
{
	auto&& res = ChPtr::Make_S<ParameterFValuePanel>();
	res->SetInversOperatorFlg(_inversOperatorFlg);
	res->CreateValue(_valueDrawer, _device, _baseValue, _nextValue);
	res->CreateTitle(_titleDrawer, _device, _title);
	return res;
}

void ParameterValuePanel::CreateTitleImage(TextDrawerWICBitmap& _textDrawer, ID3D11Device* _device, const std::string& _title)
{
	CreateTexture(*title, _textDrawer, _device, _title, ChVec2::FromSize(PARAMETER_PANEL_TITLE_WIDTH - (PANEL_TEXT_SIDE_ALIGH * 2.0f), PARAMETER_PANEL_HEIGHT));
}

void ParameterTitlePanel::Draw(ChD3D11::Shader::BaseDrawSprite11& _drawer, const ChVec2& _leftTop)
{
	if (title == nullptr)return;

	DrawBackGround(_drawer, _leftTop);
	
	ChVec4 tmp;
	tmp.left = _leftTop.x;
	tmp.top = _leftTop.y;
	tmp.right = tmp.left + PARAMETER_PANEL_WIDTH;
	tmp.bottom = tmp.top + PARAMETER_PANEL_HEIGHT;

	tmp.left += PANEL_TEXT_SIDE_ALIGH;
	tmp.right -= PANEL_TEXT_SIDE_ALIGH;

	sprite.SetPosRect(RectToGameWindow(tmp));

	_drawer.Draw(*title, sprite);
}

void ParameterTitlePanel::DrawBackGround(ChD3D11::Shader::BaseDrawSprite11& _drawer, const ChVec2& _leftTop)
{
	ChVec4 tmp;
	tmp.left = _leftTop.x;
	tmp.top = _leftTop.y;
	tmp.right = _leftTop.x + PARAMETER_PANEL_WIDTH;
	tmp.bottom = _leftTop.y + PARAMETER_PANEL_HEIGHT;

	sprite.SetPosRect(RectToGameWindow(tmp));

	_drawer.Draw(*background, sprite);
}

void ParameterTitlePanel::CreateTexture(ChD3D11::Texture11& _outTexture, TextDrawerWICBitmap& _textDrawer, ID3D11Device* _device, const std::string& _text, const ChVec2& _size)
{
	_textDrawer.brush.SetColor(ChVec4(0.0f, 0.0f, 0.0f, 1.0f));

	_textDrawer.drawer.DrawStart();
	_textDrawer.drawer.DrawToScreen(ChStr::UTF8ToWString(_text), _textDrawer.format, _textDrawer.brush, ChVec4::FromRect(0.0f, 0.0f, _size.w, _size.h));
	_textDrawer.drawer.DrawEnd();
	_outTexture.CreateColorTexture(_device, _textDrawer.bitmap.GetBitmap());
}

void ParameterValuePanel::CreateTexture(ChD3D11::Texture11& _outTexture, TextDrawerWICBitmap& _textDrawer, ID3D11Device* _device, const std::string& _text, const ChVec2& _size)
{
	_textDrawer.brush.SetColor(ChVec4(0.0f, 0.0f, 0.0f, 1.0f));

	_textDrawer.drawer.DrawStart();
	_textDrawer.drawer.DrawToScreen(ChStr::UTF8ToWString(_text), _textDrawer.format, _textDrawer.brush, ChVec4::FromRect(0.0f, 0.0f, _size.w, _size.h));
	_textDrawer.drawer.DrawEnd();
	_outTexture.CreateColorTexture(_device, _textDrawer.bitmap.GetBitmap());
}

void ParameterValuePanel::Draw(ChD3D11::Shader::BaseDrawSprite11& _drawer, const ChVec2& _leftTop)
{
	if (title == nullptr)return;

	DrawBackGround(_drawer, _leftTop);

	ChVec4 tmp;
	tmp.top = _leftTop.y;
	tmp.bottom = _leftTop.y + PARAMETER_PANEL_HEIGHT;

	if (title != nullptr)
	{
		tmp.left = _leftTop.x;
		tmp.right = tmp.left + PARAMETER_PANEL_TITLE_WIDTH;

		tmp.left += PANEL_TEXT_SIDE_ALIGH;
		tmp.right -= (PANEL_TEXT_SIDE_ALIGH);

		sprite.SetPosRect(RectToGameWindow(tmp));

		_drawer.Draw(*title, sprite);
	}

	if (value != nullptr)
	{
		tmp.left = _leftTop.x + PARAMETER_PANEL_TITLE_WIDTH;
		tmp.right = tmp.left + PARAMETER_PANEL_VALUE_WIDTH;

		tmp.left += PANEL_TEXT_SIDE_ALIGH;
		tmp.right -= (PANEL_TEXT_SIDE_ALIGH);

		sprite.SetPosRect(RectToGameWindow(tmp));

		_drawer.Draw(*value, sprite);
	}


}

void ParameterULValuePanel::CreateValue(TextDrawerWICBitmap& _textDrawer, ID3D11Device* _device, unsigned long* _baseValue, unsigned long* _nextValue)
{
	if (ChPtr::NullCheck(_baseValue))return;
	if (ChPtr::NullCheck(_nextValue))return;
	if (ChPtr::NullCheck(_device))return;
	value = nullptr;
	value = ChPtr::Make_S<ChD3D11::Texture11>();
	CreateTexture(*value, _textDrawer, _device, std::to_string(*_nextValue), ChVec2(PARAMETER_PANEL_VALUE_WIDTH - (PANEL_TEXT_SIDE_ALIGH * 2.0f), PARAMETER_PANEL_HEIGHT));
	baseValue = _baseValue;
	nextValue = _nextValue;
	beforeValue = *nextValue;
}

ParameterOpelator ParameterULValuePanel::GetParameterOperator()
{
	ParameterOpelator res = ParameterOpelator::EQUAL;

	if (*baseValue > *nextValue)res = inversOperatorFlg ? ParameterOpelator::BIGGER : ParameterOpelator::SMALLER;
	if (*baseValue < *nextValue)res = inversOperatorFlg ? ParameterOpelator::SMALLER : ParameterOpelator::BIGGER;

	return res;
}

void ParameterULValuePanel::UpdateValue(TextDrawerWICBitmap& _textDrawer, ID3D11Device* _device)
{
	if (ChPtr::NullCheck(nextValue))return;
	if (ChPtr::NullCheck(baseValue))return;
	if (ChPtr::NullCheck(_device))return;
	if (*nextValue == beforeValue)return;
	value->Release();
	CreateTexture(*value, _textDrawer, _device, std::to_string(*nextValue), ChVec2(PARAMETER_PANEL_VALUE_WIDTH - (PANEL_TEXT_SIDE_ALIGH * 2.0f), PARAMETER_PANEL_HEIGHT));
	beforeValue = *nextValue;
}

void ParameterFValuePanel::CreateValue(TextDrawerWICBitmap& _textDrawer, ID3D11Device* _device, float* _baseValue, float* _nextValue)
{
	if (ChPtr::NullCheck(_baseValue))return;
	if (ChPtr::NullCheck(_nextValue))return;
	if (ChPtr::NullCheck(_device))return;
	value = ChPtr::Make_S<ChD3D11::Texture11>();
	CreateTexture(*value, _textDrawer, _device, std::to_string(*_nextValue), ChVec2(PARAMETER_PANEL_VALUE_WIDTH - (PANEL_TEXT_SIDE_ALIGH * 2.0f), PARAMETER_PANEL_HEIGHT));
	baseValue = _baseValue;
	nextValue = _nextValue;
	beforeValue = *nextValue;
}

ParameterOpelator ParameterFValuePanel::GetParameterOperator()
{
	ParameterOpelator res = ParameterOpelator::EQUAL;

	if ((*baseValue - *nextValue) > FLOATING_TEST_VALUE)res = inversOperatorFlg ? ParameterOpelator::BIGGER : ParameterOpelator::SMALLER;
	if ((*nextValue - *baseValue) > FLOATING_TEST_VALUE)res = inversOperatorFlg ? ParameterOpelator::SMALLER : ParameterOpelator::BIGGER;

	return res;
}

void ParameterFValuePanel::UpdateValue(TextDrawerWICBitmap& _textDrawer, ID3D11Device* _device)
{
	if (ChPtr::NullCheck(baseValue))return;
	if (ChPtr::NullCheck(nextValue))return;
	if (ChPtr::NullCheck(_device))return;
	if (std::abs(*nextValue - beforeValue) < FLOATING_TEST_VALUE)return;
	value->Release();
	CreateTexture(*value, _textDrawer, _device, std::to_string(*nextValue), ChVec2(PARAMETER_PANEL_VALUE_WIDTH - (PANEL_TEXT_SIDE_ALIGH * 2.0f), PARAMETER_PANEL_HEIGHT));
	beforeValue = *nextValue;
}

void ParameterList::Init(ID3D11Device* _device, ChPtr::Shared<BaseMecha> _baseMecha)
{
	CreateTextDrawer(textDrawer, static_cast<unsigned long>(PARAMETER_PANEL_WIDTH - (PANEL_TEXT_SIDE_ALIGH * 2.0f)), static_cast<unsigned long>(PARAMETER_PANEL_HEIGHT), TITLE_FONT_SIZE);
	CreateTextDrawer(titleTextDrawer, static_cast<unsigned long>(PARAMETER_PANEL_TITLE_WIDTH - (PANEL_TEXT_SIDE_ALIGH * 2.0f)), static_cast<unsigned long>(PARAMETER_PANEL_HEIGHT), VALUE_TITLE_FONT_SIZE);
	CreateTextDrawer(valueTextDrawer, static_cast<unsigned long>(PARAMETER_PANEL_VALUE_WIDTH - (PANEL_TEXT_SIDE_ALIGH * 2.0f)), static_cast<unsigned long>(PARAMETER_PANEL_HEIGHT), VALUE_TITLE_FONT_SIZE);

	titleBGTexture = ChPtr::Make_S<ChD3D11::Texture11>();
	titleBGTexture->CreateTexture(EDIT_TEXTURE_DIRECTORY("ParameterTitle.png"), _device);

	valueBGTexture = ChPtr::Make_S<ChD3D11::Texture11>();
	valueBGTexture->CreateTexture(EDIT_TEXTURE_DIRECTORY("ParameterValue.png"), _device);

	displays[ChStd::EnumCast(DisplayType::Partial)] = ChPtr::Make_S<ParameterPartialDisplay>();
	displays[ChStd::EnumCast(DisplayType::Entire)] = ChPtr::Make_S<ParameterEntireDisplay>();

	basePartsParameter = ChPtr::Make_S<PartsParameters>();
	nextPartsParameter = ChPtr::Make_S<PartsParameters>();
	baseAllParameter = ChPtr::Make_S<PartsParameters>();
	nextAllParameter = ChPtr::Make_S<PartsParameters>();

	nextParameter = baseParameter = _baseMecha->GetCoreParts();

	auto&& coreBaseObj = baseParameter->GetBaseObject();

	*nextPartsParameter = *basePartsParameter = *coreBaseObj->GetComponent<PartsParameters>();

	AddAllParameterData(*basePartsParameter,baseParameter);

	*nextAllParameter = *baseAllParameter;

	displays[ChStd::EnumCast(DisplayType::Partial)]->Init(titleBGTexture, valueBGTexture, basePartsParameter, nextPartsParameter, _device, textDrawer, titleTextDrawer, valueTextDrawer);
	displays[ChStd::EnumCast(DisplayType::Entire)]->Init(titleBGTexture, valueBGTexture, baseAllParameter, nextAllParameter, _device, textDrawer, titleTextDrawer, valueTextDrawer);

}

void ParameterList::SetBaseParts(ID3D11Device* _device, ChPtr::Shared<MechaPartsObject> _partsObject)
{
	SubParameterData(*basePartsParameter,baseParameter);
	SubAllParameterData(*baseAllParameter,baseParameter);
	AddParameterData(*basePartsParameter,_partsObject);
	AddAllParameterData(*baseAllParameter,_partsObject);
	*nextPartsParameter = *basePartsParameter;
	*nextAllParameter = *baseAllParameter;
	baseParameter = nullptr;
	baseParameter = _partsObject;
	UpdateDisplayParameter(_device);
}

void ParameterList::SetNextParts(ID3D11Device* _device, ChPtr::Shared<MechaPartsObject> _partsObject)
{
	SubParameterData(*nextPartsParameter, baseParameter);
	AddParameterData(*nextPartsParameter, _partsObject);
	baseParameter = nullptr;
	baseParameter = _partsObject;
	UpdateDisplayParameter(_device);
}

void ParameterList::AddParameterData(PartsParameters& _parameter, ChPtr::Shared<MechaPartsObject> _partsObject)
{
	if (_partsObject == nullptr)return;

	auto&& coreBaseObj = _partsObject->GetBaseObject();

	_parameter += *coreBaseObj->GetComponent<PartsParameters>();
}

void ParameterList::AddAllParameterData(PartsParameters& _parameter, ChPtr::Shared<MechaPartsObject> _partsObject)
{
	return;
	if (_partsObject == nullptr)return;

	auto&& coreBaseObj = _partsObject->GetBaseObject();

	_parameter += *coreBaseObj->GetComponent<PartsParameters>();

	auto&& nextPosList = coreBaseObj->GetComponents<NextPos>();

	for (auto&& nextPos : nextPosList)
	{
		std::string nexPosName = nextPos->GetObjectName();
		AddAllParameterData(_parameter,_partsObject->GetChildParts(nexPosName));
	}

}

void ParameterList::SubParameterData(PartsParameters& _parameter, ChPtr::Shared<MechaPartsObject> _partsObject)
{
	if (_partsObject == nullptr)return;

	auto&& coreBaseObj = _partsObject->GetBaseObject();

	_parameter -= *coreBaseObj->GetComponent<PartsParameters>();

}

void ParameterList::SubAllParameterData(PartsParameters& _parameter, ChPtr::Shared<MechaPartsObject> _partsObject)
{
	return;
	if (_partsObject == nullptr)return;

	auto&& coreBaseObj = _partsObject->GetBaseObject();

	_parameter -= *coreBaseObj->GetComponent<PartsParameters>();

	auto&& nextPosList = coreBaseObj->GetComponents<NextPos>();

	for (auto&& nextPos : nextPosList)
	{
		std::string nexPosName = nextPos->GetObjectName();
		SubAllParameterData(_parameter,_partsObject->GetChildParts(nexPosName));
	}

}

bool ParameterList::Update(MenuBase::ActionType _type)
{
	if (_type == MenuBase::ActionType::Right)
	{
		displays[ChStd::EnumCast(displayType)]->Up();
		return true;
	}

	if (_type == MenuBase::ActionType::Left)
	{
		displays[ChStd::EnumCast(displayType)]->Down();
		return true;
	}

	return false;
}

void ParameterList::UpdateDisplayParameter(ID3D11Device* _device)
{
	if (baseParameter == nullptr)return;
	if (nextParameter == nullptr)return;

	for (unsigned char i = 0; i < DISPLAY_TYPE_COUNT; i++)
	{
		displays[i]->Update(_device, textDrawer, titleTextDrawer, valueTextDrawer);;
	}

}

void ParameterList::CreateTextDrawer(TextDrawerWICBitmap& _textDrawer,unsigned long _w,unsigned long _h, float _fontSize)
{
	_textDrawer.bitmap = ChD3D::WICBitmapCreatorObj().CreateBitmapObject(_w,_h);
	_textDrawer.drawer.Init(_w, _h, _textDrawer.bitmap, ChD3D::DirectFontBase::LocaleNameId::Japanese);
	_textDrawer.drawer.SetClearDisplayFlg(true);
	_textDrawer.drawer.SetClearDisplayColor(ChVec4::FromColor(0.0f,0.0f,0.0f,0.0f));
	_textDrawer.format = _textDrawer.drawer.CreateTextFormat(L"ƒƒCƒŠƒI", nullptr, DWRITE_FONT_WEIGHT_ULTRA_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, _fontSize);
	_textDrawer.format.SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	_textDrawer.brush = _textDrawer.drawer.CreateBrush(ChVec4::FromColor(0.0f, 0.0f, 0.0f, 1.0f));
}

void ParameterList::Draw(ChD3D11::Shader::BaseDrawSprite11& _drawer)
{
	if (displays[ChStd::EnumCast(displayType)] == nullptr)return;
	displays[ChStd::EnumCast(displayType)]->Draw(_drawer);
}
