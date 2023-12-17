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
	if (Tree->Height < Height)
	{
		Tree->Height = Height;
	}

	const FVector RotationAxis = End - Start;
	FVector DotProduct = FVector(0, RotationAxis.Z, -RotationAxis.Y);
	DotProduct.Normalize();
	DotProduct *= Radius;
	for (int i = 0; i < Tree->MeshQuality; i++)
	{
		const float DegreesToRotate = 360.0f / Tree->MeshQuality * i;
		const FVector RotatedPoint = UKismetMathLibrary::RotateAngleAxis(DotProduct, DegreesToRotate, RotationAxis);
		VerticesStart.Add(Start + RotatedPoint);
		VerticesEnd.Add(End + RotatedPoint);
	}
	
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
	if (Energy - Tree->BranchCost - Tree->SegmentCost < 0) return;
	
	Tree->NewSegments.Remove(this);
	CanGrowLeaves = false;

	USegment* SegmentObj = SegmentObj = NewObject<USegment>();
	SegmentObj->Setup(Tree, this, GrowthDirection.GetSafeNormal(), Index + 1, (Energy - Tree->SegmentCost) / 4);
	ToSegments.Add(SegmentObj); // Segment
	
	if (SecondBranch)
	{
		SegmentObj = NewObject<USegment>();
		SegmentObj->Setup(Tree, this, BranchGrowthDirection, Index + 1, (Energy - Tree->BranchCost) / 8);
		ToSegments.Add(SegmentObj); // Branch
		
		const FVector SecondBranchDirection = UKismetMathLibrary::RotateAngleAxis(BranchGrowthDirection.GetSafeNormal(), 180, BranchGrowthDirection.GetSafeNormal());
		SegmentObj = NewObject<USegment>();
		SegmentObj->Setup(Tree, this, SecondBranchDirection, Index + 1, (Energy - Tree->BranchCost) / 8);
		ToSegments.Add(SegmentObj); // Second Branch
	}
	else
	{
		SegmentObj = NewObject<USegment>();
		SegmentObj->Setup(Tree, this, BranchGrowthDirection.GetSafeNormal(), Index + 1, (Energy - Tree->BranchCost) / 4);
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
	Leaves->EnergyGain = Tree->LeavesEnergyGain;
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
		return;
	}

	if (DaysWithoutEnergy > 0)
	{
		DaysWithoutEnergy--;
	}
	
	Energy -= Radius * Tree->DailyCostMultiplier;
	Radius += Energy / Tree->InitEnergy * Tree->GrowthRadiusMultiplier;
	const float EnergyToSpend = Energy * Tree->TreeEnergyMultiplier;
	Tree->SavedEnergy += EnergyToSpend;
	Energy -= EnergyToSpend;

	for (auto& Vertex : VerticesStart)
	{
		Vertex = Start + (Vertex - Start).GetSafeNormal() * Radius;
	}
	for (auto& Vertex : VerticesEnd)
	{
		Vertex = End + (Vertex - End).GetSafeNormal() * Radius;
	}
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


