// Copyright © 2023 Silas Schuerger, Levin Theil


#include "Tree.h"

#include "Globals.h"
#include "Branch.h"


ATree::ATree()
{
	PrimaryActorTick.bCanEverTick = true;
	Trunk = nullptr;
	FGlobals::Instance().MeshQuality = MeshQuality;
	FGlobals::Instance().TrunkRadius = TrunkRadius;
	FGlobals::Instance().StartingHeight = StartingHeight;
	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>("Mesh");
	SetRootComponent(Mesh);
}

void ATree::BeginPlay()
{
	Super::BeginPlay();
	GenerateTree();
}

void ATree::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (!Trunk) return;

	FlushPersistentDebugLines(GetWorld());
	if (ShowDebug)
	{
		IterateBranches(Trunk, &ATree::DrawDebugHelpers);
	}

	if (FGlobals::Instance().MeshQuality != MeshQuality ||
		FGlobals::Instance().TrunkRadius != TrunkRadius ||
		FGlobals::Instance().StartingHeight != StartingHeight)
	{
		FGlobals::Instance().MeshQuality = MeshQuality;
		FGlobals::Instance().TrunkRadius = TrunkRadius;
		FGlobals::Instance().StartingHeight = StartingHeight;
		GenerateTree();
	}
}

void ATree::GenerateTree()
{
	Mesh->ClearAllMeshSections();
	delete Trunk;
	Trunk = new FTrunk(StartingEnergy);
	FlushPersistentDebugLines(GetWorld());
	IterateBranches(Trunk, &ATree::DrawTree);
}

void ATree::AdvanceDay()
{
	check(Trunk);
	Trunk->Grow(FVector::UpVector, 5);
	Mesh->ClearAllMeshSections();
	IterateBranches(Trunk, &ATree::DrawTree);

	if (FGlobals::Instance().IsTreeAlive == false)
	{
		GenerateTree();
		FGlobals::Instance().IsTreeAlive = true;
	}
}

void ATree::DrawDebugHelpers(FTrunk* BranchIn)
{	
	for (const auto& TreePoint : BranchIn->TreePoints)
	{
		DrawDebugSphere(GetWorld(), TreePoint->Location + GetActorLocation(), 0.1, 4, FColor::Red, true);

		for (const auto& Vertex : TreePoint->Vertices)
		{
			DrawDebugSphere(GetWorld(), Vertex + GetActorLocation(), 0.1, 4, FColor::Blue, true);
		}
	}
}

void ATree::DrawTree(FTrunk* BranchIn)
{
	if (ShowDebug) DrawDebugHelpers(BranchIn);
	
	checkf(BranchIn->TreePoints.Num() > 1, TEXT("Only one TreePoint in Tree. There must be at least two."));
	
	TArray<FVector> Vertices;
	TArray<int32> Triangles;

	// Put the vertices of all TreePoints in an array
	for (const auto& TreePoint : BranchIn->TreePoints)
		for (const auto& Vertex : TreePoint->Vertices)
			Vertices.Add(Vertex);
	
	// Every TreePoint-Vertex-Ring connects to the one above, except the most upper one
	for(int i = 0; i < BranchIn->TreePoints.Num() - 1; i++)
	{
		for(int j = 0; j < MeshQuality; j++)
		{
			// Triangles with bottom side connected
			Triangles.Add(i * MeshQuality + j);
			Triangles.Add((i + 1) * MeshQuality + j);
			if(j < MeshQuality - 1) Triangles.Add(i * MeshQuality + j + 1);
			else Triangles.Add(i * MeshQuality);
		
			// Triangles with top side connected
			Triangles.Add((i + 1) * MeshQuality + j);
			if(j < MeshQuality - 1)
			{
				Triangles.Add((i + 1) * MeshQuality + j + 1);
				Triangles.Add(i * MeshQuality + j + 1);
			}
			else
			{
				Triangles.Add((i + 1) * MeshQuality);
				Triangles.Add(i * MeshQuality);
			}
		}
	}
	
	Mesh->CreateMeshSection(BranchIn->Id, Vertices, Triangles, TArray<FVector>(), TArray<FVector2d>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);
}

void ATree::IterateBranches(FTrunk* BranchIn, void(ATree::* Function)(FTrunk* CurrentBranch))
{
	(this->*Function)(BranchIn);
	
	for (const auto& Branch : BranchIn->Branches)
	{
		IterateBranches(Branch, Function);
	}
}

