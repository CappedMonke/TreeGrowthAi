// Copyright © 2023 Silas Schuerger, Levin Theil


#include "Branch.h"

// Sets default values
ABranch::ABranch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABranch::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABranch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

