// Fill out your copyright notice in the Description page of Project Settings.

#include "Lot.h"
#include "JobManager.h"

#include "EngineGlobals.h"

#include "UnrealNetwork.h"


//TODO REMOVE

//print to screen macro
#define printCritical(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 600.0, FColor::Red,text)



// Sets default values
ALot::ALot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

ALot::ALot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bReplicates = true;
	bNetLoadOnClient = true;
}

void ALot::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALot, currentHealth);
	DOREPLIFETIME(ALot, stockpiles);
}

// Called when the game starts or when spawned
void ALot::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//Handles Building taking damage.
void ALot::TakeDamage(float x)
{
	currentHealth -= x;

	if (currentHealth > maxHealth)
	{
		currentHealth = maxHealth;
	}

	if (currentHealth <= 0)
	{
		DestroyLot();
	}
	else
	{
		UpdateActiveUI();
	}
}


FStockpile* ALot::GetStockpile(FName item)
{
	//find the right stockpile
	int num = stockpiles.Num();
	for (int i = 0; i < num; i++)
	{
		if (stockpiles[i].itemType == item)
		{
			//found stockpile.
			return &(stockpiles[i]);
		}
	}

	return nullptr;
}

//Returns wether this Lot HAS a stockpile of this type. calls GetStockpile. (GetStockpile cannot be used in BP so...)
bool ALot::HasStockpile(FName item)
{
	if (GetStockpile(item) != nullptr)
	{
		return true;
	}
	//else
	return false;
}


//adds item of type item to both predicted and actual amount.
void ALot::MakeItem(FName item, int amount = 1)
{
	AddItem(item, amount);
	AddPredictedItem(item, amount);
}

//removes item of type item from both predicted and actual amount.
void ALot::DeleteItem(FName item, int amount = 1)
{
	RemoveItem(item, amount);
	RemovePredictedItem(item, amount);
}


//adds amount items to the stockpile. Does not check if there is space!
void ALot::AddItem(FName item, int amount = 1)
{
	FStockpile* pile = GetStockpile(item);
	if (pile == nullptr || !pile)
	{
		//no stockpile of this item type in this building.
		printCritical("ERROR: tried to add item but didn't have stockpile!!");
		return;
	}

	//add one item.
	pile->currentStored += amount;

	//if stockpile would overflow, limit to maxStorage.
	if (pile->currentStored > pile->maxStorage)
	{
		pile->currentStored = pile->maxStorage;

		//currently, I know that this happens with schools.
		if (playerLabel == "School")
		{
			//don't print, I know this happens.
		}
		else
		{
			printCritical("ERROR: added item but currentstored was >maxStorage!!");
			printCritical("NOT FROM A SCHOOL!");
		}
	}


	UpdateActiveUI();
}

//removes amount items from the stockpile. Does not check if there are enough!
void ALot::RemoveItem(FName item, int amount = 1)
{
	FStockpile* pile = GetStockpile(item);
	if (pile == nullptr || !pile)
	{
		//no stockpile of this item type in this building.
		printCritical("ERROR: tried to remove item but didn't have stockpile!!");
		return;
	}

	//add one item.
	pile->currentStored -= amount;

	//if stockpile would go below 0, limit to 0.
	if (pile->currentStored < 0)
	{
		printCritical("ERROR: removed item but currentstored was <0!!");
		pile->currentStored = 0;
	}


	UpdateActiveUI();
}

void ALot::AddStockpile(FName item, EStockpileType type, int maxStorage)
{
	FStockpile* pile = new FStockpile(item, type, maxStorage);
	stockpiles.Add(*pile);

	UpdateActiveUI();
}

void ALot::RemoveStockpile(FName item)
{
	//find the right stockpile
	int num = stockpiles.Num();
	for (int i = 0; i < num; i++)
	{
		if (stockpiles[i].itemType == item)
		{
			//found stockpile. Remove.

			stockpiles.RemoveAt(i);
			return;
		}
	}

	UpdateActiveUI();
}

//DeliverySystem

//Checks stockpiles and creates a (1) job.
//should be called every 2 seconds or so
void ALot::MakeJob(AJobManager* jManager)
{
	if (jManager == nullptr || !jManager || jManager == NULL)
	{
		return;
	}

	if (stockpiles.Num() <= 0)
	{
		return;
	}



	//do for all stockpiles
	for (int i = 0; i < stockpiles.Num(); i++)
	{
		FStockpile* pile = &(stockpiles[i]);
		if (pile == nullptr || !pile)
		{
			continue;
		}

		//educt stockpile; if not full, make a pull job.
		if (pile->stockpileType == EStockpileType::Educt)
		{
			if (pile->currentPredicted < pile->maxStorage)
			{
				//educt stockpile not full. create a job. find priority first though.

				//limit amount of jobs for this stockpile
				if (pile->currentStored + pullJobLimit <= pile->currentPredicted)
				{
					//limit reached, don't make pulljob for this stockpile
					continue;
				}

				int predicted = pile->currentPredicted;
				int max = pile->maxStorage;


				// priority

				float finalPriority = 0;
				
				//fullness
				float fullnessPriority = 0;
				if (predicted == 0)
				{
					fullnessPriority = 100;
				}
				else if (predicted < max * 0.2)
				{
					//stockpile very low
					fullnessPriority = 90;
				}
				else if (predicted < max * 0.5)
				{
					//stockpile getting low
					fullnessPriority = 75;
				}
				else if (predicted < max * 0.80)
				{
					//stockpile quite full
					fullnessPriority = 55;
				}
				else 
				{
					//stockpile very full
					fullnessPriority = 35;
				}

				float itemPriority = jobManager->GetItemPriority(pile->itemType);
			//	float itemPriority = 0; //TODO FIX ASAP


				finalPriority = fullnessPriority + itemPriority;

				// \priority

				UJob* newJob = jManager->MakeJob(EJobType::Pull, this, pile->itemType, finalPriority);
			}
		}
		//product stockpile; if not empty, make a push job.
		else if (pile->stockpileType == EStockpileType::Product)
		{
			if (pile->currentPredicted > 0)
			{
				//product stockpile not empty. create a job. find priority first though.

				//limit amount of jobs for this stockpile
				if (pile->currentStored - pushJobLimit >= pile->currentPredicted)
				{
					//limit reached, don't make pushjob for this stockpile
					continue;
				}

				int predicted = pile->currentPredicted;
				int max = pile->maxStorage;

				float priority = 50;
				if (predicted == max)
				{
					priority = 60;
				}
				//don't make pushjob unless completely Full!

				UJob* newJob = jManager->MakeJob(EJobType::Push, this, pile->itemType, priority);
			}
		}

		//if it's a warehouse, don't make jobs. ->might be implemented later

	}
}


//removes a job from myJobs
void ALot::RemoveJob(UJob* job)
{
	if (job == nullptr || !job)
	{
		printCritical("ERROR AT 10");
		return;
	}

	myJobs.Remove(job);
}

//adds the job to myjobs
void ALot::AddJob(UJob* job)
{
	myJobs.Add(job);
}

//increases priority of all jobs in the myjobs list by amount.
void ALot::IncreaseJobPriorities(float amount)
{
	for (int i = 0; i < myJobs.Num(); i++)
	{
		myJobs[i]->priority += amount;
	}
}



//checks if this building has an offered (and not active) push job for an item
//returns the job if one is found, else returns nullptr.
UJob* ALot::HasPushJobForItem(FName item, int amount)
{
	//check if any of my jobs are a pushjob for this item.
	for (int i = 0; i < myJobs.Num(); i++)
	{
		if (myJobs[i] == nullptr || !myJobs[i])
		{
			printCritical("ERROR AT 6");
			myJobs.RemoveAt(i);
			return nullptr;
		}
		UJob* job = myJobs[i];

		if (job->item == item
			&& job->jobType == EJobType::Push
			&& job->source == this
			&& job->jobStatus == EJobStatus::Offered)
		{
			return job;
		}
	}

	//if not returned yet, doesn't have a push job.
	return nullptr;
}

//checks if this building has an offered (and not active) pull job for an item
//returns the job if one is found, else returns nullptr.
UJob* ALot::HasPullJobForItem(FName item, int amount)
{
	//check if any of my jobs are a pulljob for this item.
	for (int i = 0; i < myJobs.Num(); i++)
	{
		if (myJobs[i] == nullptr || !myJobs[i])
		{
			printCritical("ERROR AT 3");
			myJobs.RemoveAt(i);
			return nullptr;
		}

		UJob* job = myJobs[i];

		if (job->item == item 
			&& job->jobType == EJobType::Pull 
			&& job->destination == this
			&& job->jobStatus == EJobStatus::Offered)
		{
			return job;
		}
	}

	//if not returned yet, doesn't have a pull job.
	return nullptr;
}

//checks if this building has an item there (eg in order to be picked up)
//will only attempt to get item from non-educt stockpile
//returns true only if predictedStock and currentStock are high enough!
bool ALot::ItemAvailable(FName item, int amount)
{
	FStockpile* pile = GetStockpile(item);
	
	
	if (pile == nullptr || !pile)
	{
		//no stockpile of this item type in this building.
		return false;
	}

	
	if (pile->stockpileType != EStockpileType::Educt 
		&& (pile->currentStored - amount) >= 0 
		&& (pile->currentPredicted - amount) >= 0)
	{
		return true;		
	}
	
	/*
	
	if (pile->stockpileType == EStockpileType::Educt)
	{
		return false;
	}

	if ((pile->currentStored - amount) >= 0 && (pile->currentPredicted - amount) >= 0)
	{
		return true;
	}
	
	*/

	return false;
}

//checks if this building has an item there (eg in order to be picked up)
//however this function will also return true for educt stockpiles! (used eg for education buildings)
//returns true only if both precictedStock and currentStock are high enough!
bool ALot::HasItemStored(FName item, int amount)
{
	FStockpile* pile = GetStockpile(item);


	if (pile == nullptr || !pile)
	{
		//no stockpile of this item type in this building.
		return false;
	}


	if ((pile->currentStored - amount) >= 0 && (pile->currentPredicted - amount) >= 0)
	{
		return true;
	}

	/*

	if (pile->stockpileType == EStockpileType::Educt)
	{
	return false;
	}

	if ((pile->currentStored - amount) >= 0 && (pile->currentPredicted - amount) >= 0)
	{
	return true;
	}

	*/

	return false;
}

//checks if this building has enough space for these items 
//will only attempt to fit item into non-product stockpile
//returns true only if precictedStock and currentStock are low enough!
bool ALot::CanFitItem(FName item, int amount)
{
	FStockpile* pile = GetStockpile(item);
	if (pile == nullptr || !pile)
	{
		//no stockpile of this item type in this building.
		return false;
	}

	if (pile->stockpileType != EStockpileType::Product 
		&& (pile->currentStored + amount) <= pile->maxStorage 
		&& (pile->currentPredicted + amount) <= pile->maxStorage)
	{
		return true;
	}

	return false;
}

//like CanFitItem, checks if this building has enough space for these items 
//however this function will also return true for product stockpiles! (used eg for harvesting buildings)
//returns true only if precictedStock and currentStock are low enough!
bool ALot::SpaceForItem(FName item, int amount)
{
	FStockpile* pile = GetStockpile(item);
	if (pile == nullptr || !pile)
	{
		//no stockpile of this item type in this building.
		return false;
	}

	if ((pile->currentStored + amount) <= pile->maxStorage 
		&& (pile->currentPredicted + amount) <= pile->maxStorage)
	{
		return true;
	}

	return false;
}

//removes this item from the predicted stockpile
//CHECK IF POSSIBLE by using ItemAvailable before this!
void ALot::RemovePredictedItem(FName item, int amount)
{
	FStockpile* pile = GetStockpile(item);
	if (pile == nullptr || !pile)
	{
		//no stockpile of this item type in this building. THIS MEANS THERE IS AN ERROR SOMEWHERE
		printCritical("ERROR: Tried to remove Predicted item but didn't have stockpile!!");
		return;
	}
	pile->currentPredicted -= amount;

	if (pile->currentPredicted < 0)
	{
		printCritical("ERROR: removed Predicted item but current predicted was <0!!");
		pile->currentPredicted = 0;
	}

}

//adds this item to the predicted stockpile
//CHECK IF POSSIBLE by using CanFitItem before this!
void ALot::AddPredictedItem(FName item, int amount)
{
	FStockpile* pile = GetStockpile(item);
	if (pile == nullptr || !pile)
	{
		//no stockpile of this item type in this building.
		//playerLabel = UGlobals::GetItemName(item);
		printCritical("ERROR: Tried to add Predicted item but didn't have stockpile!!");
		return;
	}
	pile->currentPredicted += amount;

	if (pile->currentPredicted > pile->maxStorage)
	{
		pile->currentPredicted = pile->maxStorage;

		//currently, I know that this happens with schools.
		if (playerLabel == "School")
		{
			//don't print, I know this happens
		}
		else
		{
			printCritical("ERROR: added Predicted item but currentPredicted was above maxStorage!!");
			printCritical("NOT FROM A SCHOOL!");
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------
//Stockpile struct

FStockpile::FStockpile()
{ 
	itemType = "MissingNo";
	maxStorage = 0;
	currentStored = 0;
	currentPredicted = 0;
}

FStockpile::FStockpile(FName type, EStockpileType pileType, int maxStore)
{
	itemType = type;
	stockpileType = pileType;
	maxStorage = maxStore;
	currentStored = 0;
	currentPredicted = 0;
}

//void FStockpile::AddItems(int x)
//{
//	currentStored += x;
//	if (currentStored > maxStorage)
//	{
//		currentStored = maxStorage;
//
//	/*	if (GEngine)
//		{
//			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Tried to add item over limit!");
//		}*/
//	}
//}
//
//void FStockpile::RemoveItems(int x)
//{
//	currentStored -= x;
//	if (currentStored < 0)
//	{
//		currentStored = 0;
//
//	}
//}