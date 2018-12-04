// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Globals.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lot.generated.h"

//Items will not be taken out of an educt stockpile by the JobSystem.
//Items will not be put into a product stockpile by the JobSystem.
//items can be taken out of and put into a warehouse stockpile by the JobSystem.
UENUM(BlueprintType)
enum class EStockpileType : uint8
{
	Educt, Product, Warehouse
};


//used to keep track of how many of what item are in a building. Used for all buildings, inluding warehouse.
USTRUCT(BlueprintType)
struct FStockpile
{
	GENERATED_BODY()

public:
	FStockpile();
	FStockpile(FName type, EStockpileType pileType, int maxStore);

	//item stockpiled here(eg log, plank, axe)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName itemType;

	//whether it is an educt, product or storage stockpile
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStockpileType stockpileType;

	//how many items can fit this storage (usually around 3-7)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int maxStorage;

	//how many items are stored at the moment.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int currentStored = 0;
	
	//amount of items that here, plus items that are on the way here (in case of educts), minus items that are assigned to a job already (in case of products)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int currentPredicted = 0;
};


//a lot is a base class for buildings, construction sites, fields and some abstract things. 
//(necessary for eg builder / serf destination finding)
UCLASS(abstract)
class WARRIORSANDSERFS_API ALot : public AActor
{
	GENERATED_BODY()

	ALot(const FObjectInitializer& ObjectInitializer);
	
public:	
	// Sets default values for this actor's properties
	ALot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
public:
	//general
//	Player* player; //which player this building belongs to.

	//I want to allow players to "name" their Buildings.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString playerLabel; 

	//what Type of lot this is
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELotType lotType;

	//Building as part of the production chain

	//every lot has different stockpiles (warehouse has one for every item, constructionsite has wood, stone... Field has none though!?)
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	TArray<FStockpile> stockpiles;

	//jobManager in the level. Found in scene when the building is created / loaded.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AJobManager* jobManager;

	//jobs that this lot has created
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UJob*> myJobs;

	//How many Pulljobs per stockpile are allowed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float pullJobLimit = 3;
	
	//How many Pushjobs per stockpile are allowed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float pushJobLimit = 1;

	//combat

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxHealth;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	float currentHealth;

	///Functions

	//Handles Building taking damage.
	UFUNCTION(BlueprintCallable, Category = Health)
	void TakeDamage(float x);

	//Handles Lot Destruction.
	//Implemented in Bluprints
	//May be overridden in every level of Lot (Building, HarvestingBuilding, Lumberjacks'...)
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void DestroyLot();


	//Helper function
	//returns nullptr if the building doesn't have a stockpile for this item.
	//UFUNCTION(BlueprintCallable) //Exposed pointer to structs not allowed in UE...
	FStockpile* GetStockpile(FName item);

	//Returns wether this Lot HAS a stockpile of this type. calls GetStockpile. (GetStockpile cannot be used in BP so...)
	UFUNCTION(BlueprintCallable) 
	bool HasStockpile(FName item);

	//adds item of type item to both predicted and actual amount.
	UFUNCTION(BlueprintCallable)
	void MakeItem(FName item, int amount);

	//removes item of type item from both predicted and actual amount.
	UFUNCTION(BlueprintCallable)
	void DeleteItem(FName item, int amount);

	//adds ONE item of type item to the stockpile
	UFUNCTION(BlueprintCallable)
	void AddItem(FName item, int amount);

	//adds this item to the predicted stockpile
	UFUNCTION(BlueprintCallable)
	void AddPredictedItem(FName item, int amount);

	//removes ONE item of type item from the stockpile
	UFUNCTION(BlueprintCallable)
	void RemoveItem(FName item, int amount);

	//removes this item from the predicted stockpile
	UFUNCTION(BlueprintCallable)
	void RemovePredictedItem(FName item, int amount);

	//Update ui that has already spawned
	//uiToUpdate describes which UI element to update (to save performance)
	UFUNCTION(BlueprintImplementableEvent, Category = UI)
	void UpdateActiveUI(FName uiToUpdate);

	//adds a stockpile from the list.
	UFUNCTION(BlueprintCallable)
	void AddStockpile(FName item, EStockpileType type, int maxStorage);

	//reduces the maxStored value of the item's stockpile
	//used for constructionsites, when this item is used to fuel builderhits.
	UFUNCTION(BlueprintCallable)
	void ReduceStockpileSize(FName item, int amount);

	//removes the stockpile from the list.
	//used for constructionsites, when all materials of a kind have been used up for builderhits.
	UFUNCTION(BlueprintCallable)
	void RemoveStockpile(FName item);


	//For Deliverysystem

	//Checks stockpiles and creates jobs.
	//should be called every 2 seconds or so
	UFUNCTION(BlueprintCallable)
	void MakeJob(AJobManager* jManager);

	//removes the job from myjobs
	UFUNCTION(BlueprintCallable)
	void RemoveJob(UJob* job);

	//adds the job to myjobs
	UFUNCTION(BlueprintCallable)
	void AddJob(UJob* job);

	//increases priority of all jobs in the myjobs list by amount.
	UFUNCTION(BlueprintCallable)
	void IncreaseJobPriorities(float amount);

	//checks if this building has an offered (and not active) push job for an item
	//returns the job if one is found, else returns nullptr.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UJob* HasPushJobForItem(FName item, int amount);

	//checks if this building has an offered (and not active) pull job for an item
	//returns the job if one is found, else returns nullptr.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UJob* HasPullJobForItem(FName item, int amount);

	//checks if this building has an item there (eg in order to be picked up)
	//will only attempt to get item from non-educt stockpile
	//returns true only if precictedStock and currentStock are high enough!
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool ItemAvailable(FName item, int amount);

	//checks if this building has an item there (eg in order to be picked up)
	//however this function will also return true for educt stockpiles! (used eg for education buildings)
	//returns true only if both precictedStock and currentStock are high enough!
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasItemStored(FName item, int amount);

	//checks if this building has enough space for these items 
	//will only attempt to fit item into non-product stockpile
	//returns true only if precictedStock and currentStock are low enough!
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CanFitItem(FName item, int amount);
	
	//like CanFitItem, checks if this building has enough space for these items 
	//however this function will also return true for product stockpiles! (used eg for harvesting buildings)
	//returns true only if precictedStock and currentStock are low enough!
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool SpaceForItem(FName item, int amount);
};


