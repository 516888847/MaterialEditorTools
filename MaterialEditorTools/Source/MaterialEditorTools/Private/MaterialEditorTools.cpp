// _(:�١���)_._(:�١���)_._(:�١���)_._(:�١���)_

#include "MaterialEditorTools.h"

#define LOCTEXT_NAMESPACE "FMaterialEditorToolsModule"

void FMaterialEditorToolsModule::StartupModule()
{
#if WITH_EDITOR
#if ENGINE_MINOR_VERSION >= 25
	MaterialEditorGraphActions = MakeShareable(new FMaterialEditorToolsCore);
	MaterialEditorGraphActions->Init();
#endif
#endif
}

void FMaterialEditorToolsModule::ShutdownModule()
{
#if WITH_EDITOR
#if ENGINE_MINOR_VERSION >= 25
	MaterialEditorGraphActions->Shutdown();
#endif
#endif
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMaterialEditorToolsModule, MaterialEditorTools)