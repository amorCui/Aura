// Copyright Amor


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
/**
 * 广播初始属性值函数
 * 此函数在界面控制器初始化时调用，用于向UI广播属性的初始值
 * 确保UI在显示时具有正确的初始状态，避免显示为默认值或空白
 *
 * 功能说明：
 * 1. 从属性集中获取当前属性值
 * 2. 通过委托系统广播这些值到所有监听的UI元素
 * 3. 通常在UI初始化、角色创建或重新绑定时调用
 *
 * 设计模式：观察者模式（Observer Pattern）
 * 控制器作为发布者（Publisher），UI控件作为订阅者（Subscriber）
 * 当属性值变化时，控制器通知所有订阅的UI更新显示
 *
 * 调用时机：
 * 1. 玩家角色首次创建时
 * 2. 界面控制器初始化完成时
 * 3. 游戏状态重置时
 * 4. 角色属性被重新绑定时
 */
void UOverlayWidgetController::BroadCastInitialValues()
{
    /**
     * 安全类型转换：将基类UAttributeSet指针转换为具体的UAuraAttributeSet类型
     * CastChecked<>: 安全的类型转换，如果转换失败会触发断言（开发阶段）或崩溃（发布阶段）
     *
     * 为什么要进行类型转换：
     * 1. AttributeSet是基类指针，只能访问基类的通用功能
     * 2. UAuraAttributeSet包含具体的属性访问函数（如GetHealth、GetMana）
     * 3. 转换后可以使用类型安全的属性获取方法
     *
     * 安全考虑：
     * 1. 确保AttributeSet不为nullptr
     * 2. 确保AttributeSet确实是UAuraAttributeSet类型
     * 3. 如果转换失败，程序会在开发阶段立即停止，有助于快速发现问题
     */
    const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

    /**
     * 广播生命值（Health）初始值
     * OnHealthChanged: 生命值变化委托，UI控件可以绑定到这个委托
     * Broadcast(): 触发委托，通知所有监听者当前的生命值
     *
     * 数据流向：
     * 属性集(AttributeSet) → 控制器(WidgetController) → UI控件(UserWidget)
     *
     * 为什么需要广播初始值：
     * 1. UI在初始化时需要知道当前的属性值，而不是显示默认值
     * 2. 确保UI与游戏状态同步，避免显示不一致
     * 3. 支持UI控件的延迟加载或动态创建
     */
    OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());

    /**
     * 广播最大生命值（MaxHealth）初始值
     * OnMaxHealthChanged: 最大生命值变化委托
     *
     * 最大生命值的作用：
     * 1. 确定生命条的满值（100%）
     * 2. 计算生命值百分比（当前生命值/最大生命值）
     * 3. 限制生命值的上限（不能超过最大生命值）
     */
    OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());

    /**
     * 广播法力值（Mana）初始值
     * OnManaChanged: 法力值变化委托
     *
     * 法力值的用途：
     * 1. 施放技能消耗的资源
     * 2. 某些技能的效果可能与法力值相关
     * 3. 法力值恢复速率可能影响技能使用频率
     */
    OnManaChanged.Broadcast(AuraAttributeSet->GetMana());

    /**
     * 广播最大法力值（MaxMana）初始值
     * OnMaxManaChanged: 最大法力值变化委托
     *
     * 注意：广播顺序通常不重要，但应该保持一致
     * 先广播基础值再广播最大值，或反之，确保UI正确处理
     */
    OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());

}

/**
 * 绑定回调到依赖项函数
 * 此函数用于订阅Gameplay Ability System(GAS)中属性变化的事件
 * 当游戏中的属性值发生变化时，GAS会触发这些事件，我们通过回调函数接收并处理
 *
 * 功能说明：
 * 1. 获取属性变化的委托（Delegate）
 * 2. 将控制器类的成员函数绑定到这些委托上
 * 3. 当属性变化时，GAS自动调用绑定的回调函数
 *
 * 设计模式：观察者模式 + 回调模式
 * 控制器作为观察者，订阅GAS的属性变化事件，当事件发生时更新UI
 *
 * 调用时机：
 * 1. 在控制器初始化完成并设置好AbilitySystemComponent和AttributeSet之后
 * 2. 通常在SetWidgetControllerParams函数之后调用
 * 3. 确保在游戏开始前建立好所有的事件绑定
 */
void UOverlayWidgetController::BindCallbacksToDependencies()
{
    /**
     * 安全类型转换：将基类UAttributeSet指针转换为具体的UAuraAttributeSet类型
     * CastChecked<>: 安全的类型转换，如果转换失败会触发断言
     *
     * 注意：这里使用const指针，因为我们只需要读取属性，不修改属性集本身
     * 属性值的修改应该通过GameplayEffects进行，而不是直接修改AttributeSet
     */
    const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

    /**
     * 绑定生命值变化回调
     * GetGameplayAttributeValueChangeDelegate(): GAS提供的函数，返回指定属性的变化委托
     *
     * 参数说明：
     * - AuraAttributeSet->GetHealthAttribute(): 获取生命值属性的标识符（FGameplayAttribute）
     *
     * 绑定方法：
     * AddUObject(): 将UObject对象的成员函数绑定到委托上
     *
     * 绑定参数：
     * - this: 拥有回调函数的对象（当前控制器实例）
     * - &UOverlayWidgetController::HealthChanged: 回调函数指针
     *
     * 工作原理：
     * 当生命值属性通过GameplayEffect或其他方式改变时，GAS会触发这个委托
     * 委托会调用所有绑定的回调函数，包括这里的HealthChanged函数
     */
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute())
        .AddUObject(this, &UOverlayWidgetController::HealthChanged);

    /**
     * 绑定最大生命值变化回调
     * 与生命值类似，监听最大生命值属性的变化
     */
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute())
        .AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);

    /**
     * 绑定法力值变化回调
     * 监听法力值属性的变化
     */
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute())
        .AddUObject(this, &UOverlayWidgetController::ManaChanged);

    /**
     * 绑定最大法力值变化回调
     * 监听最大法力值属性的变化
     */
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute())
        .AddUObject(this, &UOverlayWidgetController::MaxManaChanged);

    /**
     * 可以在这里添加更多属性的绑定：
     *
     * 示例：
     * AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetStaminaAttribute())
     *     .AddUObject(this, &UOverlayWidgetController::StaminaChanged);
     */

    
}

/**
 * 生命值变化回调函数
 * 当游戏中的生命值属性发生变化时，GAS自动调用此函数
 *
 * @param Data 属性变化数据，包含：
 *     - OldValue: 变化前的旧值
 *     - NewValue: 变化后的新值
 *     - GEModData: GameplayEffect修改的数据（如来源、原因等）
 *
 * const修饰符说明：
 * 1. 函数被声明为const，因为它不会修改控制器对象的状态
 * 2. 只是将新值广播给UI，不改变控制器内部数据
 * 3. 符合const正确性，可以在const对象上调用
 */
void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
    /**
     * 广播新的生命值给所有监听的UI元素
     * OnHealthChanged: 之前声明的动态多播委托
     * Broadcast(): 触发委托，传递新的生命值
     *
     * 数据流向：
     * GAS属性变化 → HealthChanged回调 → OnHealthChanged委托 → UI更新函数
     */
    OnHealthChanged.Broadcast(Data.NewValue);

    /**
     * 可选：添加调试或额外逻辑
     *
     * 示例：
     * // 打印变化信息
     * UE_LOG(LogTemp, Log, TEXT("Health changed: %.1f -> %.1f (Delta: %.1f)"),
     *        Data.OldValue, Data.NewValue, Data.NewValue - Data.OldValue);
     *
     * // 检查生命值是否为0（角色死亡）
     * if (Data.NewValue <= 0.0f)
     * {
     *     OnCharacterDied.Broadcast();
     * }
     */
}

/**
 * 最大生命值变化回调函数
 * 当最大生命值属性变化时调用
 *
 * @param Data 属性变化数据
 *
 * 功能说明：
 * 1. 接收新的最大生命值
 * 2. 广播给UI更新（如血条的最大长度）
 */
void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
    /**
     * 广播新的最大生命值
     * UI可以使用这个值重新计算血条的百分比
     * 例如：当前生命值 / 最大生命值 = 血条填充百分比
     */
    OnMaxHealthChanged.Broadcast(Data.NewValue);
}

/**
 * 法力值变化回调函数
 * 当法力值属性变化时调用
 *
 * @param Data 属性变化数据
 */
void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
    /**
     * 广播新的法力值
     * 用于更新法力条和法力值文本
     */
    OnManaChanged.Broadcast(Data.NewValue);
}

/**
 * 最大法力值变化回调函数
 * 当最大法力值属性变化时调用
 *
 * @param Data 属性变化数据
 */
void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
    /**
     * 广播新的最大法力值
     * 用于更新法力条的最大长度和法力值上限文本
     */
    OnMaxManaChanged.Broadcast(Data.NewValue);
}

/**
 * 完整的数据流说明：
 *
 * 1. 游戏逻辑修改属性（通过GameplayEffect）：
 *    GameplayEffect → AbilitySystemComponent → AttributeSet
 *
 * 2. GAS触发属性变化事件：
 *    AttributeSet → GameplayAttributeValueChangeDelegate
 *
 * 3. 控制器接收事件并处理：
 *    GameplayAttributeValueChangeDelegate → BindCallbacksToDependencies绑定的回调函数
 *
 * 4. 控制器广播给UI：
 *    回调函数 → OnXXXChanged委托 → UI更新函数
 *
 * 5. UI更新显示：
 *    UI更新函数 → 更新血条、文本、动画等
 */

 




