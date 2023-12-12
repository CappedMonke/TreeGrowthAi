// Copyright © 2023 Silas Schuerger, Levin Theil


#include "Branch.h"

#include "Globals.h"


FBranch::FBranch(const int InitEnergy, FTrunk* FromBranch)
	: FTrunk(InitEnergy)
{
	this->FromBranch = FromBranch;
}

FBranch::~FBranch()
{
}

void FBranch::Setup(const FTreePoint* TreePoint, const int Vertex)
{
	AddTreePoint(TreePoint->Location, TreePoint->Vertices[Vertex]);
	AddTreePoint(TreePoint->Vertices[Vertex], TreePoint->Location);
}

void FBranch::Grow(const FVector& Direction, int EnergyIn)
{
	FTrunk::Grow(Direction, EnergyIn);
	GrowLeaves();
}

void FBranch::GrowLeaves()
{
}
