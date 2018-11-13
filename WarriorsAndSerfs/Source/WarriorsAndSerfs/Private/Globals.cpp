// Fill out your copyright notice in the Description page of Project Settings.

#include "Globals.h"

FItemAmount::FItemAmount()
{
}

FDTS_Occupation::FDTS_Occupation()
{
}

FDTS_SubjectStat::FDTS_SubjectStat()
{
}

UGlobals::UGlobals()
{
}

UGlobals::~UGlobals()
{
}

//returns the (player-side) name of the Lot
FString UGlobals::GetLotType(ELotType lot)
{
	switch (lot)
	{
	case ELotType::MissingNo:
		return "MISSING";
		break;
	case ELotType::Debug:
		return "DEBUG";
		break;
	case ELotType::Building:
		return "Building";
		break;
	case ELotType::Constructionsite:
		return "Constructionsite";
		break;
	case ELotType::DeadDrop:
		return "Loot Box";
		break;
	default:
		return "DEFAULT!?";
		break;
	}
}

//returns the (player-side) name of the job
FString UGlobals::GetHarvestingJobType(EHarvestingJobType job)
{
	switch (job)
	{
	case EHarvestingJobType::None:
		return "Idle";
		break;
	case EHarvestingJobType::Planting:
		return "Planting";
		break;
	case EHarvestingJobType::Returning:
		return "Returning";
		break;
	case EHarvestingJobType::Tending:
		return "Tending";
		break;
	case EHarvestingJobType::Harvesting:
		return "Harvesting";
		break;
	case EHarvestingJobType::ReturningWithHarvest:
		return "Returning with Harvest";
		break;
	case EHarvestingJobType::Polishing:
		return "Polishing";
		break;
	case EHarvestingJobType::Resting:
		return "Resting";
		break;
	default:
		return "DEFAULT!?";
		break;
	}
}

//returns the (player-side) name of the job
FString UGlobals::GetCraftingJobType(ECraftingJobType job)
{
	switch (job)
	{
	case ECraftingJobType::None:
		return "Idle";
		break;
	case ECraftingJobType::Crafting:
		return "Crafting";
		break;
	case ECraftingJobType::Resting:
		return "Resting";
		break;
	default:
		return "DEFAULT!?";
		break;
	}
}


UDataTable* UGlobals::GetUDatatable(const FString& Path)
{
	return Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), NULL, *Path));
}

