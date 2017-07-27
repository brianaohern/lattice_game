// Fill out your copyright notice in the Description page of Project Settings.

#include "ArenaTile.h"


// Sets default values for this component's properties
AArenaTile::AArenaTile()
{
	tileMesh = CreateDefaultSubobject<UStaticMeshComponent>("TileMesh");

	PrimaryActorTick.bCanEverTick = true;
}

void AArenaTile::Spawn(UWorld* TheWorld)
{
}

void AArenaTile::Clear()
{
	if (IsValid())
	{
		//tileMesh->Destroy();
		tileMesh = NULL;
	}
}

bool AArenaTile::IsValid() const
{
	return (tileMesh != NULL);
}

uint16 AArenaTile::GetHealth()
{
	return Health;
}

void AArenaTile::SetHealth(uint16 newHealth)
{
	Health = newHealth;
}

uint16 AArenaTile::GetHealthRegenRate()
{
	return HealthRegenRate;
}

void AArenaTile::SetHealthRegenRate(uint16 newHealthRegenRate)
{
	HealthRegenRate = newHealthRegenRate;
}

uint16 AArenaTile::GetHealthRegenDelay()
{
	return HealthRegenDelay;
}

void AArenaTile::SetHealthRegenDelay(uint16 newHealthRegenDelay)
{
	HealthRegenDelay = newHealthRegenDelay;
}

void AArenaTile::TakeDamage(uint16 damage)
{
}

// Called when the game starts
void AArenaTile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AArenaTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

