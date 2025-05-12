// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class ServerTest : ModuleRules
{
	public ServerTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

        //string ThirdPartyPath = "../../ThirdParty/";
        //PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "include"));
        //string LibrariesPath = Path.Combine(ThirdPartyPath, "lib");
        //DirectoryInfo d = new DirectoryInfo(LibrariesPath);
        //FileInfo[] Files = d.GetFiles("*.lib");
        //foreach (FileInfo file in Files)
        //{
        //    PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, file.Name));
        //}

        //Definitions.Add(string.Format("WITH_GRPC_BINDING=1"));

    }
}

