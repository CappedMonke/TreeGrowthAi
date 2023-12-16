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
	void BranchOff(const bool ShouldBranchOff, const FVector& GrowthDirection, const FVector& BranchGrowthDirection);
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
	float Radius = 1.0f;
	float Height;
	float MaxDistanceToTrunk; // TODO: Branches should not grow infinitely to the side, maximal the height from Branchpoint to top or bottom (whatever is shorter)
	int DaysWithoutEnergy = 0;
	bool CanGrowLeaves = true;

	static constexpr int MaxDaysWithoutEnergy = 7;
	static constexpr float DailyCostMultiplier = 1.4f;
	
	static constexpr float SegmentCost = 15.0f;
	static constexpr float BranchCost = 40.0f;
	static constexpr float LeavesCost = 60.0f;

	static constexpr float TreeTotalEnergyMultiplier = 0.4;
	static constexpr float GrowthLengthMulitplier = 10.0f;
	static constexpr float GrowthRadiusMultiplier = 0.0001f;
	static constexpr float MinGrowthAngle = 0.0f;
	static constexpr float MaxGrowthAngle = 20.0f;
	static constexpr float MinBranchAngle = 20.0f;
	static constexpr float MaxBranchAngle = 120.0f;
};
