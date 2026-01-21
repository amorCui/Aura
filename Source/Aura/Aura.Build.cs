// Copyright Amor

using UnrealBuildTool;

public class Aura : ModuleRules
{
    public Aura(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
    
        PublicDependencyModuleNames.AddRange(new string[] 
        { 
            "Core", 
            "CoreUObject", 
            "Engine", 
            "InputCore",
            "EnhancedInput", 
            "GameplayAbilities"  
        });

        // 私有模块依赖声明
        // 当前模块需要依赖以下模块，但这些模块的头文件不会暴露给当前模块的调用者
        // 这有助于减少编译依赖，提高编译速度
        PrivateDependencyModuleNames.AddRange(new string[]
        {

            "GameplayTags",       // 游戏标签系统模块：用于管理和查询游戏对象的状态标签
            "GameplayTasks"       // 游戏任务系统模块：用于创建和管理游戏中的任务、目标系统
        });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
