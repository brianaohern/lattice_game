// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArenaTile.h"
#include <vector>
#include "Arena.generated.h"

USTRUCT(BlueprintType)
struct FArenaGridRow
{
	GENERATED_BODY()

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

USTRUCT(BlueprintType)
struct FArenaGrid
{
	GENERATED_BODY()

	int32 x;
	int32 y;

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
		for (int i = 0; i < x; i++)
		{
			AddNewRow();
		}

		// Add columns
		for (int i = 0; i < x; i++)
		{
			for (int j = 0; j < y; j++)
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

	FArenaGrid()
	{
		x = 20;
		y = 20;
	}

	FArenaGrid(uint16 x_in, uint16 y_in)
	{
		x = x_in;
		y = y_in;
	}
};

UCLASS()
class LATTICEGAME_API AArena : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArena();
	AArenaTile* AddTile(FVector Location);
	void DestroyTile();

	UFUNCTION(BlueprintNativeEvent)
	void StartBuild(int32 x, int32 y, int32 z);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Grid")
	TArray<FArenaGrid> arenaStack;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
