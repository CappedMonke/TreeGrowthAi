// Copyright © 2023 Silas Schuerger, Levin Theil

#include "Segment.h"

#include "Leaves.h"
#include "Tree.h"


void USegment::Setup(ATree* TreeIn, USegment* LastSegment, const FVector& ToLocation, const int ID, float InitEnergy)
{
	this->Tree = TreeIn;
	this->Index = ID;
	this->Energy = InitEnergy;
	
	if(LastSegment)
	{
		this->FromSegment = LastSegment;
		Start = FromSegment->End;
	}
	else
	{
		Start = FVector::ZeroVector;
	}
	End = Start + ToLocation * GrowthLengthMulitplier;
	Height = End.Z;
		
	Tree->AllSegments.Add(this);
	Tree->NewSegments.Add(this);
}

void USegment::GrowSegment(const bool ShouldGrow, const FVector& GrowthDirection)
{
	if(!ShouldGrow) return;
	if (Energy - SegmentCost < 0) return;

	Tree->NewSegments.Remove(this);
	CanGrowLeaves = false;
	
	USegment* SegmentObj = NewObject<USegment>();
	SegmentObj->Setup(Tree, this, GrowthDirection.GetSafeNormal(), Index + 1, Energy / 2 - SegmentCost);
	ToSegments.Add(SegmentObj);

	Energy = Energy / 2;
}

// Is only called for Segments with leaves
void USegment::ShareEnergy()
{
	if (!Leaves)
	{
		Tree->AllLeaves.Remove(nullptr);
		Tree->LeavesSegments.Remove(this);
		return;
	}
	
	const float SharedEnergy = Leaves->GetCollectedEnergy() / Index;;
	USegment* Segment = this;
	while (Segment)
	{
		Segment->Energy += SharedEnergy;
		Segment = Segment->FromSegment;
	}
}

void USegment::BranchOff(const bool ShouldBranchOff, const FVector& GrowthDirection, const FVector& BranchGrowthDirection)
{
	if(!ShouldBranchOff) return;
	if (Energy - BranchCost < 0) return;

	// TODO: Handle that they dont spawn too close to each other
	
	Tree->NewSegments.Remove(this);
	CanGrowLeaves = false;
	
	USegment* SegmentObj = NewObject<USegment>();
	SegmentObj->Setup(Tree, this, GrowthDirection.GetSafeNormal(), Index + 1, (Energy - BranchCost) / 4);
	ToSegments.Add(SegmentObj); // Segment
	
	SegmentObj = NewObject<USegment>();
	SegmentObj->Setup(Tree, this, BranchGrowthDirection.GetSafeNormal(), Index + 1, (Energy - BranchCost) / 4);
	ToSegments.Add(SegmentObj); // Branc

	Energy = (Energy - BranchCost) / 2;
}

void USegment::GrowLeaves(const bool ShouldGrowLeaves)
{
	if(!ShouldGrowLeaves || !CanGrowLeaves) return;
	if (Energy - LeavesCost < 0) return;

	Energy -= LeavesCost;
	CanGrowLeaves = false;
	Leaves = Tree->GetWorld()->SpawnActor<ALeaves>(Tree->LeavesClass, End, FRotator::ZeroRotator);
	Tree->LeavesSegments.Add(this);
	Tree->AllLeaves.Add(Leaves);
}

void USegment::Grow()
{
	if(Energy - Radius * DailyCostMultiplier < 0)
	{
		DaysWithoutEnergy++;
		if (DaysWithoutEnergy > MaxDaysWithoutEnergy)
		{
			DeleteToSegments();
		}
		Energy = 0;
		return;
	}

	if (DaysWithoutEnergy > 0)
	{
		DaysWithoutEnergy--;
	}
	Energy -= Radius * DailyCostMultiplier;
	Radius += Energy * GrowthRadiusMultiplier;
	const float EnergyToSpend = Energy * TreeTotalEnergyMultiplier;
	Tree->TreeEnergy += EnergyToSpend;
	Energy -= EnergyToSpend;
}

void USegment::DeleteToSegments()
{
	for (const auto& Segment : ToSegments)
	{
		Segment->DeleteToSegments();
		Tree->RemoveSegment(Segment);
	}
	Tree->RemoveSegment(this);
}


