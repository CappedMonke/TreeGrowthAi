// "// Copyright © 2023 Silas Schuerger, Levin Theil"

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Branch.generated.h"

UCLASS()
class TREEGROWTHAI_API ABranch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABranch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
