// Copyright © 2023 Silas Schuerger, Levin Theil


#include "Tree.h"

#include "Segment.h"
#include "Kismet/KismetMathLibrary.h"


ATree::ATree()
{
}

void ATree::GenerateTree()
{
	if(!AllSegments.IsEmpty())
	{
		AllSegments.Empty();
        NewSegments.Empty();
		for (const auto& Leaves : AllLeaves)
		{
			if (Leaves)
			{
				Leaves->Destroy();
			}
		}
		AllLeaves.Empty();
	}

	NewObject<USegment>()->Setup(this, nullptr, FVector::UpVector, 0, InitEnergy);

	OverallEnergy = InitEnergy;
	Day = 0;
	DrawDebug();
}

void ATree::AdvanceDay()
{	
	for (const auto& Leaf : AllLeaves)
	{
		if (Leaf)
		{
			Leaf->CollectEnergy();
		}
	}
	
	for (const auto& Segment : LeavesSegments)
	{
		Segment->ShareEnergy();
	}

	const int SegmentEnergyShare = SavedEnergy / NewSegments.Num();
	SavedEnergy = 0;
	for (const auto& Segment : NewSegments)
	{
		Segment->Energy += SegmentEnergyShare;
	}
	
	for (const auto& Segment : AllSegments)
	{
		Segment->Grow();
	}

	OverallEnergy = 0;
	for (const auto& Segment : AllSegments)
	{
		OverallEnergy += Segment->Energy;
	}
	
	Day++;
	DrawDebug();

	// Reset if Tree cant grow no more
	if (NewSegments.IsEmpty())
	{
		GenerateTree();
	}
}

void ATree::DrawDebug()
{
	if (!EnableDebug) return;
	
	FlushPersistentDebugLines(GetWorld());
	FlushDebugStrings(GetWorld());
	for (const auto& Segment : AllSegments)
	{
		if (!Segment) break;
		float EnergyRatio = FMath::Clamp(Segment->Energy / InitEnergy, 0, 1);
		const float Red = FMath::Lerp(255.0f, 0.0f, EnergyRatio);
		const float Green = FMath::Lerp(0.0f, 255.0f, EnergyRatio);
		FColor Color = FColor(Red, Green, 0);
		DrawDebugCylinder(GetWorld(), Segment->Start + GetActorLocation(), Segment->End + GetActorLocation(), Segment->Radius, 8, Color, true);
		if (EnableDebugStrings)
		{
			DrawDebugString(GetWorld(), Segment->Start + (Segment->End - Segment->Start) / 2 + GetActorLocation(), FString::SanitizeFloat(Segment->Energy));
		}
	}
	if (EnableDebugStrings)
	{
		DrawDebugString(GetWorld(), GetActorLocation() + FVector(1, 0, 0) * 50, FString::SanitizeFloat(OverallEnergy), nullptr, FColor::Green);
	}

	for (const auto& Segment : NewSegments)
	{
		if (!Segment) break;
		
		float EnergyRatio = FMath::Clamp(Segment->Energy / BranchCost, 0, 1);
		const float Red = FMath::Lerp(255.0f, 0.0f, EnergyRatio);
		const float Green = FMath::Lerp(0.0f, 255.0f, EnergyRatio);
		FColor Color = FColor(Red, Green, 0);
		DrawDebugSphere(GetWorld(), Segment->End, 2, 8, Color, true);
	}
}

void ATree::AddSegment()
{
	TArray<USegment*> Segments = NewSegments;
	for (const auto& Segment : Segments)
	{
		Segment->GrowSegment(true, GetRandomDirection(Segment->End - Segment->Start, 0, MaxSegmentAngle));
	}

	AdvanceDay();
	
	DrawDebug();
}

void ATree::BranchOff()
{
	TArray<USegment*> Segments = NewSegments;
	for (const auto& Segment : Segments)
	{
		const bool TwoBranches = FMath::RandRange(0, TwoBranchesSpawnRate) == 0 ? true : false;
		Segment->BranchOff(true, GetRandomDirection(Segment->End - Segment->Start, 0, MaxSegmentAngle),
			GetRandomDirection(Segment->End - Segment->Start, MinBranchAngle, MaxBranchAngle), TwoBranches);
	}

	AdvanceDay();
	
	DrawDebug();
}

void ATree::GrowLeaves()
{
	if (!LeavesClass) return;
	
	TArray<USegment*> Segments = NewSegments;
	for (const auto& Segment : Segments)
	{
		Segment->GrowLeaves(true);
	}

	DrawDebug();
}

void ATree::RemoveSegment(USegment* Segment)
{
	AllSegments.Remove(Segment);
	NewSegments.Remove(Segment);
	if (Segment->Leaves)
	{
		AllLeaves.Remove(Segment->Leaves);
		Segment->Leaves->Destroy();
	}
}

void ATree::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (EnableDebug) DrawDebug();
	else FlushPersistentDebugLines(GetWorld());
}

void ATree::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if(!AllSegments.IsEmpty())
	{
		AllSegments.Empty();
		NewSegments.Empty();
		for (const auto& Leaves : AllLeaves)
		{
			if (Leaves)
			{
				Leaves->Destroy();
			}
		}
		AllLeaves.Empty();
	}
}

void ATree::BeginPlay()
{
	Super::BeginPlay();

	GenerateTree();
}

FVector ATree::GetRandomDirection(const FVector& From, const float MinAngle, const float MaxAngle)
{
	check(MinAngle >= 0);
	check(MaxAngle <= 90);

	const FVector FromVector = From.GetSafeNormal();
	const float HelperAngle = FMath::FRandRange(0.0f, 360.0f);
	FVector HelperDotProduct = FVector(0, FromVector.Z, -FromVector.Y);
	HelperDotProduct.Normalize();
	const FVector HelperVector = UKismetMathLibrary::RotateAngleAxis(HelperDotProduct, HelperAngle, FromVector);
	const FVector DotProduct = FVector::CrossProduct(FromVector, HelperVector).GetSafeNormal();
	return UKismetMathLibrary::RotateAngleAxis(FromVector, FMath::FRandRange(MinAngle, MaxAngle), DotProduct).GetSafeNormal();
}

