// Fill out your copyright notice in the Description page of Project Settings.

#include "JobManager.h"


//TODO REMOVE

//print to screen macro
#define printCritical(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 600.0, FColor::Red,text)

#define MAX_JOB_DISTANCE 10000.0


FLotWithJob::FLotWithJob()
{}

FLotWithJob::FLotWithJob(ALot* theLot, UJob* theJob)
{
	lot = theLot;
	job = theJob;
}

FDTS_Item::FDTS_Item()
{
}

FDTS_Building::FDTS_Building()
{
}

FDTS_Harvestable::FDTS_Harvestable()
{
}

UJob::UJob()
{}

//Call after creating a new job.
void UJob::InitJob(EJobType type, ALot* employee, FName it, float prio)
{
	jobType = type;
	item = it;
	priority = prio;

	jobStatus = EJobStatus::Offered;

	//may not have an employee specified eg with FindJobForItem
	if (employee && employee != nullptr)
	{
		if (type == EJobType::Pull)
		{
			destination = employee;
		}
		else
		{
			source = employee;
		}
	}
}

//cancels job. Frees items from assignment (Does different things depending on Job status)
	//also used when finished a job properly!
//does NOT explicitly delete the job object, it (should) be destroyed by GC (if the reference to it was also removed from an assigned serf).
void UJob::CancelJob()
{
	if (jobStatus != EJobStatus::Offered)
	{
		if (destination == nullptr || !destination)
		{
			printCritical("Error with dest around here!");
			return;
		}
		if (source == nullptr || !source)
		{
		//	printCritical("Error with source around here!");
		}
	}

	if (jobStatus == EJobStatus::Offered)
	{
		//remove or add item to predicted items of building. Only one of those is filled yet.

		if (jobType == EJobType::Pull)
		{

			if (destination == nullptr || !destination)
			{
				printCritical("Error with dest IN here!");
				return;
			}
			destination->RemovePredictedItem(item, 1);

			destination->RemoveJob(this);
		}
		else
		{
			if (source == nullptr || !source)
			{
				printCritical("Error with source IN  here!");
				return;
			}
			source->AddPredictedItem(item, 1);

			source->RemoveJob(this);
		}
	}
	else if (jobStatus == EJobStatus::ToSource)
	{
		if (source == nullptr || !source)
		{
			printCritical("Error with source in ToSource here!");
			return;
		}

		//reset prediction for source and destination buildings.

		source->AddPredictedItem(item, 1);
		destination->RemovePredictedItem(item, 1);

		destination->RemoveJob(this);
		source->RemoveJob(this);
	}
	else if (jobStatus == EJobStatus::ToDestination)
	{
		if (source == nullptr || !source)
		{
			printCritical("Error with source in ToDestination here!");
			return;
		}

		//item has already been picked up from source, only remove prediction on destination!

		destination->RemovePredictedItem(item, 1);

		destination->RemoveJob(this);
		source->RemoveJob(this);
	}
	else if (jobStatus == EJobStatus::Finished)
	{
		if (source == nullptr || !source)
		{
			//TODO
			//this case happens often. should be fine though? afaik, there are no issues with it.

			//printCritical("Error with source in Finished here!");
			
			if (destination && destination != nullptr)
			{
				destination->RemoveJob(this);
			}
			return;
		}

		//job has been finished properly! Just Delete Job.

		destination->RemoveJob(this);
		source->RemoveJob(this);
	}

	//UObjects without references are removed by Unreal's garbage collection.
	//that meanst the object should be gone after it is removed from the jobManager's list, and the serf's myJob Variable!

}



AJobManager::AJobManager()
{
	InitItemPriorities();
}


//adds the lot to the lots array
void AJobManager::AddLot(ALot* lot)
{
	if (lots.Find(lot) == INDEX_NONE)
	{
		lots.Add(lot);
	}
}

//removes lot from the lots array
void AJobManager::RemoveLot(ALot* lot)
{
	lots.Remove(lot);
}

//removes all jobs associated with this Lot from the jobOffers List.
//Does NOT remove jobs that are "done" with this lot, IE jobs that have this lot as source, but are jobstatus past ToSource!
void AJobManager::RemoveJobs(ALot* lot)
{
	//Job Offers

	//iterate backwards to avoid crashes / skipping items
	for (int i = jobOffers.Num() - 1; i >= 0; i--)
	{
		if (jobOffers[i] == nullptr || !jobOffers[i])
		{
			printCritical("CRITICAL AT 16!");
			return;
		}

		if (jobOffers[i]->source == lot || jobOffers[i]->destination == lot)
		{
			jobOffers[i]->CancelJob();
			jobOffers.RemoveAt(i);
		}
	}

	//active Jobs

	//iterate backwards to avoid crashes / skipping items
	for (int i = activeJobs.Num() - 1; i >= 0; i--)
	{
		if (activeJobs[i] == nullptr || !activeJobs[i])
		{
			printCritical("CRITICAL AT 17!");
			return;
		}


		if (activeJobs[i]->source == lot || activeJobs[i]->destination == lot)
		{
			//do not cancel active jobs that have this job as source
			if (activeJobs[i]->source == lot)
			{
				if (activeJobs[i]->jobStatus != EJobStatus::ToSource)
				{
					activeJobs[i]->CancelJob();
					activeJobs.RemoveAt(i);
				}
			}

			else
			{
				activeJobs[i]->CancelJob();
				activeJobs.RemoveAt(i);
			}
		}
	}
}

//called from a lot making a jobOffer.
//Creates a new Job and inits it's vars
//adds job to JobOffers, as well as the employee's myJobs.
//handles predicted items of employee, depending on jobType.
UJob* AJobManager::MakeJob(EJobType jobType, ALot* employee, FName item, float prio)
{
	UJob* job;
	job = NewObject<UJob>();
	job->InitJob(jobType, employee, item, prio);

	jobOffers.Add(job);

	//may not always have specified an employee such as in FindJobForItem
	if (employee && employee != nullptr)
	{
		employee->AddJob(job);

		if (jobType == EJobType::Pull)
		{
			employee->AddPredictedItem(item, 1);
		}
		else
		{
			employee->RemovePredictedItem(item, 1);
		}

	}
	return job;
}

// alls Job::CancelJob(), then removes job from the list.
void AJobManager::CancelJob(UJob* job)
{
	if (job == nullptr || !job)
	{
		//job is somehow invalid (I have no idea how this could conceivably happen.)
		printCritical("Job invalid at 12");
		return;
	}
	job->CancelJob();

	if (job->jobStatus == EJobStatus::Offered)
	{
		jobOffers.Remove(job);
	}
	else
	{
		activeJobs.Remove(job);
	}
}



//called from a serf looking for a job. 
//Checks the jobOffers List and searches for a suitable job.
//if successful, transfers Job to activeJobs and assignes serf etc
//if unsucessful returns nullptr.
UJob* AJobManager::FindJob()
{
	FLotWithJob* chosenJob = nullptr;
	UJob* jobToCancel = nullptr;
	float chosenPrio = 0;

	//iterate through jobOffers until a job is found that can be executed
	for (int i = 0; i < jobOffers.Num(); i++)
	{
		if (jobOffers[i] == nullptr || !jobOffers[i])
		{
			//job is somehow invalid (I have no idea how this could conceivably happen.)
			printCritical("ERROR: job offer was invalid wth!!");
			break;
		}
		UJob* job = jobOffers[i];

		//only look at job if it has a higher priority than the current chosen one.
		if (job->priority > chosenPrio)
		{
			//Pull Jobs
			if (job->jobType == EJobType::Pull)
			{
				//first, try to find a lot that has an item it already wants to get rid of (push job).

				FLotWithJob* flwj = FindLotWithPushJob(job->destination, job->item);
				if (flwj && flwj != nullptr)
				{
					//found a Lot that has a push job for this item!

					//update chosen priority
					chosenPrio = job->priority;

					//that job would have to be cancelled, so save that to jobToCancel
					jobToCancel = flwj->job;

					//save the job and the target lot to chosenJob
					chosenJob = new FLotWithJob(flwj->lot, job);
				}

				//if no job was found, try to find an item that is available somewhere.
				ALot* lot = FindItem(job->destination, job->item);
				if (lot && lot != nullptr)
				{
					//found a valid item

					//update chosen priority
					chosenPrio = job->priority;

					//no job would be cancelled, so set it to nullptr (as it might've been set earlier)
					jobToCancel = nullptr;

					//save the job and the target lot to chosenJob
					chosenJob = new FLotWithJob(lot, job);
				}
			}

			//Push Jobs
			else
			{
				//first, try to find a lot that wants this item already (pull job).
				UJob* otherJob = FindPullJobForItem(job->source, job->item);
				if (otherJob && otherJob != nullptr)
				{
					//found a Lot that has a pull job for this item!
					ALot* targetLot = otherJob->destination;


					//update chosen priority
					chosenPrio = job->priority;

					//that job would have to be cancelled, so save that to jobToCancel
					jobToCancel = otherJob;

					//save the job and the target lot to chosenJob
					chosenJob = new FLotWithJob(targetLot, job);
				}


				//if no job was found, try to find an item that is available somewhere.
				ALot* lot = FindFreeSpace(job->source, job->item);
				if (lot && lot != nullptr)
				{
					//found a valid item

					//update chosen priority
					chosenPrio = job->priority;

					//no job would be cancelled, so set it to nullptr (as it might've been set earlier)
					jobToCancel = nullptr;

					//save the job and the target lot to chosenJob
					chosenJob = new FLotWithJob(lot, job);
				}
			}
		}
	}

	//if we found a valid job
	if (chosenJob != nullptr)
	{
		//if another job is merged into this job, cancel it
		if (jobToCancel != nullptr)
		{
			CancelJob(jobToCancel);
		}

		return AcceptJob(chosenJob->lot, chosenJob->job);
	}

	//if didn't return yet, couldn't find any job.
	return nullptr;
}






//previous version, without job priority
//UJob* AJobManager::FindJob()
//{
//
//	//iterate through jobOffers until a job is found that can be executed
//	for(int i = 0; i < jobOffers.Num(); i++)
//	{
//		if (jobOffers[i] == nullptr || !jobOffers[i])
//		{
//			//job is somehow invalid (I have no idea how this could conceivably happen.)
//			printCritical("ERROR: job offer was invalid wth!!");
//			break;
//		}
//		UJob* job = jobOffers[i];
//
//		//Pull Jobs
//		if (job->jobType == EJobType::Pull)
//		{
//			//first, try to find a lot that has an item it already wants to get rid of (push job).
//			
//			FLotWithJob* flwj = FindLotWithPushJob(job->destination, job->item);
//			if (flwj && flwj != nullptr)
//			{
//				//found a Lot that has a push job for this item!
//
//				//cancel the job that is merged into this job.
//				CancelJob(flwj->job);
//
//				//accept this job.
//				return AcceptJob(flwj->lot, job);
//			}
//
//			//if no job was found, try to find an item that is available somewhere.
//			ALot* lot = FindItem(job->destination, job->item);			
//			if (lot && lot != nullptr)
//			{
//				//found a valid item
//				return AcceptJob(lot, job);
//			}
//		}
//
//		//Push Jobs
//		else
//		{
//			//first, try to find a lot that wants this item already (pull job).
//			UJob* otherJob = FindPullJobForItem(job->source, job->item);
//			if (otherJob && otherJob != nullptr)
//			{
//				//found a Lot that has a pull job for this item!
//				ALot* targetLot = otherJob->destination;
//				//cancel the job that is merged into this job.
//				CancelJob(otherJob);
//
//				//accept this job.
//				return AcceptJob(targetLot, job);
//			}
//
//
//			//if no job was found, try to find an item that is available somewhere.
//			ALot* lot = FindFreeSpace(job->source, job->item);
//			if (lot && lot != nullptr)
//			{
//				//found a valid item
//				return AcceptJob(lot, job);
//			}
//		}
//	}
//
//	//if didn't return yet, couldn't find any job.
//	return nullptr;
//}
//



// EVEN older version
//UJob* AJobManager::FindJob()
//{
//
//	//iterate through jobOffers until a job is found that can be executed
//	for (int i = 0; i < jobOffers.Num(); i++)
//	{
//		UJob* job = jobOffers[i];
//
//		//Pull Jobs
//		if (job->jobType == EJobType::Pull)
//		{
//			//iterate through all lots until one is found that can spare the item.
//			for (int j = 0; j < lots.Num(); j++)
//			{
//				ALot* lot = lots[j];
//
//				if (job->source == lot || job->destination == lot)
//				{
//					//trying to find an item in the same building. Skip.
//					continue;
//				}
//				if (lot->ItemAvailable(job->item, 1))
//				{
//
//
//					//lot has an item that can be used for this job, reserve it for this job!
//					lot->RemovePredictedItem(job->item, 1);
//
//					//add 1 to predicted items (because an item is coming in now)
//					job->destination->AddPredictedItem(job->item, 1);
//
//
//					//change some variables in the job
//					job->AssignTarget(lot);
//
//					//move job into active list
//					activeJobs.Add(job);
//					jobOffers.Remove(job);
//
//					return job;
//				}
//			}
//		}
//
//		//Push Jobs
//		else
//		{
//			//iterate through all lots until one is found that can fit the item.
//			for (int j = 0; j < lots.Num(); j++)
//			{
//				ALot* lot = lots[j];
//
//				if (job->source == lot || job->destination == lot)
//				{
//					//trying to find an item in the same building. Skip.
//					continue;
//				}
//
//				if (lot->CanFitItem(job->item, 1))
//				{
//					//lot has space in stockpile that can be used for this job, reserve it for this job!
//					lot->AddPredictedItem(job->item, 1);
//
//					//remove 1 from predicted items (because an item will be taken out)
//					job->source->RemovePredictedItem(job->item, 1);
//
//
//					//change some variables in the job
//					job->AssignTarget(lot);
//
//					//move job into active list
//					activeJobs.Add(job);
//					jobOffers.Remove(job);
//
//					return job;
//				}
//			}
//		}
//	}
//
//
//	return nullptr;
//}


/*
  *tries to find a suitable job that involves this lot. (aka chainjob)

  priorities in order:
  fulfill existing pushjob from this building, also fulfilling a pulljob from another building
  fulfill existing pulljob for an item that was produced in this building (for which push job was not made yet)
  fulfill existing pushjob from this building (preferably to a non-warehouse, optimized somewhere else as below)
  create pushjob for an item that was produced in this building (optimized somewhere else: that pushjob will try to push to a building educt slot rather than a warehouse)

  *called by serfs after they drop an item in lot ("chainjob")
  *returns a suitable job that has been set to accepted in this method.
  returns nullptr if no suitable job is found. 
  (always returns nullptr for warehouse stockpiles)
*/
UJob* AJobManager::FindJobByLot(ALot* lot)
{
	//TODO could do some optimization here, checking wether this lot even has products / warehouse slots, because if not, then I cant't even chainjob!
	//TODO could also optimize a bit by caching the results of looking for certain types of lots into an array...

	///first, look to complete a pushjob from this lot, to satisfy a pulljob from another lot.
	//iterate through myjobs, looking for pushjobs.
	for (int i = 0; i < lot->myJobs.Num(); i++)
	{
		if(lot->myJobs[i] == nullptr || !lot->myJobs[i])
		{
			printCritical("ERROR AT 1");
			return nullptr;
		}

		UJob* job = lot->myJobs[i];

		//if the source for this job is this lot, it is a pushjob made by this building.(only consider offered, not active jobs!)
		if (job->source == lot && job->jobType == EJobType::Push && job->jobStatus == EJobStatus::Offered)
		{
			//try to find a lot that has created a pulljob asking for this item.
			UJob* otherJob = FindPullJobForItem(job->source, job->item);
			if (otherJob && otherJob != nullptr)
			{
				//found a lot that has a pulljob for this item!
				ALot* targetLot = otherJob->destination;
				//cancel the pulljob that was found (because it is "merged" into the push job that "lot" made)
				CancelJob(otherJob);
				
				//accept the pushjob from the lot the serf is looking at ("lot"), using the other lot (which had the pulljob) as target!
				return AcceptJob(targetLot, job);
			}
		}
	}

	///second, check if product in this lot can be used for a pulljob. (although no push job was made yet)
	//iterate through all stockpiles
	for (int i = 0; i < lot->stockpiles.Num(); i++)
	{
		FStockpile* stock = &(lot->stockpiles[i]);
		if (stock->stockpileType == EStockpileType::Educt)
		{
			//educts are not taken out of a building.
			continue;
		}	

		//design decision: do I want warehouses to also chainjob or not?
		//If they do, that means more chainjobs which is cool. 
		//But if they don't, that means serfs more frequently break out of the chain, so harvesting buildings are more likely to get their produce taken out (for example)
		//->so I don't do chainjobs for warehouses.
		else if (stock->stockpileType == EStockpileType::Warehouse)
		{
			//this function should not be used for warehouses!!
			return nullptr;
		}		


		//check if this stockpile has items available
		if (!lot->ItemAvailable(stock->itemType, 1))
		{
			//this stockpile doesn't have items available.
			continue;
		}

		//there is an item available, try to find a job that needs this item.
		UJob* job = FindPullJobForItem(lot, stock->itemType);
		if (job && job != nullptr)
		{
			//valid job was found. Accept that job, using this lot as target.
			return AcceptJob(lot, job);
		}	
	}
	
	///third, try to fullfill an existing push job, without fulfulling a pulljob a the same time.
	//iterate through jobs of this lot, looking for pushjobs.
	for (int i = 0; i < lot->myJobs.Num(); i++)
	{
		if (lot->myJobs[i] == nullptr || !lot->myJobs[i])
		{
			printCritical("ERROR AT 5");
			return nullptr;
		}
		UJob* job = lot->myJobs[i];

		//if the source for this job is this lot, it is a pushjob made by this building.
		if (job->source == lot && job->jobType == EJobType::Push && job->jobStatus == EJobStatus::Offered)
		{
			//found a pushjob from this lot
			
			if (job && job != nullptr)
			{
				//job is valid

				//look for a lot that can fit this item. (FindFreeSpace will always try to find educt stockpiles over warehouses)
				ALot* target = FindFreeSpace(lot, job->item);
				if (target && target != nullptr)
				{
					//returned Lot is valid
					return AcceptJob(target, job);
				}
			}

		}
	}

	///fourth, see if there are any items in a product stockpile and just push them anywhere (this could be an educt stockpile, but more likely to be a warehouse)
	//iterate through stockpiles of this lot, looking for available product items to push..
	for (int i = 0; i < lot->stockpiles.Num(); i++)
	{
		FStockpile* stock = &(lot->stockpiles[i]);
		if (stock->stockpileType == EStockpileType::Educt)
		{
			//educts are not taken out of a building.
			continue;
		}

		//TODO design decision: do I want warehouses to also chainjob or not?
		//If they do, that means more chainjobs which is cool. But if they don't, that means serfs more frequently break out of the chain, so harvesting buildings are more likely to get their produce taken out (for example)
		else if (stock->stockpileType == EStockpileType::Warehouse)
		{
			//this function should not be used for warehouses!!
			return nullptr;
		}


		//check if this stockpile has items available
		if (!lot->ItemAvailable(stock->itemType, 1))
		{
			//this stockpile doesn't have items available.
			continue;
		}

		//there is an item available.
		//now look for a lot that can fit this item. (FindFreeSpace will always try to find educt stockpiles over warehouses)
		ALot* target = FindFreeSpace(lot, stock->itemType);
		if (target && target != nullptr)
		{
			//found a lot that can fit the item.

			//create a new pushjob for this item, and accept it, selecting the previously found lot as target.
			UJob* job = MakeJob(EJobType::Push, lot, stock->itemType, 0.76667);
			return AcceptJob(target, job);
		}
	}
	
	//if not returned yet, didn't find any job that could be made from this lot.
	return nullptr;
}

/*
*tries to find a suitable job that involves this Item.

priorities in order:
fulfill existing pulljob for this item
find suitable educt stockpile that didn't create pulljob yet
dump in a warehouse.

*called by serfs after their job has been found invalid while carrying an item (eg if their destination building was destroyed)
*returns a suitable job that has already been set to accepted.
*/
UJob* AJobManager::FindJobForItem(FName item)
{
	//Try to fullfill a pulljob

	UJob* job = FindPullJobForItem(nullptr, item);
	if (job && job != nullptr)
	{

		//already have item, so do not need to (or even can) pick it up
		job->jobStatus = EJobStatus::ToDestination;

		//found a job
		return AcceptJob(nullptr, job);
	}

	//find a lot that can fit this item (prefs non-warehouse)
	ALot* lot = FindFreeSpace(nullptr, item);
	if (lot && lot != nullptr)
	{
		//found lot that has space for this item

		UJob* newJob = MakeJob(EJobType::Push, nullptr, item, 1000);
		
		//already have item, so do not need to (or even can) pick it up
		newJob->jobStatus = EJobStatus::ToDestination;
		
		return AcceptJob(lot, newJob);
	}

	//if not returned yet, cannot find suitable job.
	return nullptr;
}



//UJob* AJobManager::FindJobByLot(ALot* lot)
//{
//	//do for all product stockpiles
//	for (int i = 0; i < lot->stockpiles.Num(); i++)
//	{
//		if (lot->stockpiles[i].stockpileType == EStockpileType::Educt)
//		{
//			//educts are not taken out of a building.
//			continue;
//		}
//		else if (lot->stockpiles[i].stockpileType == EStockpileType::Warehouse)
//		{
//			//this function should not be used for warehouses!!
//			return nullptr;
//		}
//
//		//See if a pullrequest can be fulfilled.
//
//		//This confused myself so here's some info:
//		//This method only consideres looking for a pulljob if there are items not assigned yet, because a product canot be assigned to a pull request, without also having a serf assigned already!
//		//So all items that are marked as busy (if currentPredicted < currentStocked) are either assigned to a push job (checked for below) or already have a serf assigned to that job!
//
//		//check if an item can be spared from this stockpile
//		if (lot->stockpiles[i].currentPredicted > 0)
//		{
//			//this stockpile has unassigned items, see if you can fill a pullrequest.
//
//			EItem item = lot->stockpiles[i].itemType;
//
//
//			//iterate through jobOffers until a pulljob is found that needs this item
//			for (int i = 0; i < jobOffers.Num(); i++)
//			{
//				UJob* job = jobOffers[i];
//
//				if (job->jobType == EJobType::Pull)
//				{
//					if (job->item == item)
//					{
//						//found a job that needs this item. Accept it.					
//
//						//lot has an item that can be used for this job, reserve it for this job!
//						lot->RemovePredictedItem(item, 1);
//						
//						//do not add to predicted of destination, the existing job already did that!
//
//						//change some variables in the job
//						job->AssignTarget(lot);
//
//						//move job into active list
//						activeJobs.Add(job);
//						jobOffers.Remove(job);
//
//						return job;
//					}
//				}
//			}
//
//		}
//	}
//	//if no pull jobs can be filled, try doing an existing push job for this building
//
//	//iterate through jobOffers until a pushjob by this building is found
//	for (int i = 0; i < jobOffers.Num(); i++)
//	{
//		UJob* job = jobOffers[i];
//
//		//if the source for this job is this lot, it is a pushjob made by this building.
//		if (job->source == lot && job->jobType == EJobType::Push)
//		{
//			//found a pushjob by this lot, attempt to execute it.
//			//(as this pushjob already exists, I do not need to check for currentStock or predictedStock.)
//			
//			//try to find a destination for the item
//
//
//			//iterate through all lots until one is found that can fit the item.
//			for (int j = 0; j < lots.Num(); j++)
//			{
//				ALot* blot = lots[j];
//
//				if (lot == blot)
//				{
//					//don't look at yerself
//					continue;
//				}
//
//				if (blot->CanFitItem(job->item, 1))
//				{
//
//					//reserve space in blot for this job
//					blot->AddPredictedItem(job->item, 1);
//
//					//do not remove from predicted of this lot, the existing job already did that!
//
//					//change some variables in the job
//					job->AssignTarget(blot);
//
//					//move job into active list
//					activeJobs.Add(job);
//					jobOffers.Remove(job);
//
//					return job;
//				}
//			}			
//		}
//	}
//
//	//I could also look to create a new push job here, but if there is no push job from this building yet, it apparently isn't necessary (because stock is empty, or busy or whatever)
//	
//
//	return nullptr;
//}


void AJobManager::InitItemPriorities()
{
	FName itemsDataTablePath = "DataTable'/Game/WnSAssets/Data/Items/DT_Items.DT_Items'";
	itemsDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), NULL, *itemsDataTablePath.ToString()));

	if (itemsDataTable == nullptr || !itemsDataTable)
	{
		printCritical("JobManager InitItemPriorities did not find Datatable!");
	}
}

//returns the priority value of the item.
float AJobManager::GetItemPriority(FName item)
{
	FDTS_Item* row = itemsDataTable->FindRow<FDTS_Item>(item, TEXT(""));

	return row->ItemPriority;
}

///Helper functions

//returns the closest pullJob that requires the item, nullptr if none is found.
//source is the Lot the request is coming from 
//(if none found, I suggest to try FindFreeSpace())
UJob* AJobManager::FindPullJobForItem(ALot* source, FName item)
{
	UJob* chosenJob = nullptr;
	float chosenJobDistance = MAX_JOB_DISTANCE;

	//cache whether source lot exists / was provided. (performance)
	bool hasSource = false;
	if (source && source != nullptr)
	{
		hasSource = true;
	}


	//iterate through jobOffers until a pulljob is found that needs this item
	for (int i = 0; i < jobOffers.Num(); i++)
	{
		if (jobOffers[i] == nullptr || !jobOffers[i])
		{
			printCritical("ERROR AT 4");
			return nullptr;
		}
		UJob* job = jobOffers[i];

		if (job->jobType == EJobType::Pull)
		{
			if (job->item == item)
			{
				//found a job that needs this item.

				if (hasSource)
				{
					//if pulljob has a valid destination
					if (job->destination && job->destination != nullptr)
					{
						float newJobDist = job->destination->GetDistanceTo(source);

						//if this job is closer than the lsat
						if (newJobDist < chosenJobDistance)
						{
							//new lot is closer than previous. replace.
							chosenJob = job;
							chosenJobDistance = newJobDist;
						}
					}
					else
					{
						//pulljob without destination is a bug. Notify.
						printCritical("PullJob without a destination! in FinPulljobForItem. This is a very bad bug!");
					}
				}
				else
				{
					//if there is no source building to measure the distance to, just return the first job.
					return job;
				}
			}
		}
	}

	return chosenJob;
}

//returns an offered (not active) pushJob that this Lot made; nullptr if none is found.
UJob* AJobManager::FindPushJobForLot(ALot* source)
{
	//iterate through myjobs until one is found that is a pushjob.
	for (int i = 0; i < source->myJobs.Num(); i++)
	{
		UJob* job = source->myJobs[i];

		//if the source for this job is this lot, it is a pushjob made by this building.
		if (job->source == source && job->jobType == EJobType::Push && job->jobStatus == EJobStatus::Offered)
		{
			return job;
		}
	}

	//if none was returned yet, cannott find one.
	return nullptr;
}

//OLD VERSION, WITHOUT MYJOBS, probably less performant.
////returns a pushJob that this Lot made; nullptr if none is found.
//UJob* AJobManager::FindPushJobForLot(ALot* source)
//{
//	//iterate through jobOffers until a pushjob by this building is found
//	for (int i = 0; i < jobOffers.Num(); i++)
//	{
//		UJob* job = jobOffers[i];
//
//		//if the source for this job is this lot, it is a pushjob made by this building.
//		if (job->source == source && job->jobType == EJobType::Push)
//		{
//			return job;
//		}
//	}
//	return nullptr;
//}



//returns the closest Lot that can fit (predicted and actual) the item (educt or warehouse), nullptr if none is found.
//prefers only selects a warehouse if no suitable other lot is found.
//source is the Lot the request is coming from
ALot* AJobManager::FindFreeSpace(ALot* source, FName item)
{
	ALot* chosenLot = nullptr;
	float chosenLotDistance = MAX_JOB_DISTANCE;

	//cache whether destination lot exists / was provided. (performance)
	bool hasSource = false;
	if (source && source!= nullptr)
	{
		hasSource = true;
	}

	//if I find a warehouse, hold on to that, I might chose it as destination later.
	ALot* warehouse = nullptr;
	bool foundWarehouse = false;

	//iterate through all lots until one is found that can fit the item.
	for (int i = 0; i < lots.Num(); i++)
	{
		ALot* lot = lots[i];

		//first check whether it has a stockpile for this item
		if (lot->GetStockpile(item) == nullptr)
		{
			//doesn't have a stockpile for this item, continue.
			continue;
		}

		if (lot->GetStockpile(item)->stockpileType == EStockpileType::Warehouse)
		{
			//don't consider warehouses yet, But hold on to warehouse (as last resort) and continue.

			//check if this warehouse is the closest warehouse to the source. if there is no source (eg serf still carrying an item from an invalidated job), just pick any warehouse.

			//if already saved a warehouse
			if(warehouse && warehouse != nullptr)
			{
				//if there is a source that can be used as a measuring point for the distance
				if (source && source != nullptr)
				{
					//find which warehouse is closer
					float oldWhDist = warehouse->GetDistanceTo(source);
					float newWhDist = lot->GetDistanceTo(source);

					if (newWhDist < oldWhDist)
					{
						//if new warehouse is closer, replace old warehouse
						warehouse = lot;
					}
					//else, do nothing.
				}
				//else, cannot test which is closer, so do nothing.
			}
			else
			{
				warehouse = lot;
				foundWarehouse = true;
			}
			continue;
		}
		
		if (lot == source)
		{
			//don't look at yerself
			continue;
		}

		if (lot->CanFitItem(item, 1))
		{
			//this lot can fit the item.

			if (hasSource)
			{
				float newLotDist = lot->GetDistanceTo(source);

				//if this lot is closer than the lsat
				if (newLotDist < chosenLotDistance)
				{
					//new lot is closer than previous. replace.
					chosenLot = lot;
					chosenLotDistance = newLotDist;
				}
				//else, stay with the previous chosen lot
			}
			else
			{
				//if no source was provided, just return the first possible lot.
				return lot;
			}
		}
	}

	if (chosenLot != nullptr)
	{
		//if a lot was found, return it.
		return chosenLot;
	}

	//no lot found. If found a warehouse, try to choose that as destination.
	if (foundWarehouse)
	{
		if (warehouse->CanFitItem(item, 1))
		{
			return warehouse;
		}
	}

	return nullptr;
}

//returns the closest Lot that has this item to offer (product or warehouse), nullptr if none is found.
//destination is the Lot the request is coming from
ALot* AJobManager::FindItem(ALot* destination, FName item)
{
	ALot* chosenLot = nullptr;
	float chosenLotDistance = MAX_JOB_DISTANCE;
	
	//cache whether destination lot exists / was provided. (performance)
	bool hasDestination = false; 
	if (destination && destination != nullptr)
	{
		hasDestination = true;
	}

	//iterate through all lots until one is found that has the item available.
	for (int i = 0; i < lots.Num(); i++)
	{
		ALot* lot = lots[i];

		if (destination == lot)
		{
			//trying to find an item in the same building. Skip.
			continue;
		}

		if (lot->ItemAvailable(item, 1))
		{
			//check whether this lot is closer than the last
			if (hasDestination)
			{
				float newLotDist = lot->GetDistanceTo(destination);

				//if this lot is closer than the lsat
				if (newLotDist < chosenLotDistance)
				{
					//new lot is closer than previous. replace.
					chosenLot = lot;
					chosenLotDistance = newLotDist;
				}
				//else, stay with the previous chosen lot
			}
			else
			{
				//if no destination lot provided to measure distance from, just return the first lot that has this item.
				return lot;
			}
		}
	}

	//if no lot was found, this returns nullptr as intended.
	return chosenLot;
}

//returns the closest lot that has an offered (but not active) push job for this item, meaning it has this item available...
//destination is the Lot this request is coming from.
//returns nullptr if no such Lot is found.
FLotWithJob* AJobManager::FindLotWithPushJob(ALot* destination, FName item)
{
	FLotWithJob* chosenFLot = nullptr;
	float chosenFLotDistance = MAX_JOB_DISTANCE;

	//cache whether destination lot exists / was provided. (performance)
	bool hasDestination = false;
	if (destination && destination != nullptr)
	{
		hasDestination = true;
	}

	//iterate through all lots until one is found that has made a push job for this item
	for (int i = 0; i < lots.Num(); i++)
	{
		ALot* lot = lots[i];
		if (!lot || lot == nullptr)
		{
			printCritical("Lot was invalid in FindLotWithPushJob!");
			continue;
		}

		if (destination == lot)
		{
			//trying to find an item in the same building. Skip.
			continue;
		}

		UJob* tempJob = lot->HasPushJobForItem(item, 1);
		if (tempJob && tempJob != nullptr)
		{
			//this lot has a push job for this item.

			//see if it is closer.
			if (hasDestination)
			{
				float newLotDist = lot->GetDistanceTo(destination);

				//if this lot is closer than the lsat
				if (newLotDist < chosenFLotDistance)
				{
					//new lot is closer than previous. replace.
					chosenFLot = new FLotWithJob(lot, tempJob);
					chosenFLotDistance = newLotDist;
				}
				//else, stay with the previous chosen lot

			}
			else
			{
				//if has no destination to measure distance to, just return the first valid one.

				FLotWithJob* returnFlwj = new FLotWithJob(lot, tempJob);
				return returnFlwj;
			}
		}
	}
	
	//if none was found, returns nullptr as intended.
	return chosenFLot;
}

//sets some variables in the target lot, and applies the target to the job.
//also moves job from offered jobs to active jobs, and sets the jobstatus to ToDestination
UJob* AJobManager::AcceptJob(ALot* target, UJob* job)
{	
	if (job == nullptr || !job)
	{
		printCritical("JOB INVALID AT 15!!");
		return nullptr;
	}

	//this function does not always receive a target, eg from FindJobForItem
	if (target && target != nullptr)
	{
		if (job->jobType == EJobType::Pull)
		{
			//target lot has an item that can be used for this job, reserve it for this job!
			target->RemovePredictedItem(job->item, 1);

			//do not add to predicted of destination, the existing job already did that!
		}
		else //pushjob
		{
			//do not remove to predicted of source, the existing job already did that!


			//target lot has space for this item, reserve the space for this job!
			target->AddPredictedItem(job->item, 1);
		}

		target->AddJob(job);

		//change some variables in the job
		if (job->jobType == EJobType::Pull)
		{
			job->source = target;
		}
		else
		{
			job->destination = target;
		}

		//if it is a new job...
		//job status may already be at another status (eg from FindJobForItem)
		if (job->jobStatus == EJobStatus::Offered)
		{
			job->jobStatus = EJobStatus::ToSource;
		}
	}

	//move job into active list
	activeJobs.Add(job);
	jobOffers.Remove(job);

	return job;
}







