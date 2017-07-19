// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArenaTile.generated.h"

UCLASS()
class LATTICEGAME_API AArenaTile : public AActor
{
	GENERATED_BODY()

	uint16 health;
	uint16 healthRegenRate;
	uint16 healthRegenDelay;
	
	UPROPERTY()
	FVector Location;

public:	
	// Sets default values for this component's properties
	AArenaTile();

	uint16 getHealth();
	void setHealth(uint16 newHealth);

	uint16 getHealthRegenRate();
	void setHealthRegenRate(uint16 newHealthRegenRate);

	uint16 getHealthRegenDelay();
	void setHealthRegenDelay(uint16 newHealthRegenDelay);

	void takeDamage(uint16 damage);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
