// Copyright © 2023 Silas Schuerger, Levin Theil


#include "TreeData.h"


FSegment::FSegment(FSegment* FromSegment, const FVector& ToLocation, const int ID, const float InitEnergy)
	: Index(ID), Energy(InitEnergy)
{
	if(FromSegment)
	{
		this->FromSegment = FromSegment;
		Start = FromSegment->End;
	}
	else
	{
		Start = FVector::ZeroVector;
	}
	End = Start + ToLocation * GrowthLengthMulitplier;
	Height = End.Z;

	FTreeData::Instance().AllSegments.Add(this);
	FTreeData::Instance().NewSegments.Add(this);
}

void FSegment::GrowSegment(const bool ShouldGrow, const FVector& GrowthDirection)
{
	if(!ShouldGrow) return;
	if (Energy - SegmentCost < 0) return;

	FTreeData::Instance().NewSegments.Remove(this);
	ToSegments.Add(new FSegment(this, GrowthDirection.GetSafeNormal(), Index + 1, Energy - SegmentCost));
	Energy = 0;
}

void FSegment::BranchOff(const bool ShouldBranchOff, const FVector& GrowthDirection, const FVector& BranchGrowthDirection)
{
	if(!ShouldBranchOff) return;
	if (Energy - SegmentCost - BranchCost < 0) return;

	// TODO: Handle that they dont spawn too close to each other
	
	FTreeData::Instance().NewSegments.Remove(this);
	ToSegments.Add(new FSegment(this, GrowthDirection.GetSafeNormal(), Index + 1, Energy / 2 - SegmentCost)); // Segment
	ToSegments.Add(new FSegment(this, BranchGrowthDirection.GetSafeNormal(), Index + 1, Energy / 2 - BranchCost)); // Branch
	Energy = 0;
}

void FSegment::GrowLeaves(const bool ShouldGrowLeaves)
{
	
}

void FSegment::Grow()
{
	Energy -= Radius * DailyCostMultiplier;

	if(Energy < 0)
	{
		DaysWithoutEnergy++;
		if (DaysWithoutEnergy > MaxDaysWithoutEnergy)
		{
			DeleteToSegments();
		}
		Energy = 0;
	}
	else
	{
		DaysWithoutEnergy = 0;
	}
}

void FSegment::DeleteToSegments()
{
	for (const auto& Segment : ToSegments)
	{
		Segment->DeleteToSegments();
		FTreeData::Instance().RemoveSegmentFromAll(Segment);
	}
	FTreeData::Instance().RemoveSegmentFromAll(this);
}


