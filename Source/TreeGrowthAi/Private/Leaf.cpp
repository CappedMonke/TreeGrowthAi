// Copyright © 2023 Silas Schuerger, Levin Theil


#include "Leaf.h"

// Sets default values
ALeaf::ALeaf()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALeaf::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALeaf::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

