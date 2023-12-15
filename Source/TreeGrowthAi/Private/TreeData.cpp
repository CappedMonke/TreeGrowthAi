// Copyright © 2023 Silas Schuerger, Levin Theil


#include "TreeData.h"


FTreeData& FTreeData::Instance()
{
	static FTreeData* Instance = new FTreeData();
	return *Instance;
}

void FTreeData::RemoveSegmentFromAll(FSegment* Segment)
{
	AllSegments.Remove(Segment);
	NewSegments.Remove(Segment);
	SegmentsWithLeaves.Remove(Segment);
}
