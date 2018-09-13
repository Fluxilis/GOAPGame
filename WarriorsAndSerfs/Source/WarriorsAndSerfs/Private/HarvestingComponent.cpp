// Fill out your copyright notice in the Description page of Project Settings.

#include "HarvestingComponent.h"


// Sets default values for this component's properties
UHarvestingComponent::UHarvestingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHarvestingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHarvestingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


//returns wether the stockpiles have enough space for one harvest.
bool UHarvestingComponent::SpaceForHarvest()
{
	for(int i = 0; i < harvestingProducts.Num(); i++)
	{
		FItemAmount itemAmt = harvestingProducts[i];

		if (!(myLot->SpaceForItem(itemAmt.itemType, itemAmt.amount)))
		{
			return false;
		}
	}

	//not returned false yet so
	return true;
}

//adds raw mats, ready to be polished (called when completing ReturnFromHarvest Job)
void UHarvestingComponent::AddRawMaterial()
{
	hasRawMaterial = true;
}

//removes raw mats, and adds items ready to be picked up. (called when completing polishing Job)
void UHarvestingComponent::FinishPolishingJob()
{
	hasRawMaterial = false;

	for (int i = 0; i < harvestingProducts.Num(); i++)
	{
		FItemAmount itemAmt = harvestingProducts[i];

		myLot->MakeItem(itemAmt.itemType, itemAmt.amount);
		
	}

	lifeTimePolishes++;
}

