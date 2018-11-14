// Fill out your copyright notice in the Description page of Project Settings.

#include "Globals.h"

FItemAmount::FItemAmount()
{
}

FDTS_Occupation::FDTS_Occupation()
{
}

FWorkSpeedStatImportance::FWorkSpeedStatImportance()
{
}

FDTS_WorkType::FDTS_WorkType()
{
}

FDTS_SubjectStat::FDTS_SubjectStat()
{
}

FSubjectStatValue::FSubjectStatValue()
{
}

FSubjectStatValue::FSubjectStatValue(FName _statName, FString _statValue)
{
	StatName = _statName;
	StatValue = _statValue;
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


float UGlobals::CalculateWorkspeed(TMap<FName, FSubjectStatValue> statsMap, FName workType)
{
	TArray<FWorkSpeedStatImportance> statImportances = GetUDatatable(WORKTYPESDATATABLEPATH)->FindRow<FDTS_WorkType>(workType, TEXT(""))->StatImportances;

	float totalSpeed = 100;

	for (int i = 0; i < statImportances.Num(); i++)
	{
		FString stat = statsMap.Find(statImportances[i].StatName)->StatValue;
		float importance = statImportances[i].StatImportance;

		float totalImpact = 100 - ((100 - FCString::Atof(*stat) * importance / 100));
		
		totalSpeed = totalSpeed * (totalImpact / 100);
	}

	return totalSpeed;
}