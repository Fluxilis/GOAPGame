// Fill out your copyright notice in the Description page of Project Settings.

#include "ConstructionComponent.h"

#include "UnrealNetwork.h"


//TODO REMOVE

//print to screen macro
#define printCritical(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 600.0, FColor::Red,text)


FConstructionRecipe::FConstructionRecipe()
{

}


// Sets default values for this component's properties
UConstructionComponent::UConstructionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UConstructionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

UConstructionComponent::UConstructionComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bReplicates = true;
}


// Called every frame
void UConstructionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


//called by builders - progress on the construction. Should query CanBuild() before every hit!
//returns whether progress was made (hit was successful)
bool UConstructionComponent::Hit()
{
	if (availableHits > 0)
	{
		currentHitsDone++;
		availableHits--;

		//update var for UI to read
		progressPercent = (float)currentHitsDone / (float)constructionRecipe.builderHits;
		
		//play sound / update visual etc
		DoHitFeedback();

		return true;
	}
	else
	{
		//try to use up an item to continue
		if (UseItem())
		{
			//if an item could be used up, try to hit again
			if (Hit())
			{
				//successfully hit this time, hit done was already counted.
				return true;
			}
			else
			{
				printCritical("Used Item But cannot hit still!");
				return false;
			}
		}
		else
		{
			//no item to use up could be found, so no hit can take place.
			return false;
		}

	}

	//Shouldn't be able to fall through to here but in case...
	printCritical("Fell through in Hit() !!");
	return false;
}

//whether a builder can build here. If returned false, should try UseItem();
//Not foolproof...
bool UConstructionComponent::CanBuild()
{
	if (availableHits > 0)
	{
		return true;
	}

	//check if any items from the constructionrecipe are available in a stockpile
	//Not foolproof - if eg there was a third lumber although the recipe only needs two, that third lumber would be used...
	for(int i = 0; i < constructionRecipe.materials.Num(); i++)
	{
		FItemAmount itemAmt = constructionRecipe.materials[i];
		if (myLot->HasItemStored(itemAmt.itemType, 1))
		{
			return true;
		}
	}

	//if not returned true yet, cannot build.
	return false;
}

//uses an item from one of the stockpiles, and increase availableHits.
//returns false if no item was found to use
bool UConstructionComponent::UseItem()
{
	//check if any items from the constructionrecipe are available in a stockpile
	//Not foolproof - if eg there was a third lumber although the recipe only needs two, that third lumber would be used...
	for (int i = 0; i < constructionRecipe.materials.Num(); i++)
	{
		FItemAmount itemAmt = constructionRecipe.materials[i];
		if (myLot->HasItemStored(itemAmt.itemType, 1))
		{
			//found a suitable item

			//remove item from stockpile
			myLot->DeleteItem(itemAmt.itemType, 1);

			//also reduce stockpilesize (because no more items are needed than the inital stockpile size)
			myLot->ReduceStockpileSize(itemAmt.itemType, 1);

			itemsUsedUp++;

			//add to availablehits
			float addHits = ((1.0f / (float)GetTotalItemsNeeded()) * (float)constructionRecipe.builderHits);
			int addAvailableHits = (int)addHits;

			//deal with left-over hits... (because dealing with ints and division...)
			//if all items have been usedip, the nre availablehits must be all hits remaining to complete the construction.
			if (itemsUsedUp >= GetTotalItemsNeeded())
			{
				addAvailableHits = constructionRecipe.builderHits - (availableHits + currentHitsDone);
			}

			availableHits += addAvailableHits;

			return true;
		}
	}

	//if not returned true yet, didn't have item...
	return false;
}

//checks constructionRecipe and counts total amount of items needed
int UConstructionComponent::GetTotalItemsNeeded()
{
	int totalItems = 0;

	for (int i = 0; i < constructionRecipe.materials.Num(); i++)
	{
		FItemAmount itemAmt = constructionRecipe.materials[i];
		totalItems += itemAmt.amount;
	}

	return totalItems;
}