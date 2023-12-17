// Copyright © 2023 Silas Schuerger, Levin Theil

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Leaves.generated.h"

UCLASS()
class TREEGROWTHAI_API ALeaves : public AActor
{
	GENERATED_BODY()

public:
	ALeaves();
	float GetCollectedEnergy();

	UFUNCTION(BlueprintNativeEvent)
	void CollectEnergy();

	UFUNCTION(BlueprintCallable)
	void GainEnergy();

	UFUNCTION(BlueprintCallable)
	void Decay();
	
	float EnergyGain = 0;

private:
	static constexpr int MaxDaysWithoutEnergy = 5;
	int DaysWithoutEnergy = 0;
	float CollectedEnergy = 0;
};
