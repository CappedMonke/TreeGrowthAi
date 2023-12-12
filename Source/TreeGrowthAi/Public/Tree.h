// Copyright © 2023 Silas Schuerger, Levin Theil

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Tree.generated.h"

struct FTreePoint;
class FTrunk;

UCLASS()
class TREEGROWTHAI_API ATree : public AActor
{
	GENERATED_BODY()

public:
	ATree();

protected:
	virtual void BeginPlay() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

private:
	// UNREAL
	UFUNCTION(CallInEditor, Category="Tree")
	void GenerateTree();
	UFUNCTION(CallInEditor, Category="Tree")
	void AdvanceDay();
	
	UPROPERTY(EditAnywhere, Category="Tree")
	int MeshQuality = 12;
	UPROPERTY(EditAnywhere, Category="Tree")
	float TrunkRadius = 1.0f;
	UPROPERTY(EditAnywhere, Category="Tree")
	float StartingHeight = 5.0f;
	UPROPERTY(EditAnywhere, Category="Tree")
	int StartingEnergy = 50;
	UPROPERTY(EditAnywhere, Category="Tree")
	bool ShowDebug = false;
	UPROPERTY()
	UProceduralMeshComponent* Mesh;

	// SELF
	void DrawDebugHelpers(FTrunk* BranchIn);
	void DrawTree(FTrunk* BranchIn);
	void IterateBranches(FTrunk* BranchIn, void(ATree::*Function)(FTrunk* CurrentBranch));
	
	FTrunk* Trunk;
};
