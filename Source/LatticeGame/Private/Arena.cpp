// Fill out your copyright notice in the Description page of Project Settings.

#include "Arena.h"
#include <vector>

USTRUCT()
struct FArenaGridColumn
{
	TArray<AArenaTile*> tiles;

	// Every column has ArenaX number of stacks
	void AddNewTile()
	{
		AArenaTile* newTile = new AArenaTile();
		tiles.Add(newTile);
	}

	FArenaGridColumn()
	{
	}
};

USTRUCT()
struct FArenaGridRow
{
	TArray<FArenaGridColumn> tileColumns;

	// Every row has ArenaY number of columns
	void AddNewColumn()
	{
		tileColumns.Add(FArenaGridColumn());
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
	uint16 z;

	TArray<FArenaGridRow> tileRows;

	// Every grid has ArenaX number of rows
	// AddRows is used to initialize all rows in grid
	void AddNewRow()
	{
		tileRows.Add(FArenaGridRow());
	}

	void BuildArena()
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
				tileRows[i].AddNewColumn();
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

	TArray<FArenaGrid> arenaStack;

	for (int i = 0; i < ArenaZ; i++)
	{
		arenaStack.Add(FArenaGrid(ArenaX, ArenaY));
		arenaStack[i].BuildArena();
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

