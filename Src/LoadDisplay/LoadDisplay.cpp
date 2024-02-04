#include"../../../BaseIncluder.h"

#include"../../../AllStruct.h"

#include"../BaseMecha/BaseMecha.h"
#include"../BaseMecha/MechaPartsObject.h"
#include"../BaseMecha/MechaParts.h"

#include"LoadDisplay.h"

#define MECHA_PREVIEW_WIDTH 0.0f
#define MECHA_PREVIEW_HEIGHT 0.0f

void LoadDisplay::Release()
{
	if (loadMechaList.empty())return;
	loadMechaList.clear();
	openFlg = false;
}

std::string LoadDisplay::GetSelectMechaPath()
{


	return nullptr;
}

bool LoadDisplay::Update()
{
	if (!openFlg)return false;
}

void LoadDisplay::Draw(ChD3D11::Shader::BaseDrawSprite11& _spriteShader)
{
	if (!openFlg)return;

}

void LoadDisplay::Open(ID3D11Device* _device, ChD3D11::Shader::BaseDrawMesh11& _meshDrawer)
{
	drawNowSelect = 0;
	loadPath = "";

	for (auto&& file : std::filesystem::directory_iterator(PLAYER_MECHA_PATH("")))
	{
		std::wstring path = (file.path().c_str());

		auto&& loadMecha = ChPtr::Make_S<LoaderPanel>();
		loadMecha->mecha = ChPtr::Make_S<BaseMecha>();
		loadMecha->mecha->Create(ChVec2(1.0f, 1.0f), _meshDrawer, nullptr);
		loadMecha->mecha->Load(_device, ChStr::UTF8ToString(path));

		loadMecha->mechaTexture.CreateRenderTarget(_device, static_cast<unsigned long>(MECHA_PREVIEW_WIDTH), static_cast<unsigned long>(MECHA_PREVIEW_HEIGHT));
	}
	openFlg = true;
}

void LoadDisplay::Close()
{
	if (loadMechaList.empty())
		loadMechaList.clear();

	openFlg = false;
}

void LoadDisplay::Select()
{

}