// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Globals.h"
#include "Lot.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CraftingComponent.generated.h"


//Most buildings use a recipe to turn an item into another (eg 1 log -> 2 planks)
USTRUCT(BlueprintType)
struct FCraftingRecipe
{
	GENERATED_BODY()
public:
	FCraftingRecipe();

	//"ingredients", what items are needed to start this recipe.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemAmount> educts;

	//what you get from finishing this recipe
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemAmount> products;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float craftDuration; //how many seconds this recipe needs to complete
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BURGHERS_API UCraftingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCraftingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Reference to the lot (usually a building) this component is attached to. Set in Blueprint Defaults!!
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ALot* myLot;

	//currently producing something
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool working;

	//what recipe this building is currently using
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCraftingRecipe currentRecipe;

	//how many recipies this building has completed in their life.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float lifeTimeRecipes;

	//most buildings need to be staffed by a subject
	//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	Subject* staff; //reference to the subject that works in this building

	//whether this building has a subject assigned to it
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isStaffed; //I could use staff, if it is empty building is not staffed, but with this bool it is more obvious.



	///Functions
	//Economy
	

	//Check if the recipe can be started
	UFUNCTION(BlueprintCallable)
	bool CanStartRecipe(FCraftingRecipe recipe);


	//attempts starting production using recipe.
	UFUNCTION(BlueprintCallable)
	void StartRecipe(FCraftingRecipe recipe);

	//adds products to stockpile and resets building
	UFUNCTION(BlueprintCallable)
	void RecipeDone();


	//	void AssignStaff(Subject* subject); //Assign a subject as staff to this building.

	
};
