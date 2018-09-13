// Fill out your copyright notice in the Description page of Project Settings.

#include "CraftingComponent.h"
#include "TimerManager.h"


FRecipe::FRecipe()
{

}


// Sets default values for this component's properties
UCraftingComponent::UCraftingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCraftingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCraftingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}





//returns whether the building has enough ressources for this recipe.
bool UCraftingComponent::CanStartRecipe(FRecipe recipe)
{
	//needs someone working to craft.
	if (!isStaffed)
	{
		return false;
	}

	for (int i = 0; i < recipe.educts.Num(); i++)
	{
		FStockpile* pile = myLot->GetStockpile(recipe.educts[i].itemType);

		if (pile == nullptr || !pile)
		{
			return false;
		}

		//check if there are enough items of this type in the stockpile of this building.
		if (pile->currentStored >= recipe.educts[i].amount)
		{
			//has enough of this item
		}
		else
		{
			//not enough of this item
			return false;
		}

	}
	//if not returned false yet, has enough of all items.

	//now check if there is enough space for products
	for (int i = 0; i < recipe.products.Num(); i++)
	{
		FStockpile* pile = myLot->GetStockpile(recipe.products[i].itemType);

		if (pile == nullptr || !pile)
		{
			return false;
		}
		//check if there are enough items of this type in the stockpile of this building.
		if ((pile->currentStored + recipe.products[i].amount) <= pile->maxStorage)
		{
			//has space for this item
		}
		else
		{
			//not enough space for this item
			return false;
		}

	}

	//if not returned false yet, has enough items / space.
	return true;
}

//tries to start the recipe. 
//if not enough items in stockpile, does not start.
void UCraftingComponent::StartRecipe(FRecipe recipe)
{
	if (!CanStartRecipe(recipe))
	{
		//cannot start recipe
		return;
	}

	working = true;
	currentRecipe = recipe;

	//remove educts from stockpile
	for (int i = 0; i < recipe.educts.Num(); i++)
	{
		myLot->RemoveItem(recipe.educts[i].itemType, recipe.educts[i].amount);
		myLot->RemovePredictedItem(recipe.educts[i].itemType, recipe.educts[i].amount);
	}
}

void UCraftingComponent::RecipeDone()
{
	//add products to stockpile
	for (int i = 0; i < currentRecipe.products.Num(); i++)
	{
		myLot->AddItem(currentRecipe.products[i].itemType, currentRecipe.products[i].amount);
		myLot->AddPredictedItem(currentRecipe.products[i].itemType, currentRecipe.products[i].amount);
	}

	lifeTimeRecipes++;

	working = false;
}