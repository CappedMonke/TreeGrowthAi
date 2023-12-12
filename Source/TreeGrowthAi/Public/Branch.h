// Copyright © 2023 Silas Schuerger, Levin Theil

#pragma once

#include "CoreMinimal.h"
#include "Trunk.h"

class TREEGROWTHAI_API FBranch final : public FTrunk
{
public:
	explicit FBranch(int InitEnergy, FTrunk* FromBranch);
	virtual ~FBranch() override;

	void Setup(const FTreePoint* TreePoint, int Vertex);
	virtual void Grow(const FVector& Direction, int EnergyIn) override;
	void GrowLeaves();
};
