// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Globals.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ELotType : uint8
{
	MissingNo, Debug, //Debug
	Building, Constructionsite, DeadDrop //Different types of lots (may have additional specification, eg building type)
};


UENUM(BlueprintType)
enum class EHarvestingJobType : uint8
{
	None, Planting, Returning, Tending, Harvesting, ReturningWithHarvest, Polishing, Resting
};

UENUM(BlueprintType)
enum class ECraftingJobType : uint8
{
	None, Crafting, Resting
};

//a simple struct that contains an item type and an int amount.
//used eg in Recipes
USTRUCT(BlueprintType)
struct FItemAmount
{
	GENERATED_BODY()

public:
	FItemAmount();

	//what item
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName itemType;

	//amount of items
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int amount;
};



UCLASS(Blueprintable)
class WARRIORSANDSERFS_API UGlobals : public UObject
{

	GENERATED_BODY()

public:
	UGlobals();
	~UGlobals();

	//returns the (player-side) name of the Lot
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FString GetLotType(ELotType lot);

	//returns the (player-side) name of the job
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FString GetHarvestingJobType(EHarvestingJobType job);

	//returns the (player-side) name of the job
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FString GetCraftingJobType(ECraftingJobType job);
};






