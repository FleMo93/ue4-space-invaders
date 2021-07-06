

using UnrealBuildTool;
using System.Collections.Generic;

public class SpaveInvadersTarget : TargetRules
{
	public SpaveInvadersTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "SpaveInvaders" } );
	}
}
