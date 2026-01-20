// Copyright Amor


// 包含AuraEnemy类的头文件
#include "Character/AuraEnemy.h"

#include "Aura/Aura.h"


AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

/**
 * HighlightActor 函数
 * 当敌人需要被高亮显示时调用（如玩家选中目标或鼠标悬停）
 * 实现敌人被高亮时的视觉效果，通常用于表示当前选中目标
 */
void AAuraEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

/**
 * UnHighlightActor 函数
 * 当敌人不再需要高亮时调用（如玩家取消选择或鼠标移开）
 * 恢复敌人的原始外观，移除高亮效果
 */
void AAuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);

}



