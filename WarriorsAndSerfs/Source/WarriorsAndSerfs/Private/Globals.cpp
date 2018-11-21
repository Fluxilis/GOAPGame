// Fill out your copyright notice in the Description page of Project Settings.

#include "Globals.h"

//print to screen macro
#define printCritical(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 600.0, FColor::Red,text)

FItemAmount::FItemAmount()
{
}

FFloatName::FFloatName()
{
}

FFloatName::FFloatName(FName n, float f)
{
	name = n;
	value = f;
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

FDTS_SkillLevel::FDTS_SkillLevel()
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

/*
* statsMap is the statset with which the speed for this workType should be calculated
* workType is the workType to calculate the speed of
* overrideBaseDuration is in case of crafting, where the baseDuration
*
*/
float UGlobals::CalculateWorkDuration(TMap<FName, FSubjectStatValue> statsMap, FName workType, float overrideBaseDuration)
{
	FDTS_WorkType* theWorkType = GetUDatatable(WORKTYPESDATATABLEPATH)->FindRow<FDTS_WorkType>(workType, TEXT(""));
	if (theWorkType == nullptr || !theWorkType)
	{
		//could not find this workType in the dt
		printCritical("Could not find a certain workType in the DT!");
		return 1.0;
	}

	//stat importances for this workType
	TArray<FWorkSpeedStatImportance> statImportances = theWorkType->StatImportances;

	//base speed
	float baseSpeed = theWorkType->BaseDuration;

	if (overrideBaseDuration != -1000)
	{
		baseSpeed = overrideBaseDuration;
	}

	float totalImpact = 1.0;
	
	//do for every relevant stat for this workType
	for (int i = 0; i < statImportances.Num(); i++)
	{
		FString stat = statsMap.Find(statImportances[i].StatName)->StatValue;
		if (stat == "0")
		{
			//if level of skill is "incapable", speed is zero.
			return 0;
		}

		//workspeed for this skill Level
		FDTS_SkillLevel* currentSkillLevel = GetUDatatable(SKILLLEVELSDATATABLEPATH)->FindRow<FDTS_SkillLevel>(FName(*stat), TEXT(""));
		if (currentSkillLevel == nullptr || !currentSkillLevel)
		{
			//could not find this skillLevel in the dt
			printCritical("Could not find a certain skillLevel in the DT!");
			return 1.0;
		}

		int statValue = currentSkillLevel->Percentage;

		//importance of the skill for the workType - how much the total speed is influenced by the skill
		float importance = statImportances[i].StatImportance;
		
		float statImpact = 100 - ((100 - statValue) * importance / 100);
		totalImpact = totalImpact * (statImpact / 100);
	}

	totalImpact = totalImpact * (FCString::Atoi(*(statsMap.Find(TEXT("GlobalWorkSpeed"))->StatValue)) / 100);
	return baseSpeed * (totalImpact == 0 ? 0 : (1/totalImpact)); //1/total impact because I want duration to reduce with high Impact and increase with low impact. (confusion because I changed from "workSpeed" to "workDuration")
}

FText UGlobals::GetStatDisplayText(FSubjectStatValue subjectStatVal)
{
	FDTS_SubjectStat* subjectStat = GetUDatatable(SUBJECTSTATSDATATABLEPATH)->FindRow<FDTS_SubjectStat>(subjectStatVal.StatName, TEXT(""));
	if (subjectStat == nullptr || !subjectStat)
	{
		//could not find this stat in the dt
		return FText::FromString(FString("Could not find subjectStat in datatable: ").Append(subjectStatVal.StatName.ToString()));
		printCritical("Could not find a certain subjectStat in the DT!");
	}

	FName statDisplayType = subjectStat->DisplayType;
	FString statValue = subjectStatVal.StatValue;

	if (statDisplayType == TEXT("Text"))
	{
		//just return as is
		return FText::FromString(statValue);
	}

	if (statDisplayType == TEXT("ZeroDecimalPercent")
		|| statDisplayType == TEXT("Attribute")
		|| statDisplayType == TEXT("Skill"))
	{
		int index;
		if (statValue.FindChar('.', index))
		{
			//if Value is a '.' seperated Float, return only whole number
			return FText::FromString(statValue.Left(index));
		}
		//if doesn't have decimals, return as is
		return FText::FromString(statValue);
	}

	FString failedText = "GetStatDisplayText could not find displayType: ";
	failedText.Append(statDisplayType.ToString());
	return FText::FromString(failedText);
}


TArray<FFloatName> UGlobals::SortFloatNameArray(TArray<FFloatName> arrayToSort)
{
	arrayToSort.Sort();
	return arrayToSort;
}
