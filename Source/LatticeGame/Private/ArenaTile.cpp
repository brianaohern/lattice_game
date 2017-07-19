// Fill out your copyright notice in the Description page of Project Settings.

#include "ArenaTile.h"


// Sets default values for this component's properties
AArenaTile::AArenaTile()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryActorTick.bCanEverTick = true;
}

uint16 AArenaTile::getHealth()
{
	return health;
}

void AArenaTile::setHealth(uint16 newHealth)
{
	health = newHealth;
}

uint16 AArenaTile::getHealthRegenRate()
{
	return healthRegenRate;
}

void AArenaTile::setHealthRegenRate(uint16 newHealthRegenRate)
{
	healthRegenRate = newHealthRegenRate;
}

uint16 AArenaTile::getHealthRegenDelay()
{
	return healthRegenDelay;
}

void AArenaTile::setHealthRegenDelay(uint16 newHealthRegenDelay)
{
	healthRegenDelay = newHealthRegenDelay;
}

void AArenaTile::takeDamage(uint16 damage)
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

