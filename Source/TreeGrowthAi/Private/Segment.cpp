// Copyright © 2023 Silas Schuerger, Levin Theil

#include "Segment.h"

#include "Leaves.h"
#include "Tree.h"
#include "Kismet/KismetMathLibrary.h"


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
	End = Start + ToLocation * Tree->GrowthLengthMulitplier;
	Height = End.Z;
		
	Tree->AllSegments.Add(this);
	Tree->NewSegments.Add(this);
}

void USegment::GrowSegment(const bool ShouldGrow, const FVector& GrowthDirection)
{
	if(!ShouldGrow) return;
	if (Energy - Tree->SegmentCost < 0) return;

	Tree->NewSegments.Remove(this);
	CanGrowLeaves = false;
	
	USegment* SegmentObj = NewObject<USegment>();
	SegmentObj->Setup(Tree, this, GrowthDirection.GetSafeNormal(), Index + 1, Energy / 2 - Tree->SegmentCost);
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

void USegment::BranchOff(const bool ShouldBranchOff, const FVector& GrowthDirection, const FVector& BranchGrowthDirection, const bool SecondBranch)
{
	if (!ShouldBranchOff) return;
	if (Energy - Tree->BranchCost < 0) return;
	if (SecondBranch && Energy - Tree->BranchCost * 2 < 0 ) return;
		
	Tree->NewSegments.Remove(this);
	CanGrowLeaves = false;
	
	USegment* SegmentObj = NewObject<USegment>();
	SegmentObj->Setup(Tree, this, GrowthDirection.GetSafeNormal(), Index + 1, (Energy - Tree->BranchCost) / 4);
	ToSegments.Add(SegmentObj); // Segment
	
	SegmentObj = NewObject<USegment>();
	SegmentObj->Setup(Tree, this, BranchGrowthDirection.GetSafeNormal(), Index + 1, (Energy - Tree->BranchCost) / 4);
	ToSegments.Add(SegmentObj); // Branch

	if (SecondBranch)
	{
		const FVector SecondBranchDirection = UKismetMathLibrary::RotateAngleAxis(BranchGrowthDirection.GetSafeNormal(), 180, Start - End);
		SegmentObj = NewObject<USegment>();
		SegmentObj->Setup(Tree, this, SecondBranchDirection, Index + 1, (Energy - Tree->BranchCost) / 4);
		ToSegments.Add(SegmentObj); // Branch
	}

	Energy = (Energy - Tree->BranchCost) / 2;
}

void USegment::GrowLeaves(const bool ShouldGrowLeaves)
{
	if(!ShouldGrowLeaves || !CanGrowLeaves) return;
	if (Energy - Tree->LeavesCost < 0) return;

	Energy -= Tree->LeavesCost;
	CanGrowLeaves = false;
	Leaves = Tree->GetWorld()->SpawnActor<ALeaves>(Tree->LeavesClass, End, FRotator::ZeroRotator);
	Tree->LeavesSegments.Add(this);
	Tree->AllLeaves.Add(Leaves);
}

void USegment::Grow()
{
	if(Energy - Radius * Tree->DailyCostMultiplier < 0)
	{
		DaysWithoutEnergy++;
		if (DaysWithoutEnergy > Tree->MaxDaysWithoutEnergy)
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
	
	Energy -= Radius * Tree->DailyCostMultiplier;
	Radius += Energy / Tree->InitEnergy * Tree->GrowthRadiusMultiplier;
	const float EnergyToSpend = Energy * Tree->TreeTotalEnergyMultiplier;
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


