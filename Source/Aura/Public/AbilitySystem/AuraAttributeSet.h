// Copyright Amor

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraAttributeSet.generated.h"

/**
 * ATTRIBUTE_ACCESSORS 自定义宏
 *
 * 这是一个组合宏，用于一次性为Gameplay Ability System(GAS)中的属性生成所有必要的访问器函数。
 * 它集成了四个GAS标准宏，简化了属性访问器的声明过程。
 *
 * 宏功能说明：
 * 这个宏会为指定的属性生成以下四个标准访问器函数：
 *
 * 1. GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)
 *    - 生成静态函数：static FGameplayAttribute Get{PropertyName}Attribute();
 *    - 返回属性的FGameplayAttribute标识符，用于在GAS系统中唯一标识该属性
 *    - 示例：static FGameplayAttribute GetHealthAttribute();
 *
 * 2. GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)
 *    - 生成成员函数：float Get{PropertyName}() const;
 *    - 返回属性的当前值（float类型），从FGameplayAttributeData中提取当前值
 *    - 示例：float GetHealth() const;
 *
 * 3. GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)
 *    - 生成成员函数：void Set{PropertyName}(float NewVal);
 *    - 设置属性的当前值，通常用于调试或特殊场景
 *    - 注意：在GAS中，通常通过GameplayEffects修改属性，而不是直接调用Setter
 *    - 示例：void SetHealth(float NewVal);
 *
 * 4. GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
 *    - 生成成员函数：void Init{PropertyName}(float NewVal);
 *    - 初始化属性的基础值，通常在角色创建时调用
 *    - 与Setter不同，Initter只设置BaseValue，不影响CurrentValue的Modifier
 *    - 示例：void InitHealth(float NewVal);
 *
 * 使用示例：
 *
 * 在头文件UAuraAttributeSet.h中：
 *
 * UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
 * FGameplayAttributeData Health;
 * ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);
 *
 * 在源文件UAuraAttributeSet.cpp中：
 *
 * // 这些函数会自动实现，通常不需要额外的代码
 * // 但需要确保包含相应的宏定义
 *
 * 使用ATTRIBUTE_ACCESSORS宏后，我们可以这样使用：
 *
 * // 获取属性标识符
 * FGameplayAttribute HealthAttr = UAuraAttributeSet::GetHealthAttribute();
 *
 * // 获取属性当前值
 * float CurrentHealth = MyAttributeSet->GetHealth();
 *
 * // 设置属性值（谨慎使用，通常用于调试）
 * MyAttributeSet->SetHealth(100.0f);
 *
 * // 初始化属性基础值
 * MyAttributeSet->InitHealth(100.0f);
 *
 *
 * 注意事项：
 * 1. 这个宏应该在属性声明之后立即使用
 * 2. 需要在头文件中定义，确保编译器生成函数声明
 * 3. 函数实现在GAS框架中自动提供，通常不需要手动实现
 * 4. Setter函数直接修改属性值，绕过GAS的正常流程，可能导致同步问题
 * 5. Initter函数应在属性初始化时调用，而不是运行时
 *

 * 使用场景建议：
 *
 * 1. 对于所有GAS属性都使用这个宏，确保一致性
 * 2. 在属性集的头文件中统一使用
 * 3. 为每个属性单独一行，便于阅读和维护
 * 4. 配合详细的注释说明每个属性的用途
 *
 */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * AURA角色属性集类
 * 继承自Unreal Engine的UAttributeSet，用于定义和管理游戏中的角色属性
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    /** 构造函数 */
    UAuraAttributeSet();

    /**
     * 重写父类函数，用于声明需要网络复制的属性
     * @param OutLifetimeProps 输出参数，存储需要网络复制的属性列表
     */
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    //=============================================
    // 基础生命值属性
    //=============================================

    /**
     * 当前生命值属性
     * BlueprintReadOnly: 在蓝图中只读
     * ReplicatedUsing = OnRep_Health: 网络复制时使用指定的回调函数
     * Category = "Vital Attributes": 在编辑器中的分类为"Vital Attributes"
     */
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);

    /**
     * 最大生命值属性
     * 定义角色能够达到的最大生命值上限
     */
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);

    //=============================================
    // 基础法力值属性
    //=============================================

    /**
     * 当前法力值属性
     * 用于施放技能消耗的资源
     */
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
    FGameplayAttributeData Mana;
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);

    /**
     * 最大法力值属性
     * 定义角色能够达到的最大法力值上限
     */
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attributes")
    FGameplayAttributeData MaxMana;
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);

    //=============================================
    // 属性复制回调函数
    // 当属性在服务器端发生变化并在客户端复制时触发
    //=============================================

    /**
     * 生命值变化时的网络复制回调
     * @param OldHealth 变化前的生命值，用于比较和计算差值
     */
    UFUNCTION()
    void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

    /**
     * 最大生命值变化时的网络复制回调
     * @param OldMaxHealth 变化前的最大生命值
     */
    UFUNCTION()
    void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

    /**
     * 法力值变化时的网络复制回调
     * @param OldMana 变化前的法力值
     */
    UFUNCTION()
    void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

    /**
     * 最大法力值变化时的网络复制回调
     * @param OldMaxMana 变化前的最大法力值
     */
    UFUNCTION()
    void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

};
