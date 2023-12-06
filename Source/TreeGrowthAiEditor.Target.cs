// Copyright © 2023 Silas Schuerger, Levin Theil

using UnrealBuildTool;
using System.Collections.Generic;

public class TreeGrowthAiEditorTarget : TargetRules
{
	public TreeGrowthAiEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("TreeGrowthAi");
	}
}
