#pragma once

#include"../Frames/EditFrame.h"

class AttackList;
class AttackListItem;

class SetWeaponDisplay : public EditFrame::EditFrameDisplayBase
{
public:

	void Init(EditFrame* _frame)override;

	void Update(MenuBase::ActionType _type)override;

	void UpdateMouse()override;

	void Draw(ChD3D11::Shader::BaseDrawSprite11& _spriteShader)override;

public:

	void InitAttackList();

	void ReleaseAtackList();

private:

	ChD3D11::Texture11 backGroundTexture;
	ChD3D11::Sprite11 backgroundSprite;

	ChPtr::Shared<AttackList>attackList = nullptr;
	ChPtr::Shared<AttackListItem>cancelPanel = nullptr;


};