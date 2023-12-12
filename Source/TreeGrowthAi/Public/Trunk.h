// Copyright © 2023 Silas Schuerger, Levin Theil

#pragma once

#include "CoreMinimal.h"

class FBranch;

struct FTreePoint
{
	explicit FTreePoint(const FVector& Location, const float Radius)
		: Location(Location), Radius(Radius) {}
	
	FVector Location;
	TArray<FVector> Vertices;
	float Radius;
	bool CanGrowBranch = true;
};

class TREEGROWTHAI_API FTrunk
{
public:
	explicit FTrunk(int InitEnergy);
	virtual ~FTrunk();

	void Setup();
	void AddTreePoint(const FVector& Location, const FVector& FromLocation);
	virtual void Grow(const FVector& Direction, int EnergyIn);
	void GrowBranch(const int EnergyCost, FTreePoint* TreePoint, int Vertex);
	
	TArray<FBranch*> Branches;
	TArray<FTreePoint*> TreePoints;

	int Energy = 100;
	float BaseGrowthRate = 0.002f;
	float GrowBranchMaxRadius = 10;
	int DailyEnergyConsumption = 0;
	int Id;
	FTrunk* FromBranch = nullptr;
};
