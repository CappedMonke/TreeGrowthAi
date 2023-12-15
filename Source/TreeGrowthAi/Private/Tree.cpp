// Copyright © 2023 Silas Schuerger, Levin Theil


#include "Tree.h"

#include "TreeData.h"
#include "Segment.h"


ATree::ATree()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATree::GenerateTree()
{
	if(!FTreeData::Instance().AllSegments.IsEmpty())
	{
		FTreeData::Instance().AllSegments.Empty();
        FTreeData::Instance().NewSegments.Empty();
        FTreeData::Instance().SegmentsWithLeaves.Empty();
	}

	new FSegment(nullptr, FVector::UpVector, 0, InitEnergy);
	Day = 0;
	DrawDebug();
}

void ATree::AdvanceDay()
{
	for (const auto& Segment : FTreeData::Instance().AllSegments)
	{
		Segment->Grow();
	}

	if (FTreeData::Instance().AllSegments.Num() == 0)
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
	for (const auto& Segment : FTreeData::Instance().AllSegments)
	{
		float EnergyRatio = FMath::Clamp(Segment->Energy / InitEnergy, 0, 1);
		const float Red = FMath::Lerp(255.0f, 0.0f, EnergyRatio);
		const float Green = FMath::Lerp(0.0f, 255.0f, EnergyRatio);
		FColor Color = FColor(Red, Green, 0);
		DrawDebugCylinder(GetWorld(), Segment->Start + GetActorLocation(), Segment->End + GetActorLocation(), Segment->Radius, 8, Color, true);
	}

	for (const auto& Segment : FTreeData::Instance().NewSegments)
	{
		DrawDebugSphere(GetWorld(), Segment->End, 2, 8, FColor::Green, true);
	}
}

void ATree::AddSegment() const
{
	TArray<FSegment*> Segments = FTreeData::Instance().NewSegments;
	for (const auto& Segment : Segments)
	{
		Segment->GrowSegment(true, FVector::UpVector);
	}
	
	DrawDebug();
}

void ATree::BranchOff() const
{
	TArray<FSegment*> Segments = FTreeData::Instance().NewSegments;
	for (const auto& Segment : Segments)
	{
		Segment->BranchOff(true, FVector::UpVector, FVector::RightVector);
	}

	DrawDebug();
}

void ATree::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (EnableDebug) DrawDebug();
	else FlushPersistentDebugLines(GetWorld());
}

