// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


//autogenerated

#include "Lot.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Subject.generated.h" 
//"Should always be the last #include in a header"



UCLASS(abstract)
class WARRIORSANDSERFS_API ASubject : public ACharacter
{
	GENERATED_BODY()
	
	ASubject(const FObjectInitializer& ObjectInitializer);

	//-------------------------------------------------------------------------------
	//Unreal

public:
	// Sets default values for this character's properties
	ASubject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//Unreal
	//--------------------------------------------------------------------------------
	

public:
	float test;

	//DEBUG

	//DEBUG option
	//whether the subject should take damage while their food is on 0.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool damageFromHunger = true;
	//DEBUG option
	//whether the subject should die when their health reaches 0.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool dieFromHealth = true;


	//general

	//TODO
	//Player* player; //which player this subject belongs to.
	
	//current world location target of the subject's moveTo behaviour 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector currentDestinationLocation;
	
	//current target the subject's moveTo behaviour (The Lot they are currently headed towards)
	//eg a serf heading to a warehouse to pick up or drop off an item
	//or a lumberjack heading to the Inn to eat
	//or a militiaman heading to an enemy building to attack it.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ALot* currentDestinationLot;

	//I want to allow players to "name" their Subjects. (Name should be kept across eg transforming from recruit to warrior)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString playerLabel; 

	//what Type of subject this is
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESubjectType subjectType;

	//The cost to educate this subject (in school or keep)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemAmount> subjectEducateCost;
	
	//max health of subject
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SubjectHealth)
	float maxHealth;

	//current health of subject
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = SubjectHealth)
	float currentHealth;

	//how much Health this subject regenerates every second.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SubjectHealth)
	float hpPerSecond;

	//max food of subject
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SubjectHunger)
	float maxFood;

	//current food of subject
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = SubjectHunger)
	float currentFood;

	//how much more hungry this subject gets every second.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SubjectHunger)
	float hungerPerSecond;

	//how much damage to health this subject takes from starvation.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SubjectHunger)
	float hungerDamagePerSecond;



	//functions

	//Sets destination Lot and location (for checking later)
	//Can set destination location without a lot (nullptr) eg for warriors, or wandering around
	UFUNCTION(BlueprintCallable)
	void SetDestination(ALot* lot, FVector location);

	//Update ui that has already spawned
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateActiveUI();

	//Subject heals for x health
	UFUNCTION(BlueprintCallable, Category = SubjectHealth)
	void HealthRegen(float x); 

	//Handles subjects taking damage.
	UFUNCTION(BlueprintCallable, Category = SubjectHealth)
	void TakeDamage(float x);

	//Subject food reduces by x
	UFUNCTION(BlueprintCallable, Category = SubjectHealth)
	void ReduceFood(float x);

	//Handles Subject death.
	//Implemented in Bluprints
	//May be overridden in every level of subjects (civilian, Housebased, Harvester...)
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SubjectDie();


};
