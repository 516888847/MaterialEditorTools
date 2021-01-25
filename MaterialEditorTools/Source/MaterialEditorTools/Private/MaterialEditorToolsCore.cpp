// _(:з」∠)_._(:з」∠)_._(:з」∠)_._(:з」∠)_

#include "MaterialEditorToolsCore.h"
#include "MaterialEditorToolsHelper.h"



void FMaterialEditorToolsCore::Init()
{

	IMaterialEditorModule* MaterialEditorModule = &FModuleManager::LoadModuleChecked<IMaterialEditorModule>("MaterialEditor");
	
	MaterialGraphEditorExtension = MaterialGraphEditorToolbarExtender->AddToolBarExtension(
		"Graph",
		EExtensionHook::After, 
		nullptr, 
		FToolBarExtensionDelegate::CreateRaw(this, &FMaterialEditorToolsCore::AddToolBarExtension)
	);
	MaterialEditorModule->GetToolBarExtensibilityManager()->AddExtender(MaterialGraphEditorToolbarExtender);
	
	//UIstyle

	*TextBoxStyle = FEditableTextBoxStyle::GetDefault();
	*PromoteInfoButtonStyle = FButtonStyle::GetDefault();
	*PromoteInfoButtonStyle = FButtonStyle::GetDefault();

	TextBoxStyle->Font.Size = 8;
	TextBoxStyle->ForegroundColor = FSlateColor(FLinearColor(1, 1, 1));
	TextBoxStyle->BackgroundColor = FSlateColor(FLinearColor(0.1, 0.1, 0.1));

	FSlateBrush* PB_NormalBrush = new FSlateBrush();
	FSlateBrush* PIB_NormalBrush = new FSlateBrush();

	FMaterialEditorToolsHelper::CopyContentFolderFromResource();
	
	FStringAssetReference PB_MatPath("MaterialInstanceConstant'/MaterialEditorTools/DONOTUSE_PromoteButtonMat_Inst.DONOTUSE_PromoteButtonMat_Inst'");
	UObject* PB_MatObj = PB_MatPath.TryLoad();
	if (PB_MatObj)
	{
		PB_MatInstanceObj = Cast<UMaterialInstanceConstant>(PB_MatObj);
		if(PB_MatInstanceObj)
		{
			float forcheck = 0;
			if (PB_MatInstanceObj->GetScalarParameterValue(FMaterialParameterInfo("HBSW"), forcheck))
			{
				PB_MatInstanceObj->SetScalarParameterValueEditorOnly(FMaterialParameterInfo("HBSW"), 0);
				PB_NormalBrush->SetResourceObject(PB_MatInstanceObj);
			}
		}

	}
	
	PromoteButtonStyle->SetNormal(*PB_NormalBrush);
	PromoteButtonStyle->SetDisabled(*PB_NormalBrush);
	PromoteButtonStyle->SetHovered(*PB_NormalBrush);
	PromoteButtonStyle->SetPressed(*PB_NormalBrush);
	

	FStringAssetReference PIB_MatPath("MaterialInstanceConstant'/MaterialEditorTools/DONOTUSE_PromoteInfoButtonMat.DONOTUSE_PromoteInfoButtonMat'");
	UObject* PIB_MatObj = PIB_MatPath.TryLoad();
	if (PIB_MatObj)
	{
		PIB_NormalBrush->SetResourceObject(PIB_MatObj);
	}
	PIB_NormalBrush->SetImageSize(FVector2D(20, 20));
	PromoteInfoButtonStyle->SetNormal(*PIB_NormalBrush);
	PromoteInfoButtonStyle->SetDisabled(*PIB_NormalBrush);
	PromoteInfoButtonStyle->SetHovered(*PIB_NormalBrush);
	PromoteInfoButtonStyle->SetPressed(*PIB_NormalBrush);
}

void FMaterialEditorToolsCore::Shutdown()
{
	MaterialGraphEditorToolbarExtender->RemoveExtension(MaterialGraphEditorExtension.ToSharedRef());
	MaterialGraphEditorExtension.Reset();
	MaterialGraphEditorToolbarExtender.Reset();

}

void FMaterialEditorToolsCore::AddToolBarExtension(FToolBarBuilder& builder)
{
	FText PromoteButtonToolTips = FText::FromString("Promote all pins of selected nodes to parameters.");
	FText PromoteInfoButtonToolTips = FText::FromString("Open info table");
	FText PrefixNameToolTips = FText::FromString("The text will be added to the beginning of the parameter name.");
	FText RemoveNameToolTips = FText::FromString("The text in the parameter name will be deleted.");
	FText GroupNameToolTips = FText::FromString("The group name of the parameter will be set to this.");
	FText IgnoreCustomDefaultValueCheckBoxToolTips = FText::FromString("After checking, the pins that default value is CustomExpressions will be ignored.\nTexture and Boolean are not affected.");
	FText IgnoreHasConnectionCheckBoxToolTips = FText::FromString("After checking, the pins that have links will be ignored.\nNote: The copied node sometimes recognizes anomalies, just close the material editor and open it again.");
	
	auto PB_ButtonHovered = [this]()
	{
		float forcheck = 0;
		if (PB_MatInstanceObj->GetScalarParameterValue(FMaterialParameterInfo("HBSW"), forcheck))
		{
			PB_MatInstanceObj->SetScalarParameterValueEditorOnly(FMaterialParameterInfo("HBSW"), 1);
		}
	};
	auto PB_ButtonUnHovered = [this]()
	{
		float forcheck = 0;
		if (PB_MatInstanceObj->GetScalarParameterValue(FMaterialParameterInfo("HBSW"), forcheck))
		{
			PB_MatInstanceObj->SetScalarParameterValueEditorOnly(FMaterialParameterInfo("HBSW"), 0);
		}
	};
	auto ICV_ChangeCheck = [this](ECheckBoxState CS)
	{
		switch (CS)
		{
		case ECheckBoxState::Checked:IgnoreCustomDefaultValueCheck = true; break;
		case ECheckBoxState::Unchecked:IgnoreCustomDefaultValueCheck = false; break;
		default:IgnoreCustomDefaultValueCheck = false; break;
		}
	};
	auto ILP_ChangeCheck = [this](ECheckBoxState CS)
	{
		switch (CS)
		{
		case ECheckBoxState::Checked:IgnoreHasConnectionCheck = true;break;
		case ECheckBoxState::Unchecked:IgnoreHasConnectionCheck = false;break;
		default:IgnoreHasConnectionCheck = false; break;
		}
	};
	auto PN_OnTextChange = [this](const FText& Text)
	{
		PrefixNameText = Text;
	};
	auto GN_OnTextChange = [this](const FText& Text)
	{
		GroupNameText = Text;
	};
	auto RN_OnTextChange = [this](const FText& Text)
	{
		RemoveNameText = Text;
	};
	auto InfoButton = [this]()
	{
		if (PromoteInfoPanelPtr!=nullptr)
		{
			if (PromoteInfoPanelPtr->GetVisibility()==EVisibility::Visible)
			{
				PromoteInfoPanelPtr->SetVisibility(EVisibility::Collapsed);
			}else
			{
				PromoteInfoPanelPtr->SetVisibility(EVisibility::Visible);
			}
		}
		return FReply::Handled();
	};

	TSharedRef<SCheckBox> IgnoreCustomDefaultValueCheckBox = SNew(SCheckBox).ToolTipText(IgnoreCustomDefaultValueCheckBoxToolTips).OnCheckStateChanged_Lambda(ICV_ChangeCheck);
	TSharedRef<SCheckBox> IgnoreHasConnectionCheckBox = SNew(SCheckBox).ToolTipText(IgnoreHasConnectionCheckBoxToolTips).OnCheckStateChanged_Lambda(ILP_ChangeCheck);
	IgnoreCustomDefaultValueCheckBox->SetIsChecked(IgnoreCustomDefaultValueCheck ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
	IgnoreHasConnectionCheckBox->SetIsChecked(IgnoreHasConnectionCheck ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);

	TSharedRef<SEditableTextBox> PrefixName = SNew(SEditableTextBox).Style(TextBoxStyle.Get()).ToolTipText(PrefixNameToolTips).OnTextChanged_Lambda(PN_OnTextChange).Text(PrefixNameText);
	TSharedRef<SEditableTextBox> RemoveName = SNew(SEditableTextBox).Style(TextBoxStyle.Get()).ToolTipText(RemoveNameToolTips).OnTextChanged_Lambda(RN_OnTextChange).Text(RemoveNameText);
	TSharedRef<SEditableTextBox> GroupName = SNew(SEditableTextBox).Style(TextBoxStyle.Get()).ToolTipText(GroupNameToolTips).OnTextChanged_Lambda(GN_OnTextChange).Text(GroupNameText);

	TSharedRef<SButton> PromoteButton = SNew(SButton)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.ContentPadding(0)
		.OnClicked_Raw(this, &FMaterialEditorToolsCore::OnButtonClick_PromoteAllPinsToParameters)
		.ButtonStyle(PromoteButtonStyle.Get())
		.ToolTipText(PromoteButtonToolTips)
		.OnHovered_Lambda(PB_ButtonHovered)
		.OnUnhovered_Lambda(PB_ButtonUnHovered)
		;
	TSharedRef<SBox> PromoteButtonBox =
		SNew(SBox).HeightOverride(55).WidthOverride(55)
		[
			PromoteButton
		];

	TSharedRef<SBox> PromoteInfoPanel = SNew(SBox);
	if (PromoteInfoPanelPtr!=nullptr)
	{
		PromoteInfoPanel = PromoteInfoPanelPtr.ToSharedRef();
	}else
	{
		PromoteInfoPanel =
			SNew(SBox).HeightOverride(54).WidthOverride(150).Visibility(EVisibility::Collapsed)[
				SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
					.FillHeight(1.0f)
					.Padding(0.0f, 0.5f, 1.0f, 0.5f)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
					.FillWidth(0.25f)
					[
						SNew(STextBlock).Text(FText::FromString("Pfx")).ToolTipText(PrefixNameToolTips)
					]
				+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						PrefixName
					]
					]
				+ SVerticalBox::Slot()
					.FillHeight(1.0f)
					.Padding(0.0f, 0.5f, 1.0f, 0.5f)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
					.FillWidth(0.25f)
					[
						SNew(STextBlock).Text(FText::FromString("Grp")).ToolTipText(GroupNameToolTips)
					]
				+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						GroupName
					]
					]
				+ SVerticalBox::Slot()
					.FillHeight(1.0f)
					.Padding(0.0f, 0.5f, 1.0f, 0.5f)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
					.FillWidth(0.25f)
					[
						SNew(STextBlock).Text(FText::FromString("Rm")).ToolTipText(RemoveNameToolTips)
					]
				+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						RemoveName
					]
					]
					]
				+ SHorizontalBox::Slot()
					.FillWidth(0.5f)
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
					.AutoHeight().VAlign(VAlign_Top)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
					.AutoWidth().VAlign(VAlign_Top).HAlign(HAlign_Left)
					[
						IgnoreCustomDefaultValueCheckBox
					]
				+ SHorizontalBox::Slot()
					.AutoWidth().VAlign(VAlign_Top).HAlign(HAlign_Left)
					[
						SNew(STextBlock)
						.Text(FText::FromString("IgnCV")).ToolTipText(IgnoreCustomDefaultValueCheckBoxToolTips)
					]
					]
				+ SVerticalBox::Slot()
					.AutoHeight().VAlign(VAlign_Top)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
					.AutoWidth().VAlign(VAlign_Top).HAlign(HAlign_Left)
					[
						IgnoreHasConnectionCheckBox
					]
				+ SHorizontalBox::Slot()
					.AutoWidth().VAlign(VAlign_Top).HAlign(HAlign_Left)
					[
						SNew(STextBlock)
						.Text(FText::FromString("IgnLP")).ToolTipText(IgnoreHasConnectionCheckBoxToolTips)
					]
					]
					]
			];
	}
	

	TSharedRef<SButton> InfoPanelButton = SNew(SButton)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.ContentPadding(0)
		.OnClicked_Lambda(InfoButton)
		.ButtonStyle(PromoteInfoButtonStyle.Get())
		;
	TSharedRef<SBox> InfoPanelButtonBox = SNew(SBox).HAlign(HAlign_Left).VAlign(VAlign_Center).HeightOverride(55).WidthOverride(20).ToolTipText(PromoteInfoButtonToolTips)[InfoPanelButton];

	builder.BeginSection("MET");
	builder.AddWidget(PromoteButtonBox);
	builder.AddWidget(PromoteInfoPanel);
	builder.AddWidget(InfoPanelButtonBox);
	builder.EndSection();
	
	PromoteInfoPanelPtr = PromoteInfoPanel;
}

FReply FMaterialEditorToolsCore::OnButtonClick_PromoteAllPinsToParameters()
{

	//尝试拿当前激活的编辑器
	
	UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
	TArray<UObject*> EditedAssets = AssetEditorSubsystem->GetAllEditedAssets();
	TArray<FMaterialEditor*> OpenedMaterialEditors;
	
	for (UObject* EditedAsset : EditedAssets)
	{
		IAssetEditorInstance* EditorInstance = AssetEditorSubsystem->FindEditorForAsset(EditedAsset, false);
		if (EditorInstance)
		{
			FMaterialEditor* MaterialEditorInstance = static_cast<FMaterialEditor*>(EditorInstance); 
			if (MaterialEditorInstance)
			{
				OpenedMaterialEditors.Add(MaterialEditorInstance);
			}
		}
		
	}
	
	FMaterialEditor* CurrentMaterialEditor = nullptr;
	
	for (FMaterialEditor* MaterialEditorInstance : OpenedMaterialEditors)
	{
		TSharedPtr<SWidget>FatherWidget = MaterialEditorInstance->GetToolkitHost()->GetParentWidget();
		if (FatherWidget.IsValid())
		{
			while (FatherWidget->IsParentValid())
			{
				FatherWidget = FatherWidget->GetParentWidget();
			}
			TSharedPtr<SWindow> FatherWindow = StaticCastSharedPtr<SWindow>(FatherWidget);
			if (FatherWindow)
			{
				if (FatherWindow->IsActive())
				{
					CurrentMaterialEditor = MaterialEditorInstance;
					break;
				}
			}
		}
	}
	
	//确定编辑器可用开始生成节点操作
	if (CurrentMaterialEditor != nullptr)
	{

		TArray<UObject*>CurrentSelectedNodes = CurrentMaterialEditor->GetSelectedNodes().Array();
		int SortBase = 0;
		for (UObject* Node : CurrentSelectedNodes)
		{
			UMaterialGraphNode_Base* GraphNode = Cast<UMaterialGraphNode_Base>(Node);
			if (GraphNode)
			{

				//准备工作
				const FScopedTransaction Transaction(FText::FromString("PromoteAllPinsToParameters"));
				UEdGraph* GraphObj = GraphNode->GetGraph();
				TArray<UEdGraphPin*> AllPins;
				GraphNode->GetInputPins(AllPins);
				GraphObj->Modify();
				FVector2D NewNodePos;
				FVector2D AdditionalNodePos;
				NewNodePos.X = GraphNode->NodePosX - 250;
				NewNodePos.Y = GraphNode->NodePosY;
				AdditionalNodePos.X = GraphNode->NodePosX - 120;
				AdditionalNodePos.Y = GraphNode->NodePosY;
				
				//拿pin列表进行操作
				for (int i = 0; i < AllPins.Num(); ++i)
				{
					
					UEdGraphPin* Pin = AllPins[i];
					
					//检测是否有连接,当前有复制后识别不准的bug
					
					if (IgnoreHasConnectionCheck&&Pin->HasAnyConnections())
					{
						continue;
					}
					
					EPinType PinType;
					UClass* PinClass = FMaterialEditorToolsHelper::GetPinClass(Pin,PinType);
					if (PinClass != nullptr)
					{
						//根据类型添加节点
						FMaterialGraphSchemaAction_NewNode Action;
						FMaterialGraphSchemaAction_NewNode AdditionalAction;
						UMaterialGraphNode* NewMaterialNode = nullptr;
						UMaterialGraphNode* AdditionalMaterialNode = nullptr;
						switch (PinType)
						{
						case boolean:
						{
							Action.MaterialExpressionClass = PinClass;
							NewMaterialNode = Cast<UMaterialGraphNode>(Action.PerformAction(GraphObj, Pin, NewNodePos));
							NewNodePos.Y += 80;
							AdditionalNodePos.Y += 80;
							break;
						}
						case float1:
						{
							Action.MaterialExpressionClass = PinClass;
							NewMaterialNode = Cast<UMaterialGraphNode>(Action.PerformAction(GraphObj, Pin, NewNodePos));
							NewNodePos.Y += 80;
							AdditionalNodePos.Y += 80;
							break;
						}
						case float2:
						{
							AdditionalAction.MaterialExpressionClass = UMaterialExpressionComponentMask::StaticClass();
							AdditionalMaterialNode = Cast<UMaterialGraphNode>(AdditionalAction.PerformAction(GraphObj, Pin, AdditionalNodePos));
							Action.MaterialExpressionClass = PinClass;
							NewMaterialNode = Cast<UMaterialGraphNode>(Action.PerformAction(GraphObj, AdditionalMaterialNode->GetPinAt(0), NewNodePos));
							NewNodePos.Y += 180;
							AdditionalNodePos.Y += 180;
							break;
						}
						case float3:
						{
							Action.MaterialExpressionClass = PinClass;
							NewMaterialNode = Cast<UMaterialGraphNode>(Action.PerformAction(GraphObj, Pin, NewNodePos));
							NewNodePos.Y += 180;
							AdditionalNodePos.Y += 180;
							break;
						}
						case float4:
						{
							AdditionalAction.MaterialExpressionClass = UMaterialExpressionAppendVector::StaticClass();
							AdditionalMaterialNode = Cast<UMaterialGraphNode>(AdditionalAction.PerformAction(GraphObj, Pin, AdditionalNodePos));
							Action.MaterialExpressionClass = PinClass;
							NewMaterialNode = Cast<UMaterialGraphNode>(Action.PerformAction(GraphObj, AdditionalMaterialNode->GetPinAt(0), NewNodePos));
							NewMaterialNode->GetSchema()->TryCreateConnection(NewMaterialNode->GetPinAt(4), AdditionalMaterialNode->GetPinAt(1));
							NewNodePos.Y += 180;
							AdditionalNodePos.Y += 180;
							break;
						}
						case t2d:
						{
							Action.MaterialExpressionClass = PinClass;
							NewMaterialNode = Cast<UMaterialGraphNode>(Action.PerformAction(GraphObj, Pin, NewNodePos));
							NewNodePos.Y += 180;
							AdditionalNodePos.Y += 180;
							break;
						}
						}

						//用于后面的设置
						UMaterialExpressionParameter* ParameterExpression = Cast<UMaterialExpressionParameter>(NewMaterialNode->MaterialExpression);
						UMaterialExpressionTextureObjectParameter* T2DParameterExpression = Cast<UMaterialExpressionTextureObjectParameter>(NewMaterialNode->MaterialExpression);
						UMaterialExpressionScalarParameter* ScalarParameterExpression = Cast<UMaterialExpressionScalarParameter>(NewMaterialNode->MaterialExpression);
						UMaterialExpressionVectorParameter* VectorParameterExpression = Cast<UMaterialExpressionVectorParameter>(NewMaterialNode->MaterialExpression);

						//设置名字分组和排序
						if (NewMaterialNode != nullptr)
						{
							//名字
							if (NewMaterialNode->MaterialExpression->HasAParameterName())
							{
								FString PinName = Pin->PinName.ToString();
								FString LName, RName;
								
								if(PinName.Split("(", &LName, &RName, ESearchCase::IgnoreCase, ESearchDir::FromEnd))
								{
									if (!RemoveNameText.IsEmpty())
									{
										LName = LName.Replace(*RemoveNameText.ToString(), TEXT(""), ESearchCase::IgnoreCase);
									}
									NewMaterialNode->MaterialExpression->SetParameterName(*(PrefixNameText.ToString() + LName));
								}
								else
								{
									if (!RemoveNameText.IsEmpty())
									{
										PinName = PinName.Replace(*RemoveNameText.ToString(), TEXT(""), ESearchCase::IgnoreCase);
									}
									NewMaterialNode->MaterialExpression->SetParameterName(*(PrefixNameText.ToString() + PinName));
								};

							}
							//分组
							if (!GroupNameText.IsEmpty())
							{
								if (ParameterExpression)
								{
									ParameterExpression->Group = *GroupNameText.ToString();
								}
								else if (T2DParameterExpression)
								{
									T2DParameterExpression->Group = *GroupNameText.ToString();
								}
							}
							//排序
							if (ParameterExpression)
							{
								ParameterExpression->SortPriority = GraphNode->GetPinIndex(Pin) + SortBase;
							}
							else if (T2DParameterExpression)
							{
								T2DParameterExpression->SortPriority = GraphNode->GetPinIndex(Pin) + SortBase;
							}
						}
						
						//尝试设置默认值，虽然是个憨憨做法但是能用.
						FString HoverTextOut = "";
						float ScalarValue = 0;
						FVector4 VectorValue = FVector4(0, 0, 0, 1);
						
						GraphNode->GetPinHoverText(*Pin, HoverTextOut);

						//todo:Regex?
						if (HoverTextOut.RemoveFromStart("DefaultValue = ", ESearchCase::CaseSensitive))
						{
							if (HoverTextOut.RemoveFromStart("Custom expressions", ESearchCase::CaseSensitive))
							{
								HoverTextOut = "Custom expressions";
							}
							else if (HoverTextOut.RemoveFromStart("(", ESearchCase::CaseSensitive))
							{
								FString LName, RName;
								if (HoverTextOut.Split(")", &LName, &RName, ESearchCase::CaseSensitive, ESearchDir::FromStart))
								{
									HoverTextOut = LName;

								}
								else
								{
									HoverTextOut = "";//没找到），格式不匹配
								}
							}
							else
							{
								HoverTextOut = "";//等号空格后面的开头文字不符合预期
							}
						}
						else
						{
							HoverTextOut = "";//没找到开头预期文字
						}
						
						if (!HoverTextOut.IsEmpty() && HoverTextOut != "Custom expressions")
						{
							switch (PinType)
							{
							case float1:
								if (HoverTextOut.IsNumeric())
								{
									ScalarValue = FCString::Atof(*HoverTextOut);
								}
								ScalarParameterExpression->DefaultValue = ScalarValue;
								break;
							case float2:
							case float3:
							case float4:
								FString LName, RName;
								int loopcount = 0;
								while (HoverTextOut.Split(", ", &LName, &RName,ESearchCase::CaseSensitive))
								{
									if (LName.IsNumeric())
									{
										VectorValue[loopcount] = FCString::Atof(*LName);
									}
									HoverTextOut = RName;
									loopcount++;
								}
								if (RName.IsNumeric())
								{
									VectorValue[loopcount] = FCString::Atof(*RName);
								}

								VectorParameterExpression->DefaultValue = (FLinearColor)VectorValue;
								break;
							}
						}
						
						if (IgnoreCustomDefaultValueCheck && HoverTextOut == "Custom expressions" && PinType != boolean && PinType != t2d)
						{
							if (NewMaterialNode != nullptr)
							{
								NewMaterialNode->DestroyNode();
								NewMaterialNode = nullptr;
							}
							if (AdditionalMaterialNode != nullptr)
							{
								AdditionalMaterialNode->DestroyNode();
								AdditionalMaterialNode = nullptr;
							}
						}
					}

				}
				SortBase += AllPins.Num();
			}
		}
		
	}
	return FReply::Handled();
	
}

