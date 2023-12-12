// Copyright © 2023 Silas Schuerger, Levin Theil

#pragma once

#include "CoreMinimal.h"
#include "Trunk.h"

class TREEGROWTHAI_API FBranch : public FTrunk
{
public:
	explicit FBranch(int Energy, FTrunk* FromBranch);
	virtual ~FBranch() override;

	FTrunk* FromBranch;
};
