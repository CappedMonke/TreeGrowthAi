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
	void AddSegment();

	UFUNCTION(CallInEditor, Category="Tree")
	void BranchOff();

	UFUNCTION(CallInEditor, Category="Tree")
	void GrowLeaves() const;
	
	void DrawDebug() const;

	UPROPERTY(EditAnywhere, Category="Tree")
	bool EnableDebug = true;
	UPROPERTY(EditAnywhere, Category="Tree")
	bool EnableDebugStrings = true;
	
	UPROPERTY(EditAnywhere, Category="Tree")
	int InitEnergy = 2000;

	int Day = 0;

	UPROPERTY(EditAnywhere, Category="Tree")
	TSubclassOf<ALeaves> LeavesClass;

	UPROPERTY()
	TArray<USegment*> AllSegments;
	UPROPERTY()
	TArray<USegment*> NewSegments;
	UPROPERTY()
	TArray<USegment*> LeavesSegments;
	UPROPERTY()
	TArray<ALeaves*> AllLeaves;

	void RemoveSegment(USegment* Segment);
	float TreeEnergy = 0;

	UPROPERTY(EditAnywhere, Category="Segments")
	int MaxDaysWithoutEnergy = 7;
	UPROPERTY(EditAnywhere, Category="Segments")
	float DailyCostMultiplier = 5.0f;
	UPROPERTY(EditAnywhere, Category="Segments")
	float SegmentCost = 10.0f;
	UPROPERTY(EditAnywhere, Category="Segments")
	float BranchCost = 40.0f;
	UPROPERTY(EditAnywhere, Category="Segments")
	float LeavesCost = 60.0f;
	UPROPERTY(EditAnywhere, Category="Segments", meta = (ToolTip = "The higher the lesser the chance to spawn two branches."))
	int TwoBranchesSpawnRate = 3;
	UPROPERTY(EditAnywhere, Category="Segments")
	float TreeTotalEnergyMultiplier = 0.2; // How much Energy will be sent to the NewSegments
	UPROPERTY(EditAnywhere, Category="Segments")
	float GrowthLengthMulitplier = 5.0f;
	UPROPERTY(EditAnywhere, Category="Segments")
	float GrowthRadiusMultiplier = 0.1f;
	UPROPERTY(EditAnywhere, Category="Segments")
	float MinGrowthAngle = 0.0f;
	UPROPERTY(EditAnywhere, Category="Segments")
	float MaxGrowthAngle = 20.0f;
	UPROPERTY(EditAnywhere, Category="Segments")
	float MinBranchAngle = 20.0f;
	UPROPERTY(EditAnywhere, Category="Segments")
	float MaxBranchAngle = 90.0f;

	
protected:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void BeginPlay() override;
};
