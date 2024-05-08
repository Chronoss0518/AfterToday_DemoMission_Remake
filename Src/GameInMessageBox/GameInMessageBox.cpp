#include"../BaseIncluder.h"
#include"GameInMessageBox.h"

#define MESSAGE_BOX_TOP 585.0f
#define MESSAGE_BOX_LEFT 256

#define MESSAGE_BOX_WIDTH 768.0f
#define MESSAGE_BOX_HEIGHT 99.0f

#define MESSENGER_TEXT_SIZE 20.0f
#define MESSENGER_TEXT_PADDING 5.0f

#define MESSENGER_BOX_WIDTH 260.0f
#define MESSENGER_BOX_HEIGHT 30.0f

#define MESSAGE_TEXT_SIZE 20.0f
#define MESSAGE_TEXT_PADDING 5.0f


void GameInMessageBox::Init(ID3D11Device* _device)
{

	if (ChPtr::NullCheck(_device))return;
	device = _device;

	unsigned long messengerW = static_cast<unsigned long>(MESSENGER_BOX_WIDTH - (MESSENGER_TEXT_PADDING * 2.0f));
	unsigned long messengerH = static_cast<unsigned long>(MESSENGER_BOX_HEIGHT - (MESSENGER_TEXT_PADDING * 2.0f));

	messengerDrawer.bitmap = ChD3D::WICBitmapCreatorObj().CreateBitmapObject(messengerW, messengerH);
	messengerDrawer.drawer.Init(messengerW, messengerH, messengerDrawer.bitmap, ChD3D::DirectFontBase::LocaleNameId::Japanese);
	messengerDrawer.drawer.SetClearDisplayFlg(true);
	messengerDrawer.drawer.SetClearDisplayColor(ChVec4(0.0f));
	messengerDrawer.brush = messengerDrawer.drawer.CreateBrush(ChVec4(1.0f));
	messengerDrawer.format = messengerDrawer.drawer.CreateTextFormat(L"メイリオ", nullptr, DWRITE_FONT_WEIGHT_BLACK, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, MESSENGER_TEXT_SIZE);
	messengerDrawer.format.SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	messengerDrawer.format.SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	unsigned long messageW = static_cast<unsigned long>(MESSAGE_BOX_WIDTH - (MESSAGE_TEXT_PADDING * 2.0f));
	unsigned long messageH = static_cast<unsigned long>(MESSAGE_BOX_HEIGHT - MESSENGER_BOX_HEIGHT - (MESSAGE_TEXT_PADDING * 2.0f));

	messageDrawer.bitmap = ChD3D::WICBitmapCreatorObj().CreateBitmapObject(messageW, messageH);
	messageDrawer.drawer.Init(messageW, messageH, messageDrawer.bitmap, ChD3D::DirectFontBase::LocaleNameId::Japanese);
	messageDrawer.drawer.SetClearDisplayFlg(true);
	messageDrawer.drawer.SetClearDisplayColor(ChVec4(0.0f));
	messageDrawer.brush = messageDrawer.drawer.CreateBrush(ChVec4(1.0f));
	messageDrawer.format = messageDrawer.drawer.CreateTextFormat(L"メイリオ", nullptr, DWRITE_FONT_WEIGHT_BLACK, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, MESSAGE_TEXT_SIZE);
	messageDrawer.format.SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	messageDrawer.format.SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	messageBox.image.CreateTexture(ChStr::UTF8ToWString(TEXTURE_DIRECTORY("MessageBox.png")));
	SPRITE_INIT(messageBox.sprite, RectToGameWindow(ChVec4::FromRect(MESSAGE_BOX_LEFT,MESSAGE_BOX_TOP, MESSAGE_BOX_LEFT + MESSAGE_BOX_WIDTH, MESSAGE_BOX_TOP + MESSAGE_BOX_HEIGHT)));

	SPRITE_INIT(messengerText.sprite, RectToGameWindow(ChVec4::FromRect(MESSAGE_BOX_LEFT + MESSENGER_TEXT_PADDING,MESSAGE_BOX_TOP + MESSENGER_TEXT_PADDING, MESSAGE_BOX_LEFT + messengerW + MESSENGER_TEXT_PADDING, MESSAGE_BOX_TOP + messengerH + MESSENGER_TEXT_PADDING)));
	SPRITE_INIT(messageTextSprite, RectToGameWindow(ChVec4::FromRect(MESSAGE_BOX_LEFT + MESSAGE_TEXT_PADDING,MESSAGE_BOX_TOP + MESSENGER_BOX_HEIGHT + MESSAGE_TEXT_PADDING, MESSAGE_BOX_LEFT + messageW + MESSAGE_TEXT_PADDING, MESSAGE_BOX_TOP + MESSENGER_BOX_HEIGHT + messageH + MESSAGE_TEXT_PADDING)));
}

void GameInMessageBox::Release()
{
	EndAfterTime();
}

void GameInMessageBox::Update()
{
	if (!IsDrawMessage())return;

	if (message != drawMessage)
	{
		messageAddCount--;
		if (messageAddCount >= 0)return;
		messageAddCount = messageAddFrame;
		drawMessage += message[drawMessage.length()];
		nextTextTexture = ChPtr::Make_S<ChD3D11::Texture11>();

		CreateText(*nextTextTexture, messageDrawer, drawMessage, ChVec4::FromRect(0.0f,0.0f,MESSAGE_BOX_WIDTH - MESSAGE_TEXT_PADDING * 2.0f, MESSAGE_BOX_HEIGHT - MESSENGER_BOX_HEIGHT - MESSAGE_TEXT_PADDING * 2.0f));
		return;
	}

	afterFrameCount--;

	if (afterFrameCount >= 0)return;
	EndAfterTime();
}

void GameInMessageBox::Draw(ChD3D11::Shader::BaseDrawSprite11& _drawer)
{
	if (!IsDrawMessage())return;

	if (nextTextTexture != nullptr)
	{
		nowTextTexture = nullptr;
		nowTextTexture = nextTextTexture;
		nextTextTexture = nullptr;
	}

	_drawer.Draw(messageBox.image, messageBox.sprite);

	_drawer.Draw(*nowTextTexture, messageTextSprite);

	_drawer.Draw(messengerText.image, messengerText.sprite);

}

void GameInMessageBox::EndSetDrawMessage()
{
	if (drawMessage == message)return;
	messageAddCount = 0;
	drawMessage = message;
	nextTextTexture = ChPtr::Make_S<ChD3D11::Texture11>();
	CreateText(*nextTextTexture, messageDrawer, drawMessage, ChVec4::FromRect(0.0f, 0.0f, MESSAGE_BOX_WIDTH - MESSAGE_TEXT_PADDING * 2.0f, MESSAGE_BOX_HEIGHT - MESSENGER_BOX_HEIGHT - MESSAGE_TEXT_PADDING * 2.0f));
}

void GameInMessageBox::EndAfterTime()
{
	afterFrameCount = -1;
	nowTextTexture = nullptr;
	messengerText.image.Release();
}

void GameInMessageBox::SetMessage(const std::wstring& _messenger, const std::wstring& _message, unsigned long _afterFrame, unsigned long _messageAddFrame)
{
	if (_message.length() <= 0)return;
	if (_messenger.length() <= 0)return;

	std::wstring testMessage = L"";
	for (unsigned long i = 0; i < _message.length(); i++)
	{
		if (_message[i] == L'\\')
		{
			if (_message[i + 1] == L'n')
			{
				testMessage += L'\n';
				i++;
				continue;
			}
		}
		testMessage += _message[i];
	}
	messageAddFrame = _messageAddFrame;
	messageAddCount = messageAddFrame;
	afterFrameCount = _afterFrame;
	message = testMessage;
	drawMessage = message[0];

	if (messageAddFrame <= 0)
	{
		drawMessage = message;
	}

	nowTextTexture = nullptr;
	nowTextTexture = ChPtr::Make_S<ChD3D11::Texture11>();
	CreateText(*nowTextTexture, messageDrawer, drawMessage, ChVec4::FromRect(0.0f, 0.0f, MESSAGE_BOX_WIDTH - MESSAGE_TEXT_PADDING * 2.0f, MESSAGE_BOX_HEIGHT - MESSENGER_BOX_HEIGHT - MESSAGE_TEXT_PADDING * 2.0f));

	CreateText(messengerText.image, messengerDrawer, _messenger, ChVec4::FromRect(0.0f, 0.0f, MESSENGER_BOX_WIDTH - MESSENGER_TEXT_PADDING * 2.0f, MESSENGER_BOX_HEIGHT - MESSENGER_TEXT_PADDING * 2.0f));
}

void GameInMessageBox::CreateText(ChD3D11::Texture11& _outTexture, TextDrawerClass& _drawer, const std::wstring& _text, const ChVec4& _screenRect)
{
	_drawer.drawer.DrawStart();

	_drawer.drawer.DrawToScreen(_text,_drawer.format,_drawer.brush, _screenRect);

	_drawer.drawer.DrawEnd();

	_outTexture.CreateColorTexture(device,_drawer.bitmap.GetBitmap());
}