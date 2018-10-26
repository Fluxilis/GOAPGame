// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Lot.h"
#include "Subject.h"
#include "ConstructionComponent.h"
#include "HarvestingComponent.h"
#include "CraftingComponent.h"

#include "Engine/DataTable.h"

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "JobManager.generated.h"

/**
 * 
 */

 //Pull Jobs want to get an item from anywhere to the Destination
 //Push Jobs want to send an item from Source to anywhere
UENUM(BlueprintType)
enum class EJobType : uint8
{
	Pull, Push
};

//Until a serf is assigned, the Job is offered. 
//When a Serf is assigned, the Status is ToSource (Serf en-route to pick up item)
//when the Item was picked up, the Status is ToDestination (Serf en-route to drop off item)
//when the item was dropped off, the Job is completed successfully, and status is finished. CancelJob() should be called now, through the Jobmanager.
UENUM(BlueprintType)
enum class EJobStatus : uint8
{
	Offered, ToSource, ToDestination, Finished
};


//simple struct to return a Lot and a Job (intended as this Lot HAS this job)
USTRUCT(BlueprintType)
struct FLotWithJob
{
	GENERATED_BODY()

public:
	FLotWithJob();
	FLotWithJob(ALot* theLot, UJob* theJob);

	//lot
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ALot* lot;

	//job
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UJob* job;
};

//contains an itemype and a float priority. in Jobmanager there should be a list with one FItemPriority for every EItem. no other FItemPriorities should exist (permanently)
USTRUCT(BlueprintType)
struct FItemPriority
{
	GENERATED_BODY()

public:
	FItemPriority();
	FItemPriority(EItem item, float prio);

	//what item
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItem itemType;

	//priority
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float priority;
};

//struct for item datatables
USTRUCT(BlueprintType)
struct FDTS_Item : public FTableRowBase
{
	GENERATED_BODY()

public:
	FDTS_Item();

	//what the player sees as item name
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText DisplayName;

	//where in lists (eg warehouse stockpiles) this item should be displayed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int OrderInLists;

	//help text the player can read (eg tooltip, info button)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText HelpText;

	//name of the icon asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Icon;

	//name of the 3dMesh asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Mesh;

	//transport priority of the item
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ItemPriority;
};

//struct for building datatables
USTRUCT(BlueprintType)
struct FDTS_Building: public FTableRowBase
{
	GENERATED_BODY()

public:
	FDTS_Building();

	//what the player sees as building name
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText DisplayName;

	//where in lists (eg building browser button) this building should be displayed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int OrderInLists;

	//max health of this building
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxHealth;

	//what kind of subject works in this building
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ESubjectType WhoWorksHere;

	//How many Pulljobs this building may create per stockpile
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int PullJobLimit;

	//How many Pushjobs this building may create per stockpile
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int PushJobLimit;

	//Items and hits needed to construct the building
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FConstructionRecipe ConstructionRecipe;

	//The Stockpiles this building has
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FStockpile> Stockpiles;
	
	//radius of harvesting area (if it has one)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float HarvestAreaRadius;

	//how far harvesting area can be from the door (if it has one)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float HarvestAreaMaxDist;

	//The possible Harvest Recipes for this building (if it is a harvesting Building)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FHarvestingRecipe> HarvestRecipeList;

	//The possible Crafting Recipes for this building (if it is a crafting Building)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FCraftingRecipe> CraftingRecipeList;

	//help text the player can read (eg tooltip, info button)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText HelpText;

	//name of the icon asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString BlueprintClass;

	//name of the icon asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Icon;

	//name of the 3dMesh asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Mesh;
};


//Jobs are transport tasks that need serfs to execute.
//Jobs can be roughly divided in Pull and Push Jobs, pull Jobs want to receive an item, while pus Jobs want to get rid of an item in stockpile.
//When a Job is accepted, the source / destination is filled, the item in the source is removed from predictedStock, and added to the Destination's predicted Stock, and a Serf is assigned.
UCLASS(Blueprintable)
class UJob : public UObject
{
	GENERATED_BODY()
public:
	//You need to call InitJob with the variables after this!
	UJob();

	//sets variables for Job, >type< influences wether >employee< fills Source or Destination.
	UFUNCTION(BlueprintCallable)
	void InitJob(EJobType type, ALot* employee, FName it, float prio);

	//status of the job (unassigned, picking up item or delivering item)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EJobStatus jobStatus;

	//Whether this is a push or pull job.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EJobType jobType;

	//Where the item in the job comes from
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ALot* source;

	//Where the item in the job goes
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ALot* destination;

	//What Item the job is about Eg the Log requested by the sawmill, or the axe being pushed by the carpenter.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName item;
	
	//How important this Job is (initially determined and set in MakeJob() then increased over time.)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float priority;

	///functions

	//cancels job. Frees items from assignment (Does different things depending on Job status)
	//also used when finished a job properly!
	UFUNCTION(BlueprintCallable)
	void CancelJob();
};



UCLASS(Blueprintable)
class WARRIORSANDSERFS_API AJobManager : public AInfo
{
	GENERATED_BODY()
	
public:
	AJobManager();

	//Job Managers handle jobs for one player each.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName owningPlayerName;

	//This list contains all jobs that are not yet accepted (ie no serf / item assigned)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UJob*> jobOffers;

	//This list contains all jobs that have been accepted and are being executed. (has serf & item assigned)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UJob*> activeJobs;

	//This list contains all lots of the local player. All lots are considered for jobs.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ALot*> lots;

	//TODO improve - eg make player-editable (how?)
	//This list contains the priority modifier for each item (affects jobPriority in MakeJob)
	//this list should contain exactly one object for each EItem.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemPriority> itemPriorities;

	//This datatable is loaded at runtime and contains all items and info about them, such as their transport priority (which is used here)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDataTable* dataTable;

	///functions

	//adds the lot to the lots array
	UFUNCTION(BlueprintCallable)
	void AddLot(ALot* lot);

	//removes lot from the lots array
	UFUNCTION(BlueprintCallable)
	void RemoveLot(ALot* lot);


	//removes jobs for this lot from the offered jobs list
	UFUNCTION(BlueprintCallable)
	void RemoveJobs(ALot* lot);

	//sets the initial item priorities
	UFUNCTION(BlueprintCallable)
	void InitItemPriorities();

	///helper functions

	//returns the priority value of the item.
	UFUNCTION(BlueprintCallable)
	float GetItemPriority(EItem item);

	//sets the itempriority for an item
	UFUNCTION(BlueprintCallable)
	void SetItemPriority(EItem item, float newPrio);

	//returns a pullJob that requires the item.
	//source is the Lot the request is coming from
	//if none found, try FindFreeSpace()
	UJob* FindPullJobForItem(ALot* source, FName item);
	
	//returns a pushJob that this Lot made; nullptr if none is found.
	UJob* FindPushJobForLot(ALot* source);

	//you should try FindPullJobForItem() before this!
	//returns a Lot that can fit the item (educt or warehouse).
	//source is the Lot the request is coming from
	ALot* FindFreeSpace(ALot* source, FName item);

	//returns a Lot that has this item to offer (product or warehouse).
	//destination is the Lot the request is coming from
	ALot* FindItem(ALot* destination, FName item);

	//returns a lot that has an offered (but not active) push job for this item, meaning it has this item available...
	//destination is the Lot this request is coming from.
	//returns nullptr if no such Lot is found.
	FLotWithJob* FindLotWithPushJob(ALot* destination, FName item);
	
	//Accepts the Job:
	//Adds / Removes predicted item to target
	//moves job from offered to active jobs list
	//calls Job::AssignJob() (updates jobstatus and the missing lot in Job)
	//returns the same Job
	UJob* AcceptJob(ALot* target, UJob* job);

	/// \helper functions

	//called from a lot making a jobOffer.
	UFUNCTION(BlueprintCallable)
	UJob* MakeJob(EJobType jobType, ALot* employee, FName item, float prio);
	
	//called from a serf looking for a job. 
	//Checks the jobOffers List and searches for a suitable job.
	//iff successful, transfers Job to activeJobs.
	//returns reference to the Job, for the serf to use.
	UFUNCTION(BlueprintCallable)
	UJob* FindJob();

	//finds job in list and calls Job::CancelJob() on it. Then removes job from the list.
	//also used when finished a job properly!
	UFUNCTION(BlueprintCallable)
	void CancelJob(UJob* job);



	///More Complex job finding...

	/*
	*tries to find a suitable job that involves this lot.

	priorities in order:
	fulfill pulljob for a product item in this building
	fulfill existing pushjob from this building

	*called by serfs after they drop an item in lot
	*returns a suitable job that has already been set to accepted.
	*/
	UFUNCTION(BlueprintCallable)
	UJob* FindJobByLot(ALot* lot);


	/*
	*tries to find a suitable job that involves this Item.

	priorities in order:
	fulfill pulljob for this item
	find suitable educt stockpile that didn't create pulljob yet
	dump in a warehouse.

	*called by serfs after their job has been found invalid while carrying an item (eg if their destination building was destroyed)
	*returns a suitable job that has already been set to accepted.
	*/
	UFUNCTION(BlueprintCallable)
	UJob* FindJobForItem(FName item);
};
