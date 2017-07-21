// Fill out your copyright notice in the Description page of Project Settings.

#include "Arena.h"
#include <vector>

USTRUCT()
struct FArenaGridRow
{
	TArray<AArenaTile*> Y; // Column (Actual Tile in Row)

	// Every row has ArenaY number of columns
	void AddNewColumn()
	{
		AArenaTile* newTile = new AArenaTile();
		Y.Add(newTile);
	}

	FArenaGridRow()
	{
	}
};

USTRUCT()
struct FArenaGrid
{
	uint16 x;
	uint16 y;

	TArray<FArenaGridRow> X; // Rows

	// Every grid has ArenaX number of rows
	// AddRows is used to initialize all rows in grid
	void AddNewRow()
	{
		X.Add(FArenaGridRow());
	}

	void BuildArena(UWorld* World)
	{
		// Add rows
		for(int i = 0; i < x; i++)
		{
			AddNewRow();
		}

		// Add columns
		for (int i = 0; i < x; i++)
		{
			for(int j = 0; j < y; j++)
				X[i].AddNewColumn();
		}

		// Spawn Tiles
		if (X.Num() > 0)
		{
			for (int i = 0; i < X.Num(); i++)
			{
				for (int j = 0; j < X[i].Y.Num(); j++)
				{
					X[i].Y[j]->Spawn(World);
				}
			}
		}
	}

	FArenaGrid(uint16 x_in, uint16 y_in)
	{
		x = x_in;
		y = y_in;
	}
};

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
	TArray<FArenaGrid> arenaStack;
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

