// Copyright © 2023 Silas Schuerger, Levin Theil

#pragma once

#include "CoreMinimal.h"
#include "Leaves.h"
#include "Segment.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
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
	void GrowLeaves();
	
	void DrawDebug();
	void DrawTree();

	UPROPERTY(EditAnywhere, Category="Tree")
	bool EnableMesh = false;
	UPROPERTY(EditAnywhere, Category="Tree")
	bool EnableDebug = true;
	UPROPERTY(EditAnywhere, Category="Tree")
	bool EnableDebugStrings = true;
	
	UPROPERTY(EditAnywhere, Category="Tree")
	int InitEnergy = 2000;

	UPROPERTY(EditAnywhere, Category="Tree")
	int MeshQuality = 8;

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
	float SavedEnergy = 0;
	float OverallEnergy = 0;
	float Height = 0;
	int Day = 0;

	UPROPERTY(EditAnywhere, Category="KIRewards")
	float OverallEnergyRewardMultiplier = 0.01f;
	UPROPERTY(EditAnywhere, Category="KIRewards")
	float HeightRewardMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, Category="KIRewards")
	float DayRewardMultiplier = 100.0f;
	

	UPROPERTY(EditAnywhere, Category="Segments")
	int MaxDaysWithoutEnergy = 7;
	UPROPERTY(EditAnywhere, Category="Segments")
	float DailyCostMultiplier = 5.0f;
	UPROPERTY(EditAnywhere, Category="Segments")
	float SegmentCost = 10.0f;
	UPROPERTY(EditAnywhere, Category="Segments")
	float BranchCost = 40.0f;
	UPROPERTY(EditAnywhere, Category="Segments")
	float LeavesCost = 50.0f;
	UPROPERTY(EditAnywhere, Category="Segments", meta = (ToolTip = "The higher the lesser the chance to spawn two branches."))
	int TwoBranchesSpawnRate = 3;
	UPROPERTY(EditAnywhere, Category="Segments")
	float TreeEnergyMultiplier = 0.1; // How much Energy will be sent to the NewSegments
	UPROPERTY(EditAnywhere, Category="Segments")
	float GrowthLengthMulitplier = 5.0f;
	UPROPERTY(EditAnywhere, Category="Segments")
	float GrowthRadiusMultiplier = 0.01f;
	UPROPERTY(EditAnywhere, Category="Segments")
	float MaxSegmentAngle = 8.0f;
	UPROPERTY(EditAnywhere, Category="Segments")
	float MinBranchAngle = 25.0f;
	UPROPERTY(EditAnywhere, Category="Segments")
	float MaxBranchAngle = 75.0f;

	UPROPERTY()
	UProceduralMeshComponent* Mesh;
	
protected:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void BeginPlay() override;

private:
	static FVector GetRandomDirection(const FVector& From, const float MinAngle, const float MaxAngle);
};
