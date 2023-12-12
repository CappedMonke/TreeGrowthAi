// Copyright © 2023 Silas Schuerger, Levin Theil

#pragma once

#include "CoreMinimal.h"
#include "Branch.h"

class TREEGROWTHAI_API FKi
{
public:
	static FKi& Instance()
	{
		static FKi* Instance = new FKi();
		return *Instance;
	}

	static void GrowTree(FTrunk* BranchIn);
	
private:
	FKi();
};
