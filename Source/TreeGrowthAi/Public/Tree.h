// Copyright © 2023 Silas Schuerger, Levin Theil

#pragma once

#include "CoreMinimal.h"
#include "Leaves.h"
#include "Segment.h"
#include "GameFramework/Actor.h"
#include "Tree.generated.h"

UCLASS()
class TREEGROWTHAI_API ATree : public AActor
{
	GENERATED_BODY()

public:
	ATree();

	UFUNCTION(CallInEditor, Category="Tree")
	void GenerateTree();

	UFUNCTION(CallInEditor, Category="Tree")
	void AdvanceDay();

	// Debugging
	UFUNCTION(CallInEditor, Category="Tree")
	void AddSegment() const;

	UFUNCTION(CallInEditor, Category="Tree")
	void BranchOff() const;

	UFUNCTION(CallInEditor, Category="Tree")
	void GrowLeaves() const;
	
	void DrawDebug() const;

	UPROPERTY(EditAnywhere, Category="Tree")
	bool EnableDebug = true;
	
	UPROPERTY(EditAnywhere, Category="Tree")
	int InitEnergy = 100;

	int Day = 0;

	UPROPERTY(EditAnywhere, Category="Tree")
	TSubclassOf<ALeaves> LeavesClass;

	UPROPERTY()
	TArray<USegment*> AllSegments;
	UPROPERTY()
	TArray<USegment*> NewSegments;
	UPROPERTY()
	TArray<ALeaves*> AllLeaves;

	void RemoveSegment(USegment* Segment);
	
protected:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
};
