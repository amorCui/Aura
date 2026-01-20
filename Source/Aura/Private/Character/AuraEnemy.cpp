// Copyright Amor


// 包含AuraEnemy类的头文件
#include "Character/AuraEnemy.h"

/**
 * HighlightActor 函数
 * 当敌人需要被高亮显示时调用（如玩家选中目标或鼠标悬停）
 * 实现敌人被高亮时的视觉效果，通常用于表示当前选中目标
 */
void AAuraEnemy::HighlightActor()
{
    // 设置高亮标志位为true，表示这个敌人当前被高亮显示
    bHighlighted = true;
}

/**
 * UnHighlightActor 函数
 * 当敌人不再需要高亮时调用（如玩家取消选择或鼠标移开）
 * 恢复敌人的原始外观，移除高亮效果
 */
void AAuraEnemy::UnHighlightActor()
{
    // 设置高亮标志位为false，表示取消高亮
    bHighlighted = false;
}
