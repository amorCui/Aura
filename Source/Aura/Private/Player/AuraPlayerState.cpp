// Copyright Amor

#include "Player/AuraPlayerState.h"

// 包含自定义能力系统组件和属性集的头文件
// AuraAbilitySystemComponent: 自定义的能力系统组件，可能包含额外功能
// AuraAttributeSet: 自定义的属性集，定义了游戏的具体属性
#include <AbilitySystem/AuraAbilitySystemComponent.h>
#include <AbilitySystem/AuraAttributeSet.h>

/**
 * AAuraPlayerState 构造函数
 * 在角色构造函数中创建并初始化Gameplay Ability System (GAS)的核心组件
 * 注意：构造函数中的代码在对象创建时执行，用于设置默认值和初始化组件
 */
AAuraPlayerState::AAuraPlayerState()
{
    /**
     * 创建自定义能力系统组件实例
     * CreateDefaultSubobject<>: 在构造函数中创建UObject子对象的Unreal标准方法
     * 这个方法专门用于在构造函数中创建组件，确保组件正确初始化并被垃圾回收系统管理
     * "AbilitySystemComponent": 组件名称，在编辑器中标识此组件，在蓝图编辑器中显示为组件名
     * UAuraAbilitySystemComponent: 自定义的能力系统组件类，继承自UAbilitySystemComponent
     * 自定义能力系统组件可能添加了额外的功能，如输入绑定、UI交互等
     */
    AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");

    /**
     * 设置能力系统组件的网络复制属性和复制模式
     *
     * 1. 启用能力系统组件的网络复制功能
     * SetIsReplicated(true): 将这个组件标记为需要网络复制，确保在网络游戏中被同步到所有客户端
     * 这是基础设置，告诉网络系统这个组件需要被复制
     * 如果没有启用复制，组件的变化只在本地有效，其他客户端看不到这些变化
     */
    AbilitySystemComponent->SetIsReplicated(true);

    /**
     * 2. 设置能力系统组件的具体复制模式为混合模式
     * SetReplicationMode(): 指定组件如何在不同客户端之间同步数据
     * EGameplayEffectReplicationMode::Mixed: 混合复制模式
     *
     * 混合复制模式提供三种不同情况的处理：
     * - 对于本地控制的角色：服务器和客户端之间完全复制，支持客户端预测
     *   这意味着本地玩家的操作可以立即得到反馈，减少延迟感
     * - 对于其他客户端（观察者）：只复制最小必要数据，节省带宽
     *   其他玩家看到的角色只接收最重要的状态变化，而不是所有细节
     * - 对于AI控制的角色：只在服务器上计算，客户端接收结果
     *
     * 其他复制模式选项：
     * - Full: 完全复制，服务器和所有客户端都接收所有数据（性能开销大）
     * - Minimal: 最小复制，只复制最重要的变化（节省带宽，但客户端可能不知道某些状态）
     * - Mixed: 混合模式，根据控制权不同使用不同的复制策略（推荐用于多人游戏）
     */
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    /**
     * 创建自定义属性集实例
     * CreateDefaultSubobject<>: 同样使用Unreal的标准组件创建方法
     * "AttributeSet": 组件名称，在编辑器中标识此属性集
     * UAuraAttributeSet: 自定义的属性集类，继承自UAttributeSet
     *
     * 属性集是GAS框架中定义和管理角色属性的组件：
     * - 包含所有游戏属性的定义（生命值、法力值、力量、敏捷等）
     * - 处理属性的基础值、当前值和修改器
     * - 通过GameplayEffects进行属性修改
     * - 自动处理属性变化的事件通知
     */
    AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

    /**
     * 设置网络更新频率为100赫兹（每秒更新100次）
     * NetUpdateFrequency: 网络更新频率，控制这个Actor的网络复制更新频率
     *
     * 网络更新频率的影响：
     * - 更高的频率：更及时的同步，但占用更多网络带宽
     * - 更低的频率：节省带宽，但可能导致延迟和不同步
     *
     * 100.f是一个平衡值，适合大多数游戏：
     * - 对于玩家状态这种变化不频繁的对象，100Hz通常足够
     * - 对于快速移动的角色，可能需要更高的频率（如120-240Hz）
     * - 对于静态或变化缓慢的对象，可以使用更低的频率（如10-30Hz）
     *
     * 注意：实际更新频率还受限于网络带宽、服务器性能和其他因素
     * 真正的更新频率可能会低于设置值，特别是在网络条件差的情况下
     */
    // NetUpdateFrequency = 100.f;
    // 使用新api替代直接对属性进行赋值
    SetNetUpdateFrequency(100.f);
    // 注意：可以在后续根据需要调整这个值，优化网络性能

    
}

/**
 * Start IAbilitySystemInterface接口实现部分
 */

 /**
  * AAuraPlayerState::GetAbilitySystemComponent 实现
  * 返回玩家状态的能力系统组件，这是 GAS 框架的标准接口实现
  *
  * @return 指向UAbilitySystemComponent的指针，如果未设置则返回nullptr
  *
  * 功能说明：
  * 1. 实现了IAbilitySystemInterface接口的纯虚函数
  *    IAbilitySystemInterface是GAS框架的标准接口，允许通过统一的方式获取ASC
  * 2. 允许其他系统通过PlayerState获取能力系统组件
  *    - GameplayEffects可以通过ASC应用
  *    - GameplayAbilities可以通过ASC激活
  *    - UI系统可以通过ASC查询属性值
  *    - AI系统可以通过ASC检查能力状态
  * 3. 在多人游戏中特别重要，因为玩家状态在服务器和客户端之间复制
  *    - 服务器拥有权威的ASC状态
  *    - 客户端通过复制的ASC获取同步数据
  * 4. 相比将能力系统放在角色上，放在PlayerState上更适合网络游戏：
  *    - PlayerState在角色死亡和重生时保持不变
  *    - 适合存储永久性的玩家进度和属性
  *    - 避免了角色切换时重新初始化能力系统的复杂性
  */
UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
    /**
     * 直接返回存储在玩家状态中的能力系统组件指针
     * 这是一个简单的getter实现，提供对ASC的安全访问
     *
     * 注意：
     * 1. 返回的是基类指针UAbilitySystemComponent*，提供统一的接口
     * 2. 调用者可以通过这个指针访问所有标准ASC功能
     * 3. 如果需要在外部使用自定义ASC功能，可以进行类型转换
     * 4. 这个指针在构造函数中已经初始化，确保不会返回空指针
     */
    return AbilitySystemComponent;
}

/**
 * End IAbilitySystemInterface接口实现部分
 */