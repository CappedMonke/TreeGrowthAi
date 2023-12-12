// Copyright © 2023 Silas Schuerger, Levin Theil


#include "Trunk.h"

#include "Branch.h"
#include "Globals.h"
#include "Kismet/KismetMathLibrary.h"


FTrunk::FTrunk(const int InitEnergy)
	: Energy(InitEnergy), Id(FGlobals::Instance().GetId())
{
}

FTrunk::~FTrunk()
{
	for (const auto& TreePoint : TreePoints)
	{
		delete TreePoint;
	}
}

void FTrunk::Setup()
{
	AddTreePoint(FVector::ZeroVector, FVector::DownVector);
	AddTreePoint(FVector(0, 0, FGlobals::Instance().StartingHeight), TreePoints.Last()->Location);
}

void FTrunk::AddTreePoint(const FVector& Location, const FVector& FromLocation)
{
	FTreePoint* TreePoint = new FTreePoint(Location, FGlobals::Instance().TrunkRadius);
	
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

void FTrunk::Grow(const FVector& Direction, const int EnergyIn)
{
	Energy -= EnergyIn;
	if (Energy < 0)
	{
		FGlobals::Instance().IsTreeAlive = false;
		return;
	}
	
	const FVector NewLocation = TreePoints.Last()->Location + Direction.GetSafeNormal() * EnergyIn;

	for (const auto& TreePoint : TreePoints)
	{
		TreePoint->Radius += BaseGrowthRate * EnergyIn;
		if (TreePoint->Radius > GrowBranchMaxRadius) TreePoint->CanGrowBranch = false;
		for (auto& Vertex : TreePoint->Vertices)
		{
			FVector OriginToVertex = Vertex - TreePoint->Location;
			Vertex += OriginToVertex.GetSafeNormal() * TreePoint->Radius;
		}
	}
	
	AddTreePoint(NewLocation, TreePoints.Last()->Location);
}

void FTrunk::GrowBranch(const int EnergyCost, FTreePoint* TreePoint, int Vertex)
{
	if (!TreePoint->CanGrowBranch) return;
	Energy -= EnergyCost;
	FBranch* Branch = new FBranch(EnergyCost, this);
	Branch->Setup(TreePoint, Vertex);
	TreePoint->CanGrowBranch = false;
	Branches.Add(Branch);
}
