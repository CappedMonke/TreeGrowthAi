// Copyright © 2023 Silas Schuerger, Levin Theil


#include "Tree.h"

#include "Segment.h"


ATree::ATree()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATree::GenerateTree()
{
	if(!AllSegments.IsEmpty())
	{
		AllSegments.Empty();
        NewSegments.Empty();
		for (const auto& Leaves : AllLeaves)
		{
			Leaves->Destroy();
		}
		AllLeaves.Empty();
	}

	NewObject<USegment>()->Setup(this, nullptr, FVector::UpVector, 0, InitEnergy);
	
	Day = 0;
	DrawDebug();
}

void ATree::AdvanceDay()
{
	for (const auto& Segment : AllSegments)
	{
		Segment->Grow();
	}

	if (AllSegments.Num() == 0)
	{
		GenerateTree();
	}
	
	Day++;
	DrawDebug();
}

void ATree::DrawDebug() const
{
	if (!EnableDebug) return;
	
	FlushPersistentDebugLines(GetWorld());
	for (const auto& Segment : AllSegments)
	{
		float EnergyRatio = FMath::Clamp(Segment->Energy / InitEnergy, 0, 1);
		const float Red = FMath::Lerp(255.0f, 0.0f, EnergyRatio);
		const float Green = FMath::Lerp(0.0f, 255.0f, EnergyRatio);
		FColor Color = FColor(Red, Green, 0);
		DrawDebugCylinder(GetWorld(), Segment->Start + GetActorLocation(), Segment->End + GetActorLocation(), Segment->Radius, 8, Color, true);
	}

	for (const auto& Segment : NewSegments)
	{
		float EnergyRatio = FMath::Clamp(Segment->Energy / InitEnergy, 0, 1);
		const float Red = FMath::Lerp(255.0f, 0.0f, EnergyRatio);
		const float Green = FMath::Lerp(0.0f, 255.0f, EnergyRatio);
		FColor Color = FColor(Red, Green, 0);
		DrawDebugSphere(GetWorld(), Segment->End, 2, 8, Color, true);
	}
}

void ATree::AddSegment() const
{
	TArray<USegment*> Segments = NewSegments;
	for (const auto& Segment : Segments)
	{
		FVector SegmentDirection = (Segment->End - Segment->Start).GetSafeNormal();
		SegmentDirection += FVector(FMath::FRandRange(-0.1f, 0.1f), FMath::FRandRange(-0.1f, 0.1f), FMath::FRandRange(-0.1f, 0.1f));
		Segment->GrowSegment(true, SegmentDirection);
	}
	
	DrawDebug();
}

void ATree::BranchOff() const
{
	TArray<USegment*> Segments = NewSegments;
	for (const auto& Segment : Segments)
	{
		FVector SegmentDirection = (Segment->End - Segment->Start).GetSafeNormal();
		SegmentDirection += FVector(FMath::FRandRange(-0.15f, 0.15f), FMath::FRandRange(-0.15f, 0.15f), FMath::FRandRange(-0.15f, 0.15f));
		FVector BranchDirection = SegmentDirection + FVector(
			(FMath::RandRange(0, 1) == 0) ? FMath::FRandRange(-0.8f, -0.2f) : FMath::FRandRange(0.2f, 0.8f),
			(FMath::RandRange(0, 1) == 0) ? FMath::FRandRange(-0.8f, -0.2f) : FMath::FRandRange(0.2f, 0.8f),
			(FMath::RandRange(0, 1) == 0) ? FMath::FRandRange(-0.8f, -0.2f) : FMath::FRandRange(0.2f, 0.8f)
		);
		Segment->BranchOff(true, SegmentDirection, BranchDirection);
	}

	DrawDebug();
}

void ATree::GrowLeaves() const
{
	if (!LeavesClass) return;
	
	TArray<USegment*> Segments = NewSegments;
	for (const auto& Segment : Segments)
	{
		Segment->GrowLeaves(true);
	}
}

void ATree::RemoveSegment(USegment* Segment)
{
	AllSegments.Remove(Segment);
	NewSegments.Remove(Segment);
	AllLeaves.Remove(Segment->Leaves);
	Segment->Leaves->Destroy();
}

void ATree::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (EnableDebug) DrawDebug();
	else FlushPersistentDebugLines(GetWorld());
}

