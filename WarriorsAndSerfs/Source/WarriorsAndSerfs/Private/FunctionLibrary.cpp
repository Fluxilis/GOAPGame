// Fill out your copyright notice in the Description page of Project Settings.

#include "FunctionLibrary.h"

UTexture2D * UFunctionLibrary::GetTextureByName(const FString& Path, const FString& TextureName)
{

	FString AssetPath = "Texture2D'/{Path}/{TextureName}.{TextureName}'";
	AssetPath = AssetPath.Replace(TEXT("{TextureName}"), *TextureName).Replace(TEXT("{Path"), *Path);
	FStringAssetReference AssetRef(AssetPath);

	return Cast<UTexture2D>(AssetRef.TryLoad());
}

UTexture2D * UFunctionLibrary::GetTextureByPath(const FString& Path)
{

	FString AssetPath = Path;
	FStringAssetReference AssetRef(AssetPath);

	return Cast<UTexture2D>(AssetRef.TryLoad());
}

UClass* UFunctionLibrary::GetClassByName(const FString& ClassName)
{
	check(*ClassName);
	UObject* ClassPackage = ANY_PACKAGE;

	if (UClass* Result = FindObject<UClass>(ClassPackage, *ClassName))
	{
		return Result;
	}

	if (UObjectRedirector* RenamedClassRedirector = FindObject<UObjectRedirector>(ClassPackage, *ClassName))
	{
		return CastChecked<UClass>(RenamedClassRedirector->DestinationObject);
	}

	return nullptr;
}

UStaticMesh * UFunctionLibrary::GetMeshByPath(const FString& Path)
{
	FString AssetPath = Path;
	FStringAssetReference AssetRef(AssetPath);

	return Cast<UStaticMesh>(AssetRef.TryLoad());
}