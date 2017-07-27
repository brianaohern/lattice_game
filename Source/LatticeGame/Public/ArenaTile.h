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

	uint16 Health;
	uint16 CurrentHealth;
	uint16 HealthRegenRate;
	uint16 HealthRegenDelay;
	
	UPROPERTY()
	FVector Location;
	
	UPROPERTY()
	FRotator Rotation;

	UPROPERTY()
	FVector Scale;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* tileMesh;

private:

	/** FX component */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UParticleSystemComponent* PickupPSC;

protected:

	/** ambient FX on tile */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UParticleSystem* AmbientFX;

	/** FX shown when destroyed */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UParticleSystem* DestroyFX;

	/** ambient droning sound */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	USoundCue* AmbientSound;

	/** sound played when destroyed */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	USoundCue* DestroySound;

public:	
	// Sets default values for this component's properties
	AArenaTile();

	UFUNCTION(BlueprintCallable)
	void Spawn(UWorld* TheWorld);

	uint16 GetHealth();
	void SetHealth(uint16 newHealth);

	uint16 GetHealthRegenRate();
	void SetHealthRegenRate(uint16 newHealthRegenRate);

	uint16 GetHealthRegenDelay();
	void SetHealthRegenDelay(uint16 newHealthRegenDelay);

	void TakeDamage(uint16 damage);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void Clear();
	bool IsValid() const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
