// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"

#include "CoreMinimal.h"
#include "Globals.generated.h"

#define UPDATEUIIDHEALTH TEXT("Health")
#define UPDATEUIIDFOOD TEXT("Food")
#define UPDATEUIIDALL TEXT("All")
#define UPDATEUIIDALLITEMS TEXT("AllItems")

#define SUBJECTSDATATABLEPATH "DataTable'/Game/WnSAssets/Data/Subjects/DT_Subjects.DT_Subjects'"
#define OCCUPATIONSDATATABLEPATH "DataTable'/Game/WnSAssets/Data/Occupations/DT_Occupations.DT_Occupations'"
#define SUBJECTSTATSDATATABLEPATH "DataTable'/Game/WnSAssets/Data/SubjectStats/DT_SubjectStats.DT_SubjectStats'"
#define WORKTYPESDATATABLEPATH "DataTable'/Game/WnSAssets/Data/WorkTypes/DT_WorkTypes.DT_WorkTypes'"
#define SKILLLEVELSDATATABLEPATH "DataTable'/Game/WnSAssets/Data/SkillLevels/DT_SkillLevels.DT_SkillLevels'"

/**
 * 
 */

UENUM(BlueprintType)
enum class ELotType : uint8
{
	MissingNo, Debug, //Debug
	Building, Constructionsite, DeadDrop //Different types of lots (may have additional specification, eg building type)
};


UENUM(BlueprintType)
enum class EHarvestingJobType : uint8
{
	None, Planting, Returning, Tending, Harvesting, ReturningWithHarvest, Polishing, Resting
};

UENUM(BlueprintType)
enum class ECraftingJobType : uint8
{
	None, Crafting, Resting
};

//a simple struct that contains an item type and an int amount.
//used eg in Recipes
USTRUCT(BlueprintType)
struct FItemAmount
{
	GENERATED_BODY()

public:
	FItemAmount();

	//what item
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName itemType;

	//amount of items
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int amount;
};

//a simple struct that contains an FName and float.
USTRUCT(BlueprintType)
struct FFloatName
{
	GENERATED_BODY()

public:
	FFloatName();
	FFloatName(FName n, float f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float value;

	FORCEINLINE bool operator<(const FFloatName& other) const { return value < other.value; };
};

//How important this Stat is for the workspeed
USTRUCT(BlueprintType)
struct FStatImportance
{
	GENERATED_BODY()

public:
	FStatImportance();

	//stat name (must match a stat of SubjectStats)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName StatName;

	//stat value
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StatImportance;
};

//a simple struct that contains an EHarvestingJobType and bool - wether a harvester does this jobtype or not. (eg foresters do not harvest or tend or polish, they only plant.)
USTRUCT(BlueprintType)
struct FHarvestJobBool
{
	GENERATED_BODY()

public:
	FHarvestJobBool();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EHarvestingJobType harvestingJobType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool doesJob;
};

//The different stats a subject can have (attributes(?), skills, other(?)
USTRUCT(BlueprintType)
struct FDTS_SubjectStat: public FTableRowBase
{
	GENERATED_BODY()

public:
	FDTS_SubjectStat();

	//stat name
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName StatName;

	//where in Lists (eg subject UI) this stat should be displayed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int OrderInLists;

	//stat display name
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;

	//player-visible description of the stat
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	//identifier how this stat is displayed - will be asked by UI and handled there
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DisplayType;

	//identifier for how the initial value for this stat is calculated
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName InitialValue;
};

//The SubjectStat's Value
USTRUCT(BlueprintType)
struct FSubjectStatValue
{
	GENERATED_BODY()

public:
	FSubjectStatValue();
	FSubjectStatValue(FName _statName, FString _statValue);

	//stat name
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName StatName;

	//stat value
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString StatValue;
};


//struct for item datatables
USTRUCT(BlueprintType)
struct FDTS_Occupation : public FTableRowBase
{
	GENERATED_BODY()

public:
	FDTS_Occupation();

	//occupation name
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName OccupationName;

	//what the player sees as Occupation Name / Job Title
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;

	//where in lists (eg stats?) this Occupation should be displayed
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

	//name of the blueprint class asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString BlueprintClass;

	//what items this occupation costs to educate (from a default / thin air)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemAmount> EducateCost;

	//which subject stats are recommended for this occupation and how much they factor in (relevant for choosing occupations)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FStatImportance> SuggestedSkills;

	//in case of a harvesting occupation - which of the harvestingJobTypes does the occupation include? (Plant, Tend, Harvest, Polish)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FHarvestJobBool> HarvesterJobTypes;
};



//struct for work datatable 
USTRUCT(BlueprintType)
struct FDTS_WorkType : public FTableRowBase
{
	GENERATED_BODY()

public:
	FDTS_WorkType();

	//work name 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName WorkTypeName;

	//what the player sees as Work Title
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;

	//help text the player can read (eg tooltip, info button)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText HelpText;

	//how long this workType takes to execute at factor 100%
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseDuration;

	//stat value
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FStatImportance> StatImportances;
};

//struct for skillLevels datatable (this applies to all skills)
USTRUCT(BlueprintType)
struct FDTS_SkillLevel: public FTableRowBase
{
	GENERATED_BODY()

public:
	FDTS_SkillLevel();

	//skill level 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName SkillLevel;

	//Workspeed percentage
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Percentage;

	//Title - sounds cool, makes the citizen feel important
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Title;

	//help text the player can read (eg tooltip, info button)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText HelpText;
};

UCLASS(Blueprintable)
class BURGHERS_API UGlobals : public UObject
{

	GENERATED_BODY()

public:
	UGlobals();
	~UGlobals();

	//returns the (player-side) name of the Lot
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FString GetLotType(ELotType lot);

	//returns the (player-side) name of the job
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FString GetHarvestingJobType(EHarvestingJobType job);

	//returns the (player-side) name of the job
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FString GetCraftingJobType(ECraftingJobType job);

	//returns the datatable at the specified path
	UFUNCTION(BlueprintCallable)
	static UDataTable* GetUDatatable(const FString& Path);

	//Calculates workspeed of a certain workType with a certain skill/statset (ie for a certain citizen)
	UFUNCTION(BlueprintCallable)
	static float CalculateWorkDuration(TMap<FName, FSubjectStatValue> statsMap, FName workType, float overrideBaseSpeed = -1000);

	//Calculates suitability for a certain occupation with a certain skill/statset (ie for a certain citizen)
	UFUNCTION(BlueprintCallable)
	static float CalculateOccupationSuitability(TMap<FName, FSubjectStatValue> statsMap, FName occupationType);

	//returns a Text with statValue, formatted in the way of this subjectStat
	UFUNCTION(BlueprintCallable)
	static FText GetStatDisplayText(FSubjectStatValue subjectStatVal);

	//returns a new array, sorted by value of the float
	UFUNCTION(BlueprintCallable)
	static TArray<FFloatName> SortFloatNameArray(TArray<FFloatName> arrayToSort);
};






