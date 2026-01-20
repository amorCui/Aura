// Copyright Amor
// 预编译头指令，防止头文件被多次包含
#pragma once

// 包含Unreal Engine的核心头文件
#include "CoreMinimal.h"
// 包含Unreal Engine的角色基类
#include "GameFramework/Character.h"
// 包含自动生成的代码头文件
#include "AuraCharacterBase.generated.h"

/**
 * Aura角色基类
 * 这是一个抽象基类，用于游戏中所有角色的基础功能
 * 使用Abstract标记，表示不能被直接实例化，必须被子类化
 */ 
UCLASS(Abstract)									  // 标记为抽象类，不能在编辑器中直接创建实例
class AURA_API AAuraCharacterBase : public ACharacter // 继承自Unreal Engine的标准Character类
{
    GENERATED_BODY() // UE4/UE5的宏，用于生成必要的反射代码

public:
    // 构造函数
    AAuraCharacterBase();

protected:
    // 重写父类的BeginPlay函数，在角色开始游戏时调用
    virtual void BeginPlay() override;

    // 武器组件
    // UPROPERTY宏用于向Unreal引擎暴露属性，使其在编辑器中可编辑并支持垃圾回收
    // EditAnywhere: 属性可以在任何地方编辑（实例和蓝图）
    // Category = "Combat": 在编辑器中分类为"Combat"组
    // TObjectPtr是UE5的智能指针类型，用于安全地引用UObject对象
    UPROPERTY(EditAnywhere, Category = "Combat")
    TObjectPtr<USkeletalMeshComponent> Weapon; // 指向武器骨架网格组件的指针
};