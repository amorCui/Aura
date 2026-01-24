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
    UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

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
 * 典型使用场景示例：
 *
 * 1. 在UI控件的构造函数或初始化函数中：
 *    // 绑定委托
 *    OverlayWidgetController->OnHealthChanged.AddDynamic(this, &UAuraUserWidget::UpdateHealthBar);
 *
 *    // 触发初始更新
 *    OverlayWidgetController->BroadCastInitialValues();
 *
 * 2. 在HUD初始化中：
 *    // 创建控制器并设置参数
 *    WidgetController->SetWidgetControllerParams(Params);
 *
 *    // 广播初始值（UI已经绑定）
 *    WidgetController->BroadCastInitialValues();
 *
 * 3. 在属性变化回调中：
 *    // 当属性变化时，只广播变化的值
 *    void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data)
 *    {
 *        OnHealthChanged.Broadcast(Data.NewValue);
 *    }
 */

 /**
  * 与属性变化监听的配合：
  *
  * 通常，界面控制器会有两部分功能：
  *
  * 1. 广播初始值（此函数）：
  *    - 在开始时调用一次
  *    - 确保UI显示正确的初始状态
  *
  * 2. 监听属性变化（通常在BindCallbacksToDependencies函数中）：
  *    - 订阅属性变化事件
  *    - 当属性变化时，通过委托通知UI更新
  *
  * 这样的设计确保了：
  * 1. UI始终显示最新的属性值
  * 2. UI在初始化时就有正确的值
  * 3. 属性变化时UI能及时更新
  */
