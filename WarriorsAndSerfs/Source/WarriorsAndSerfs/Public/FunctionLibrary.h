// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

	
	
};
