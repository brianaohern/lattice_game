// Fill out your copyright notice in the Description page of Project Settings.

#include "ArenaTile.h"


// Sets default values for this component's properties
UArenaTile::UArenaTile()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UArenaTile::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UArenaTile::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

