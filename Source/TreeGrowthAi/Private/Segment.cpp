// Copyright © 2023 Silas Schuerger, Levin Theil


#include "TreeData.h"


FSegment::FSegment(const FSegment* FromSegment, const FVector& ToLocation, const int ID, const float InitEnergy)
	: Index(ID), Energy(InitEnergy)
{
	if(FromSegment)
	{
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

void FSegment::BranchOff(const bool ShouldBranchOff, const FVector& Direction)
{
	if(!ShouldBranchOff) return;
	if (Energy - SegmentCost - BranchCost < 0) return;

	FTreeData::Instance().NewSegments.Remove(this);
}

void FSegment::GrowLeaves(const bool ShouldGrowLeaves)
{
	
}

void FSegment::GrowSegment(const bool ShouldGrow, const FVector& Direction)
{
	if(!ShouldGrow) return;
	if (Energy - SegmentCost < 0) return;
	
	FTreeData::Instance().NewSegments.Remove(this);
	ToSegments.Add(new FSegment(this, Direction, Index + 1, Energy));
	Energy = 0;
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


