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
};

class TREEGROWTHAI_API FTrunk
{
public:
	explicit FTrunk(int Energy);
	~FTrunk();

	void AddTreePoint(const FVector& Location, const FVector& FromLocation, const float Radius);
	void Grow();
	void GrowBranch();
	
	TArray<FBranch*> Branches;
	TArray<FTreePoint*> TreePoints;

	int Energy = 100;
	int DailyEnergyConsumption = 0;
	int Id;
};
