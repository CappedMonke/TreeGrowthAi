// "// Copyright © 2023 Silas Schuerger, Levin Theil"


#include "Trunk.h"

// Sets default values
ATrunk::ATrunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATrunk::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

