// _(:§Ù¡¹¡Ï)_._(:§Ù¡¹¡Ï)_._(:§Ù¡¹¡Ï)_._(:§Ù¡¹¡Ï)_

#pragma once

#include <string>

#include "CoreMinimal.h"
#include "Editor.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "ToolMenus.h"

#include "MaterialEditor/Public/MaterialEditorModule.h"
#include "MaterialEditor/Private/MaterialEditor.h"
#include "MaterialGraph/MaterialGraphNode_Root.h"
#include "MaterialGraph/MaterialGraphNode.h"
#include "MaterialGraph/MaterialGraphSchema.h"

#include "Materials/MaterialExpression.h"
#include "Materials/MaterialExpressionStaticBoolParameter.h"
#include "Materials/MaterialExpressionVectorParameter.h"
#include "Materials/MaterialExpressionScalarParameter.h"
#include "Materials/MaterialExpressionTextureObjectParameter.h"
#include "Materials/MaterialExpressionAppendVector.h"
#include "Materials/MaterialExpressionComponentMask.h"


#include "Interfaces/IPluginManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogMaterialEditorTools, Log, All);

UENUM()
enum EPinType
{
	none,
	float1,
	float2,
	float3,
	float4,
	boolean,
	t2d
};


class FMaterialEditorToolsHelper
{
public:
	static UClass* GetPinClass(const UEdGraphPin* TargetPin,EPinType& PinType)
	{
		UMaterialGraphNode_Root* RootPinNode = Cast<UMaterialGraphNode_Root>(TargetPin->GetOwningNode());
		UMaterialGraphNode* OtherPinNode = Cast<UMaterialGraphNode>(TargetPin->GetOwningNode());

		if (RootPinNode != nullptr)
		{
			EMaterialProperty propertyId = (EMaterialProperty)FCString::Atoi(*TargetPin->PinType.PinSubCategory.ToString());

			switch (propertyId)
			{
			case MP_Opacity:
			case MP_Metallic:
			case MP_Specular:
			case MP_Roughness:
			case MP_Anisotropy:
			case MP_TessellationMultiplier:
			case MP_CustomData0:
			case MP_CustomData1:
			case MP_AmbientOcclusion:
			case MP_Refraction:
			case MP_PixelDepthOffset:
			case MP_ShadingModel:
			case MP_OpacityMask:
				PinType = float1;
				return UMaterialExpressionScalarParameter::StaticClass();

			case MP_WorldPositionOffset:
			case MP_WorldDisplacement:
			case MP_EmissiveColor:
			case MP_BaseColor:
			case MP_SubsurfaceColor:
			case MP_SpecularColor:
			case MP_Normal:
			case MP_Tangent:
				PinType = float3;
				return UMaterialExpressionVectorParameter::StaticClass();
			}
		}
		else if (OtherPinNode)
		{
			const TArray<FExpressionInput*> ExpressionInputs = OtherPinNode->MaterialExpression->GetInputs();
			FName TargetPinName = OtherPinNode->GetShortenPinName(TargetPin->PinName);

			for (int32 Index = 0; Index < ExpressionInputs.Num(); ++Index)
			{
				FExpressionInput* Input = ExpressionInputs[Index];
				FName InputName = OtherPinNode->MaterialExpression->GetInputName(Index);
				InputName = OtherPinNode->GetShortenPinName(InputName);

				if (InputName == TargetPinName)
				{
					switch (OtherPinNode->MaterialExpression->GetInputType(Index))
					{
					case MCT_Float1:
					case MCT_Float: PinType = float1; return UMaterialExpressionScalarParameter::StaticClass();

					case MCT_Float2: PinType = float2; return UMaterialExpressionVectorParameter::StaticClass();
					case MCT_Float3: PinType = float3; return UMaterialExpressionVectorParameter::StaticClass();
					case MCT_Float4: PinType = float4; return UMaterialExpressionVectorParameter::StaticClass();

					case MCT_StaticBool: PinType = boolean; return UMaterialExpressionStaticBoolParameter::StaticClass();

					case MCT_Texture2D:
					case MCT_TextureCube:
					case MCT_VolumeTexture:
					case MCT_Texture: PinType = t2d; return UMaterialExpressionTextureObjectParameter::StaticClass();
					}

					break;
				}
			}
		}

		return nullptr;
	}
	static bool CopyContentFolderFromResource()
	{
		IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();
		FString ResourcesPath = IPluginManager::Get().FindPlugin("MaterialEditorTools")->GetBaseDir() / TEXT("Resources/ContentFolder/");
		FString ContentPath = IPluginManager::Get().FindPlugin("MaterialEditorTools")->GetBaseDir()+TEXT("/");
		return FileManager.CopyDirectoryTree(*ContentPath,*ResourcesPath,true);
	};
};
