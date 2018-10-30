// Fill out your copyright notice in the Description page of Project Settings.

#include "Globals.h"

FItemAmount::FItemAmount()
{
}


UGlobals::UGlobals()
{
}

UGlobals::~UGlobals()
{
}

//returns the (player-side) name of the Subject
FString UGlobals::GetSubjectType(ESubjectType subject)
{
	switch (subject)
	{
	case ESubjectType::MissingNo:
		return "MISSING";
		break;
	case ESubjectType::Debug:
		return "DEBUG";
		break;
	case ESubjectType::Serf:
		return "Serf";
		break;
	case ESubjectType::Builder:
		return "Builder";
		break;
	case ESubjectType::None:
		return "No one";
		break;
	case ESubjectType::Lumberjack:
		return "Lumberjack";
		break;
	case ESubjectType::Mason:
		return "Mason";
		break;
	case ESubjectType::Farmer:
		return "Farmer";
		break;
	case ESubjectType::Miner:
		return "Miner";
		break;
	case ESubjectType::Fisher:
		return "Fisher";
		break;
	case ESubjectType::Rancher:
		return "Rancher";
		break;
	case ESubjectType::Carpenter:
		return "Carpenter";
		break;
	case ESubjectType::Smelter:
		return "Smelter";
		break;
	case ESubjectType::Smith:
		return "Smith";
		break;
	case ESubjectType::Baker:
		return "Baker";
		break;
	case ESubjectType::Butcher:
		return "Butcher";
		break;
	case ESubjectType::Tanner:
		return "Tanner";
		break;
	case ESubjectType::Recruit:
		return "Recruit";
		break;
	case ESubjectType::Militia:
		return "Militia";
		break;
	case ESubjectType::Axeman:
		return "Axeman";
		break;
	case ESubjectType::Swordsman:
		return "Swordsman";
		break;
	case ESubjectType::Lancer:
		return "Lancer";
		break;
	case ESubjectType::Halbardier:
		return "Halbardier";
		break;
	case ESubjectType::Archer:
		return "Archer";
		break;
	case ESubjectType::Crossbowman:
		return "Crossbowman";
		break;
	case ESubjectType::Scout:
		return "Scout";
		break;
	case ESubjectType::Horseman:
		return "Horseman";
		break;
	case ESubjectType::Knight:
		return "Knight";
		break;
	case ESubjectType::Barbarian:
		return "Barbarian";
		break;
	case ESubjectType::Miller:
		return "Miller";
		break;
	case ESubjectType::Forester:
		return "Forester";
		break;
	default:
		return "DEFAULT!?";
		break;
	}
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

