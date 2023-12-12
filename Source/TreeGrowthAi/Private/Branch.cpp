// Copyright © 2023 Silas Schuerger, Levin Theil


#include "Branch.h"


FBranch::FBranch(int Energy, FTrunk* FromBranch)
	: FTrunk(Energy), FromBranch(FromBranch)
{
}

FBranch::~FBranch()
{
}
