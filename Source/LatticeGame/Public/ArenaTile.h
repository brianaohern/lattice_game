// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkeletalMeshTypes.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "ArenaTile.generated.h"

UCLASS()
class LATTICEGAME_API AArenaTile : public AActor
{
	GENERATED_BODY()

protected:

	/** Maximum health a tile can have */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float MaxHealth;

	/** Current health tile has (between 0 and Health) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float CurrentHealth;

	/** Number of health regeneration ticks per second */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float HealthRegenRate;

	/** Number of seconds until a tile starts regenerating after being damaged */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float HealthRegenDelay;

	/** Time elapsed since last damage taken (used for regen purposes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float HealthRegenTimerElapsed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	bool canRegen;
	
	/** World space location of tile mesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Placement)
	FVector Location;
	
	/** World space rotation of tile mesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Placement)
	FRotator Rotation;

	/** World space scale of tile mesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Placement)
	FVector Scale;

	/** Mesh to be used by tile */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* tileMesh;

	/** Ambient FX on tile */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UParticleSystem* AmbientFX;

	/** FX shown when destroyed */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UParticleSystem* DestroyFX;

	/** Ambient droning sound */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	USoundCue* AmbientSound;

	/** Sound played when destroyed */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	USoundCue* DestroySound;

	/** Called when the game starts */
	virtual void BeginPlay() override;

	/** Removes tile from memory */
	void Clear();

	/** Check that reference to tile is not null */
	bool IsValid() const;

private:

	/** FX component */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UParticleSystemComponent* PickupPSC;

public:	
	
	/** Sets default values for component's properties */
	AArenaTile();

	/** Allocate memory for tile and put mesh and collision into world */
	UFUNCTION(BlueprintCallable)
	void Spawn(UWorld* TheWorld);

	/** Called every frame */
	virtual void Tick(float DeltaTime) override;
};
