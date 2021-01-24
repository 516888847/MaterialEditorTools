// _(:�١���)_._(:�١���)_._(:�١���)_._(:�١���)_

#pragma once

#include "CoreMinimal.h"

#include "MaterialEditorToolsCore.h"

class FMaterialEditorToolsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	TSharedPtr<FMaterialEditorToolsCore> MaterialEditorGraphActions;
};
