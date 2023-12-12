// Copyright © 2023 Silas Schuerger, Levin Theil


#include "Trunk.h"
#include "Globals.h"
#include "Kismet/KismetMathLibrary.h"


FTrunk::FTrunk(const int Energy)
	: Energy(Energy), Id(FGlobals::Instance().GetId())
{
	AddTreePoint(FVector::ZeroVector, FVector::DownVector, FGlobals::Instance().TrunkRadius);
	AddTreePoint(FVector(0, 0, FGlobals::Instance().StartingHeight), TreePoints.Last()->Location, TreePoints.Last()->Radius * 0.8);
}

FTrunk::~FTrunk()
{
	for (const auto& TreePoint : TreePoints)
	{
		delete TreePoint;
	}
}

void FTrunk::AddTreePoint(const FVector& Location, const FVector& FromLocation, const float Radius)
{
	FTreePoint* TreePoint = new FTreePoint(Location, Radius);
	
	const FVector RotationAxis = Location - FromLocation;
	checkf(RotationAxis != FVector::ZeroVector, TEXT("RotationAxis can't be ZeroVector!"));
	
	FVector DotProduct = FVector(0, RotationAxis.Z, -RotationAxis.Y);
	DotProduct.Normalize();
	DotProduct *= TreePoint->Radius;

	// Calculate Vertices using the SamplePoint
	for (uint32 i = 0; i < FGlobals::Instance().MeshQuality; i++)
	{
		const float DegreesToRotate = 360.f / FGlobals::Instance().MeshQuality * i;
		const FVector RotatedPoint = UKismetMathLibrary::RotateAngleAxis(DotProduct, DegreesToRotate, RotationAxis);
		TreePoint->Vertices.Add(Location + RotatedPoint);
	}
	
	TreePoints.Add(TreePoint);
}
