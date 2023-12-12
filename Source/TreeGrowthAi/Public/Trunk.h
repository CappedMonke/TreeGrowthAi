// Copyright © 2023 Silas Schuerger, Levin Theil

#pragma once

#include "CoreMinimal.h"

class FBranch;

struct FTreePoint
{
	explicit FTreePoint(const FVector& Location)
		: Location(Location) {}
	
	FVector Location;
	TArray<FVector> Vertices;
	float Radius = 5.0f;
};

class TREEGROWTHAI_API FTrunk
{
public:
	FTrunk();
	~FTrunk();

	void AddTreePoint(const FVector& Location, const FVector& FromLocation);
	
	TArray<FBranch*> Branches;
	TArray<FTreePoint*> TreePoints;
};
