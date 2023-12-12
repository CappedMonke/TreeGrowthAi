// Copyright © 2023 Silas Schuerger, Levin Theil

#pragma once

#include "CoreMinimal.h"
#include "Trunk.h"

class TREEGROWTHAI_API FBranch : public FTrunk
{
public:
	FBranch(FTrunk* FromBranch);
	~FBranch();

	FTrunk* FromBranch;
};
