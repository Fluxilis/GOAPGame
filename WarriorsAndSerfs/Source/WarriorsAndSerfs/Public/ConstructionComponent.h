// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "Lot.h" included in CraftingComponent
#include "Globals.h"
#include "Lot.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ConstructionComponent.generated.h"



//what items are required to build a building
USTRUCT(BlueprintType)
struct FConstructionRecipe
{
	GENERATED_BODY()
public:
	FConstructionRecipe();

	//what items are needed to build.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemAmount> materials;


	//how many "hits" this building needs to complete - each builder "hits" the building ROUGHLY once a second (frequency may change)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int builderHits;
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WARRIORSANDSERFS_API UConstructionComponent : public UActorComponent
{
	GENERATED_BODY()

	UConstructionComponent(const FObjectInitializer& ObjectInitializer);

public:	
	// Sets default values for this component's properties
	UConstructionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



	//Reference to the lot (a constructionsite) this component is attached to. Set in Blueprint Defaults!!
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ALot* myLot;
	
	//what recipe this building is currently using
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FConstructionRecipe constructionRecipe;

	//how many "hits" have been done on this building, a measure of progress in the construction.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int currentHitsDone = 0;

	//DO NOT USE FOR GAME LOGIC
	//updated in Hit() - currentHits / hitsNeeded.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float progressPercent = 0;

	//how many "hits" can be done currently (until building complete or need new item)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int availableHits = 0;

	//how many items were used up during the construction already
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int itemsUsedUp = 0;

	///Functions

	//called by builders - progress on the construction.
	//returns whether progress was made (hit was successful)
	UFUNCTION(BlueprintCallable)
	bool Hit();

	//Called on successful hit, handle in BP (play sound / update visual...)
	UFUNCTION(BlueprintImplementableEvent)
	void DoHitFeedback();

	//whether a builder can build here
	UFUNCTION(BlueprintCallable)
	bool CanBuild();

	//uses an item from one of the stockpiles, and increase availableHits.
	//returns false if no item was found to use
	bool UseItem();

	///Helper Functions

	//checks constructionRecipe and counts total amount of items needed
	int GetTotalItemsNeeded();
};
