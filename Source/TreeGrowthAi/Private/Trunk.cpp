// Copyright © 2023 Silas Schuerger, Levin Theil


#include "Trunk.h"
#include "Globals.h"
#include "Kismet/KismetMathLibrary.h"


FTrunk::FTrunk()
{
	AddTreePoint(FVector::ZeroVector, FVector::DownVector);
	AddTreePoint(FVector::UpVector + FVector(0, 0, 10), TreePoints.Last()->Location);
}

FTrunk::~FTrunk()
{
	for (const auto& TreePoint : TreePoints)
	{
		delete TreePoint;
	}
}

void FTrunk::AddTreePoint(const FVector& Location, const FVector& FromLocation)
{
	FTreePoint* TreePoint = new FTreePoint(Location);
	
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
