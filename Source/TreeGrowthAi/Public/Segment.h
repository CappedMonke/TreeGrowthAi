// Copyright © 2023 Silas Schuerger, Levin Theil

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Segment.generated.h"

class ATree;
class ALeaves;

UCLASS()
class TREEGROWTHAI_API USegment : public UObject
{
	GENERATED_BODY()

public:
	void Setup(ATree* TreeIn, USegment* LastSegment, const FVector& ToLocation, const int ID, float InitEnergy);
	void BranchOff(const bool ShouldBranchOff, const FVector& GrowthDirection, const FVector& BranchGrowthDirection, bool SecondBranch);
	void GrowLeaves(const bool ShouldGrowLeaves);
	void GrowSegment(bool ShouldGrow, const FVector& GrowthDirection);
	void ShareEnergy();
	void Grow();
	void DeleteToSegments();

	UPROPERTY()
	ATree* Tree;
	
	UPROPERTY()
	USegment* FromSegment = nullptr;

	UPROPERTY()
	TArray<USegment*> ToSegments;
	
	UPROPERTY()
	ALeaves* Leaves = nullptr;

	UPROPERTY()
	FVector End;

	UPROPERTY()
	FVector Start;
	
	int Index; // Multiple Segments can have the same Index. It gives us the number of Segments before this.
	float Energy;
	float Radius = 0.3f;
	float Height;
	float MaxDistanceToTrunk;
	int DaysWithoutEnergy = 0;
	bool CanGrowLeaves = true;

	TArray<FVector> VerticesStart;
	TArray<FVector> VerticesEnd;
};
