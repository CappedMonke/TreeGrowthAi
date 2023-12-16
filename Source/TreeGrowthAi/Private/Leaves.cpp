// Copyright © 2023 Silas Schuerger, Levin Theil


#include "Leaves.h"


// Sets default values
ALeaves::ALeaves()
{
	PrimaryActorTick.bCanEverTick = true;
}

float ALeaves::GetCollectedEnergy()
{
	const float Energy = CollectedEnergy;
	CollectedEnergy = 0;
	return Energy;
}

void ALeaves::CollectEnergy_Implementation()
{
}

void ALeaves::GainEnergy()
{
	CollectedEnergy = EnergyGain;

	if(DaysWithoutEnergy > 0)
	{
		DaysWithoutEnergy--;
	}
}

void ALeaves::Decay()
{
	DaysWithoutEnergy++;

	if (DaysWithoutEnergy > MaxDaysWithoutEnergy)
	{
		Destroy();
	}
}
