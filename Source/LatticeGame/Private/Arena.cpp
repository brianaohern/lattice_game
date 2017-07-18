// Fill out your copyright notice in the Description page of Project Settings.

#include "Arena.h"
#include <vector>

USTRUCT()
struct FArenaGridStack
{
	//TArray<UArenaTile> tiles;

	// Every stack has ArenaZ number of tiles
	void AddTile()
	{
		//tiles.Add(UArenaTile());
	}

	FArenaGridStack()
	{
	}
};

USTRUCT()
struct FArenaGridColumn
{
	TArray<FArenaGridStack> tileStacks;

	// Every column has ArenaX number of stacks
	void AddStack()
	{
		// Add a new stack for each column
		tileStacks.Add(FArenaGridStack());
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
	void AddColumn()
	{
		// Add a column to each row
		tileColumns.Add(FArenaGridColumn());
	}

	FArenaGridRow()
	{
	}
};

USTRUCT()
struct FArenaGrid
{
	TArray<FArenaGridRow> tileRows;

	// Every grid has ArenaX number of rows
	void AddRow()
	{
		tileRows.Add(FArenaGridRow());
	}

	FArenaGrid()
	{

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

	FArenaGrid arenaGrid;
}





void AArena::DestroyTile()
{

}

// Called every frame
void AArena::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

