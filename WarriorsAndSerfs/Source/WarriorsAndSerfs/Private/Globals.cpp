// Fill out your copyright notice in the Description page of Project Settings.

#include "Globals.h"

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


float UGlobals::CalculateWorkspeed(TMap<FName, FSubjectStatValue> statsMap, FName workType)
{
	//stat importances for this workType
	TArray<FWorkSpeedStatImportance> statImportances = GetUDatatable(WORKTYPESDATATABLEPATH)->FindRow<FDTS_WorkType>(workType, TEXT(""))->StatImportances;

	//base speed
	float totalSpeed = 100;

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
		int statValue = GetUDatatable(SKILLLEVELSDATATABLEPATH)->FindRow<FDTS_SkillLevel>(FName(*stat), TEXT(""))->Percentage;
		//importance of the skill for the workType - how much the total speed is influenced by the skill
		float importance = statImportances[i].StatImportance;
		
		float totalImpact = 100 - ((100 - statValue) * importance / 100);
		totalSpeed = totalSpeed * (totalImpact / 100);
	}

	return totalSpeed;
}

FText UGlobals::GetStatDisplayText(FSubjectStatValue subjectStatVal)
{
	FDTS_SubjectStat* subjectStat = GetUDatatable(SUBJECTSTATSDATATABLEPATH)->FindRow<FDTS_SubjectStat>(subjectStatVal.StatName, TEXT(""));
	if (subjectStat == nullptr || !subjectStat)
	{
		//could not find this stat in the dt
		return FText::FromString(FString("Could not find subjectStat in datatable: ").Append(subjectStatVal.StatName.ToString()));
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
