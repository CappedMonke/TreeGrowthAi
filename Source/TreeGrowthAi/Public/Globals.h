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

	int GetId()
	{
		return CurrentId++;
	}

	uint32 MeshQuality = 12;
	float TrunkRadius = 1.0f;
	float StartingHeight = 1.0f;
	bool IsTreeAlive = true;
	
private:
	FGlobals() {}
	int CurrentId = 0;
};
