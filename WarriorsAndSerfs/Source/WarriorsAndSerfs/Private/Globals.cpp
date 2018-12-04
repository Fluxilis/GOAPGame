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

FStatImportance::FStatImportance()
{
}

FHarvestJobBool::FHarvestJobBool()
{

}

FDTS_Occupation::FDTS_Occupation()
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
* the calculation in this makes it so that the final multiplier is >= the lowest multiplier and <= the highest multiplier.
* if skill level gives 140% speed, and importance is 50% (0.5), the skill will affect the speed by 120% ((140%-100%) x 0.5 + 100).
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
	TArray<FStatImportance> statImportances = theWorkType->StatImportances;

	//base speed
	float baseSpeed = theWorkType->BaseDuration;

	if (overrideBaseDuration != -1000)
	{
		baseSpeed = overrideBaseDuration;
	}

	float impactCumul = 0; //cumulative workspeed % multipliers

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
		
		float statImpact = 100 - ((100 - statValue) * importance / 100); //% impact this stat has on the work (ie total % if only this stat was counted)
		impactCumul += statImpact;
	}

	impactCumul += FCString::Atoi(*(statsMap.Find(TEXT("GlobalWorkSpeed"))->StatValue)); //GlobalWorkSpeed applies to all workspeeds

	float finalMultiplier = (impactCumul / (statImportances.Num() + 1) / 100 ); // divide by number of important stats (+1 because GlobalWorkSpeed applies to all workspeeds), divide by 100 to go from percentage to multiplier

	return baseSpeed * (finalMultiplier == 0 ? 0 : (1/ finalMultiplier)); //1/total impact because I want duration to reduce with high finalMultiplier and increase with low finalMultiplier. (confusion because I changed from "workSpeed" to "workDuration")
}

// adapted from UGlobals::CalculateWorkDuration()
// while the suggested skills don't match the final workspeeds perfectly, the match is close enough (if the suggested skills are chosen properly).
float UGlobals::CalculateOccupationSuitability(TMap<FName, FSubjectStatValue> statsMap, FName occupationType)
{
	FDTS_Occupation* theOccupationType = GetUDatatable(OCCUPATIONSDATATABLEPATH)->FindRow<FDTS_Occupation>(occupationType, TEXT(""));
	if (theOccupationType == nullptr || !theOccupationType)
	{
		//could not find this occupation in the dt
		printCritical("UGlobals::CalculateOccupationSuitability could not find a certain occupation in the DT!");
		return -1.0;
	}

	//suggested skills for this occupation
	TArray<FStatImportance> statImportances = theOccupationType->SuggestedSkills;


	float impactCumul = 0; //cumulative suitability % multipliers

	//do for every suggested skill for this occupation
	for (int i = 0; i < statImportances.Num(); i++)
	{
		FString stat = statsMap.Find(statImportances[i].StatName)->StatValue;
		if (stat == "0")
		{
			//if level of skill is "incapable", suitability is zero.
			return 0;
		}

		//workspeed for this skill Level
		FDTS_SkillLevel* currentSkillLevel = GetUDatatable(SKILLLEVELSDATATABLEPATH)->FindRow<FDTS_SkillLevel>(FName(*stat), TEXT(""));
		if (currentSkillLevel == nullptr || !currentSkillLevel)
		{
			//could not find this skillLevel in the dt
			printCritical("UGlobals::CalculateOccupationSuitability Could not find a certain skillLevel in the DT!");
			return 1.0;
		}

		int statValue = currentSkillLevel->Percentage;

		//importance of the skill for the occupation - how much the total suitability is influenced by the skill
		float importance = statImportances[i].StatImportance;

		float statImpact = 100 - ((100 - statValue) * importance / 100); //% impact this stat has on the suitability (ie total % if only this stat was counted)
		impactCumul += statImpact;
	}

	impactCumul += FCString::Atoi(*(statsMap.Find(TEXT("GlobalWorkSpeed"))->StatValue)); //GlobalWorkSpeed applies to all occupations

	float finalMultiplier = (impactCumul / (statImportances.Num() + 1) / 100); // divide by number of important stats (+1 because GlobalWorkSpeed applies to all occupations), divide by 100 to go from percentage to multiplier

	return 1.0 * finalMultiplier;
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
