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

