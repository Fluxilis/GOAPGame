// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Subject.h"
#include "Lot.h"


#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "OccupationManager.generated.h"


//suitability Score of the subject for a certain jobtype
USTRUCT(BlueprintType)
struct FSubjectSuitabilityScore
{
	GENERATED_BODY()

public:
	FSubjectSuitabilityScore();
	FSubjectSuitabilityScore(ASubject* theSubject, float theScore);

	//reference to subject
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ASubject* subject;

	//score value
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float score;
};

//List of subjectscores per building - for use in Occupation manager, keeping track of multiple buildings that want new employees.
USTRUCT(BlueprintType)
struct FBuildingSubjectScores
{
	GENERATED_BODY()

public:
	FBuildingSubjectScores();

	//reference to subject
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ALot* building;

	//score value
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FSubjectSuitabilityScore> subjectScores;
};




/**
 * 
 */
UCLASS(Blueprintable)
class BURGHERS_API AOccupationManager : public AInfo
{
	GENERATED_BODY()
	
	//	TArray<FBuildingSubjectScores> 
	
public:
	AOccupationManager();
	
};
