// Fill out your copyright notice in the Description page of Project Settings.

#include "Arena.h"

// Sets default values
AArena::AArena()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AArena::BeginPlay()
{
	Super::BeginPlay();
	
	// Generate Arena
	// Get Voted Arena Size
	uint8 ArenaX = 30; // TODO: Temp size listed, should get from voting
	uint8 ArenaY = 30; // TODO: Temp size listed, should get from voting
	uint8 ArenaZ = 1;  // TODO: Temp size listed, should get from voting

	UWorld* World = GetWorld(); // TODO: TEMP

	// Build a separate arena for each layer (ArenaZ)
	for (int i = 0; i < ArenaZ; i++)
	{
		arenaStack.Add(FArenaGrid(ArenaX, ArenaY));
		arenaStack[i].BuildArena(World);
	}
}

void AArena::DestroyTile()
{
}

AArenaTile* AArena::AddTile(FVector Location)
{
	return new AArenaTile;
}

// Called every frame
void AArena::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

