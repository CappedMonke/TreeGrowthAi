// Copyright © 2023 Silas Schuerger, Levin Theil

#pragma once

#include "CoreMinimal.h"


class TREEGROWTHAI_API FGlobals
{
public:
	static FGlobals& Instance()
	{
		static FGlobals* Instance = new FGlobals();
		return *Instance;
	}

	uint32 MeshQuality = 12;
	
private:
	FGlobals() {}
};
