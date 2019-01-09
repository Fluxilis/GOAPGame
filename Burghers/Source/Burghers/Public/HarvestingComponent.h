// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Globals.h"
#include "Lot.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HarvestingComponent.generated.h"


//Many buildings harvest from the environment and produce items from it.
USTRUCT(BlueprintType)
struct FHarvestingRecipe
{
	GENERATED_BODY()
public:
	FHarvestingRecipe();

	//what interactable Environment type you need (eg trees for Woodcutting, Coal Ore for coal mining)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName interactEnvType;

	//what you get from finishing this recipe
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FItemAmount> products;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float polishDuration; //how many seconds this recipe needs to complete
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BURGHERS_API UHarvestingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHarvestingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Reference to the lot (usually a building) this component is attached to. Set in Blueprint Defaults!!
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ALot* myLot;
	
	//Whether this building has raw mats ready to polish
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool hasRawMaterial;

	//what you get each time a harvesting job is completed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemAmount> harvestingProducts;

	//how many polishing jobs this building has completed in their life.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float lifeTimePolishes;

	//functions

	//returns wether the stockpiles have enough space for one harvest.
	UFUNCTION(BlueprintCallable)
	bool SpaceForHarvest();

	//adds raw mats, ready to be polished (called when completing ReturnFromHarvest Job)
	UFUNCTION(BlueprintCallable)
	void AddRawMaterial();

	//removes raw mats, and adds items ready to be picked up. (called when completing polishing Job)
	UFUNCTION(BlueprintCallable)
	void FinishPolishingJob();

};
