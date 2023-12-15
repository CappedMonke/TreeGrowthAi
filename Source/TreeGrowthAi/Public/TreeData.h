// Copyright © 2023 Silas Schuerger, Levin Theil

#pragma once

#include "CoreMinimal.h"
#include "Segment.h"


class TREEGROWTHAI_API FTreeData
{
public:
	static FTreeData& Instance();
	
	TArray<FSegment*> AllSegments;
	TArray<FSegment*> NewSegments;
	TArray<FSegment*> SegmentsWithLeaves;

	void RemoveSegmentFromAll(FSegment* Segment);
	
private:
	FTreeData() {};
};
