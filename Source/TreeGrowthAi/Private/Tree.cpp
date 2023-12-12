// Copyright © 2023 Silas Schuerger, Levin Theil


#include "Tree.h"

#include "Globals.h"
#include "Branch.h"


ATree::ATree()
{
	PrimaryActorTick.bCanEverTick = false;
	Trunk = nullptr;
	FGlobals::Instance().MeshQuality = MeshQuality;
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

	if (FGlobals::Instance().MeshQuality != MeshQuality)
	{
		FGlobals::Instance().MeshQuality = MeshQuality;
		GenerateTree();
	}
}

void ATree::GenerateTree()
{
	delete Trunk;
	Trunk = new FTrunk();
	
	if (ShowDebug)
	{
		FlushPersistentDebugLines(GetWorld());
		IterateBranches(Trunk, &ATree::DrawDebugHelpers);
	}
}

void ATree::AdvanceDay()
{
}

void ATree::DrawDebugHelpers(FTrunk* BranchIn)
{	
	for (const auto& TreePoint : BranchIn->TreePoints)
	{
		DrawDebugSphere(GetWorld(), TreePoint->Location, 0.1, 4, FColor::Red, true);

		for (const auto& Vertex : TreePoint->Vertices)
		{
			DrawDebugSphere(GetWorld(), Vertex, 0.1, 4, FColor::Blue, true);
		}
	}
}

void ATree::DrawTree(FTrunk* BranchIn)
{	
	for (auto& TreePoint : BranchIn->TreePoints)
	{
		
	}


}

void ATree::IterateBranches(FTrunk* BranchIn, void(ATree::* Function)(FTrunk* CurrentBranch))
{
	(this->*Function)(BranchIn);
	
	for (const auto& Branch : BranchIn->Branches)
	{
		IterateBranches(Branch, Function);
	}
}

