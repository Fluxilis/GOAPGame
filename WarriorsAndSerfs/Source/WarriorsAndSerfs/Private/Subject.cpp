// Fill out your copyright notice in the Description page of Project Settings.

#include "Subject.h"

#include "UnrealNetwork.h"


//TODO REMOVE

//print to screen macro
#define printCritical(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 600.0, FColor::Red,text)

// Sets default values
ASubject::ASubject()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


ASubject::ASubject(const FObjectInitializer& ObjectInitializer)	: Super(ObjectInitializer)
{
	bReplicates = true;
	bReplicateMovement = true;
	bNetLoadOnClient = true;

	strength = GetRandomAttributeValue();
	dexterity = GetRandomAttributeValue();
	constitution = GetRandomAttributeValue(); 
	intelligence = GetRandomAttributeValue();
	wisdom = GetRandomAttributeValue();
	charisma = GetRandomAttributeValue();

	UDataTable* statsTable = GetSubjectStatsDatatable();
	TArray<FName> rowNames = statsTable->GetRowNames();
	for (int i = 0; i < rowNames.Num(); i++)
	{
		statsMap.Add(rowNames[i], FSubjectStatValue(rowNames[i], InitSubjectStat(rowNames[i])));
	}
}

void ASubject::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASubject, currentHealth);
	DOREPLIFETIME(ASubject, currentFood);
}

// Called when the game starts or when spawned
void ASubject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASubject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



	float foodToReduce = hungerPerSecond * DeltaTime;
	ReduceFood(foodToReduce);

	if (currentFood <= 0)
	{
		if (damageFromHunger)
		{
			TakeDamage((hungerDamagePerSecond * DeltaTime));
		}
	}

	if (currentFood > 0)
	{
		float healthToReg = hpPerSecond * DeltaTime;
		HealthRegen(healthToReg);
	}

	UpdateActiveUI();
}


//Sets destination Lot and location (for checking later)
//Can set destination location without a lot (nullptr) eg for warriors, or wandering around
void ASubject::SetDestination(ALot* lot, FVector location)
{
	currentDestinationLot = lot;

	currentDestinationLocation = location;
}

void ASubject::HealthRegen(float x)
{
	//this is more likey to be true than false, and I can save a little processing power (with the UpdateUI!)
	if (currentHealth == maxHealth)
	{
		return;
	}

	currentHealth += x;

	if (currentHealth > maxHealth)
	{
		currentHealth = maxHealth;
	}

	UpdateActiveUI();
}

void ASubject::TakeDamage(float x)
{
	currentHealth -= x;

	if (currentHealth <= 0)
	{
		SubjectDie();
	}
	else
	{
		UpdateActiveUI();
	}
}

void ASubject::ReduceFood(float x)
{
	currentFood -= x;

	if (currentFood < 0)
	{
		currentFood = 0;
	}

}

UDataTable* ASubject::GetOccupationsDatatable()
{
	return UGlobals::GetUDatatable(OCCUPATIONSDATATABLEPATH);
	//return UGlobals::GetUDatatable("DataTable'/Game/WnSAssets/Data/Subjects/DT_Subjects.DT_Subjects'");
}

UDataTable* ASubject::GetSubjectStatsDatatable()
{
	return UGlobals::GetUDatatable(SUBJECTSTATSDATATABLEPATH);
	//return UGlobals::GetUDatatable("DataTable'/Game/WnSAssets/Data/Subjects/DT_Subjects.DT_Subjects'");
}


FName ASubject::ChooseOccupation_Implementation()
{
	UDataTable* occupationsDataTable = GetOccupationsDatatable();
	
	if (occupationsDataTable == nullptr || !occupationsDataTable)
	{
		printCritical("Subject.cpp ChooseOccupation did not find Subject Datatable!");
	}

	int random = FMath::RandRange(0, occupationsDataTable->GetRowNames().Num() - 1);	
	return occupationsDataTable->GetRowNames()[random];
}

TArray<FName> ASubject::GetFavouredOccupations()
{
	return GetOccupationsDatatable()->GetRowNames();
}


int ASubject::GetRandomAttributeValue()
{
	//3d5, for an average value around 9 (max is 15, so there's room to grow!)
	return FMath::RandRange(1, 5) + FMath::RandRange(1, 5) + FMath::RandRange(1, 5);
}


FString ASubject::InitSubjectStat(FName statName)
{
	///would be more performant if this code was in the constructor itself.

	UDataTable* statsTable = GetSubjectStatsDatatable();
	FDTS_SubjectStat* stats = statsTable->FindRow<FDTS_SubjectStat>(statName, TEXT(""));

	if (stats->InitialValue == FName(TEXT("Zero")))
	{
		return "0";
	}
	if (stats->InitialValue == FName(TEXT("pickGender")))
	{
		return FMath::RandRange(0, 1) == 0 ? "Male" : "Female";
	}
	if (stats->InitialValue == FName(TEXT("3d5")))
	{
		return FString::FromInt(GetRandomAttributeValue());
	}

	FString failedString = "Initalising Value Failed: ";
	failedString.Append(stats->InitialValue.ToString());
	return failedString;
}