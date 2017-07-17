// Fill out your copyright notice in the Description page of Project Settings.

#include "Arena.h"
#include <vector>


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
	uint8 ArenaX = 30;
	uint8 ArenaY = 30;
	uint8 ArenaZ = 1;

	std::vector<std::vector<std::vector<int>>> intVector; // TODO: Should be of type ArenaTile

	// Initialize Vector for Arena's Floor
	for (int x = 0; x < ArenaX; x++)
	{
		intVector.push_back(std::vector<std::vector<int>>());
		for (int y = 0; y < ArenaY; y++)
		{
			intVector[x].push_back(std::vector<int>());
			for (int z = 0; z < ArenaZ; z++)
			{
				int tile = 0;
				intVector[x][y].push_back(tile);
			}
		}
	}

}

void AArena::DestroyTile()
{

}

// Called every frame
void AArena::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

