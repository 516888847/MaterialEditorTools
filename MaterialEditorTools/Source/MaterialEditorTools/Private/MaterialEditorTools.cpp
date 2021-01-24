// _(:§Ù¡¹¡Ï)_._(:§Ù¡¹¡Ï)_._(:§Ù¡¹¡Ï)_._(:§Ù¡¹¡Ï)_

#include "MaterialEditorTools.h"

#define LOCTEXT_NAMESPACE "FMaterialEditorToolsModule"

void FMaterialEditorToolsModule::StartupModule()
{
#if WITH_EDITOR
	MaterialEditorGraphActions = MakeShareable(new FMaterialEditorToolsCore);
	MaterialEditorGraphActions->Init();
#endif

}

void FMaterialEditorToolsModule::ShutdownModule()
{
#if WITH_EDITOR
	MaterialEditorGraphActions->Shutdown();
#endif
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMaterialEditorToolsModule, MaterialEditorTools)