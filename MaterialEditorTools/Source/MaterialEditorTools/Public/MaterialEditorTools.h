// _(:§Ù¡¹¡Ï)_._(:§Ù¡¹¡Ï)_._(:§Ù¡¹¡Ï)_._(:§Ù¡¹¡Ï)_

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
