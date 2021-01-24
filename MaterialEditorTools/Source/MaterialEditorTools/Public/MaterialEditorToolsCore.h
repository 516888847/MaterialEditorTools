// _(:§Ù¡¹¡Ï)_._(:§Ù¡¹¡Ï)_._(:§Ù¡¹¡Ï)_._(:§Ù¡¹¡Ï)_

#pragma once

#include "CoreMinimal.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Widgets/Layout/SBox.h"

class FMaterialEditorToolsCore : public TSharedFromThis<FMaterialEditorToolsCore>
{
public:
	void Init();
	void Shutdown();
public:
	UMaterialInstanceConstant* PB_MatInstanceObj;
	bool IgnoreCustomDefaultValueCheck = true;
	bool IgnoreHasConnectionCheck = true;
	FText PrefixNameText;
	FText RemoveNameText;
	FText GroupNameText;
private:
	TSharedPtr<FEditableTextBoxStyle> TextBoxStyle = MakeShareable(new FEditableTextBoxStyle);
	TSharedPtr<FButtonStyle> PromoteButtonStyle = MakeShareable(new FButtonStyle);
	TSharedPtr<FButtonStyle> PromoteInfoButtonStyle = MakeShareable(new FButtonStyle);
	TSharedPtr<FExtender> MaterialGraphEditorToolbarExtender = MakeShareable(new FExtender());
private:
	TSharedPtr<SBox> PromoteInfoPanelPtr;
	TSharedPtr<const FExtensionBase> MaterialGraphEditorExtension;

private:
	void AddToolBarExtension(FToolBarBuilder &builder);
	FReply OnButtonClick_PromoteAllPinsToParameters();
};
