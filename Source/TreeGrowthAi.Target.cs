// "// Copyright © 2023 Silas Schuerger, Levin Theil"

using UnrealBuildTool;
using System.Collections.Generic;

public class TreeGrowthAiTarget : TargetRules
{
	public TreeGrowthAiTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("TreeGrowthAi");
	}
}
