// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/Engine/Classes/Engine/StaticMesh.h"

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORSANDSERFS_API UFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintCallable, Category = "Texture By Name")
		static UTexture2D * GetTextureByName(const FString& Path, const FString& TextureName);
	UFUNCTION(BlueprintCallable, Category = "Texture By Name")
		static UTexture2D * GetTextureByPath(const FString& Path);
	UFUNCTION(BlueprintCallable, Category = "Class By Name")
		static UClass* GetClassByName(const FString& ClassName);
	UFUNCTION(BlueprintCallable, Category = "Mesh By Name")
		static UStaticMesh * GetMeshByPath(const FString& Path);
	
};
