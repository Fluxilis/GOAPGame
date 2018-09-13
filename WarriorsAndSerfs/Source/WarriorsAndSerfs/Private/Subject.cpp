// Fill out your copyright notice in the Description page of Project Settings.

#include "Subject.h"

#include "UnrealNetwork.h"

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
