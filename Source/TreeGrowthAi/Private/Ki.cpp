// Copyright © 2023 Silas Schuerger, Levin Theil


#include "Ki.h"

#include "Globals.h"


void FKi::GrowTree(FTrunk* BranchIn)
{
	BranchIn->Grow(BranchIn->TreePoints.Last()->Location - BranchIn->TreePoints.Last(1)->Location, 5);
	
	if (FMath::RandRange(0, 5) == 5)
	{
		BranchIn->GrowBranch(BranchIn->Energy * 0.25, BranchIn->TreePoints[BranchIn->TreePoints.Num() / 2], FMath::RandRange(0, FGlobals::Instance().MeshQuality));
	}
}

FKi::FKi()
{
}

