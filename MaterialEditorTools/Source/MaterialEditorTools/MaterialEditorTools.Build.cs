// _(:§Ù¡¹¡Ï)_._(:§Ù¡¹¡Ï)_._(:§Ù¡¹¡Ï)_._(:§Ù¡¹¡Ï)_

using UnrealBuildTool;

public class MaterialEditorTools : ModuleRules
{
	public MaterialEditorTools(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Engine",
				"CoreUObject",
				"Projects",
				"Blutility",
				"ToolMenus",
				"UnrealEd",
				"Slate",
				"SlateCore",
				"MaterialEditor",
				"EditorStyle",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
