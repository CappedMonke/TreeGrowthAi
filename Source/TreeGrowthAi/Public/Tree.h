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

	UFUNCTION(BlueprintCallable, CallInEditor, Category="Tree")
	void GenerateTree();

	UFUNCTION(BlueprintCallable, Category="Tree")
	void AdvanceDay();

	UFUNCTION(BlueprintCallable, Category="Tree")
	void AddSegments(TArray<float> ShouldAdd, TArray<float> Angles);

	UFUNCTION(BlueprintCallable, Category="Tree")
	void AddBranches(TArray<float> ShouldAdd, TArray<float> SegmentAngles, TArray<float> BranchAngles, TArray<float> ShouldAddSecond);

	UFUNCTION(BlueprintCallable, Category="Tree")
	void AddLeaves(TArray<float> ShouldAdd);
	
	// Debugging
	UFUNCTION(CallInEditor, Category="Tree")
	void AdvanceDayEditor();
	
	UFUNCTION(CallInEditor, Category="Tree")
	void AddSegmentsEditor();

	UFUNCTION(CallInEditor, Category="Tree")
	void AddBranchesEditor();

	UFUNCTION(CallInEditor, Category="Tree")
	void AddLeavesEditor();
	
	void DrawDebug();
	void DrawTree();

	UPROPERTY(EditAnywhere, Category="Tree")
	bool EnableMesh = false;
	UPROPERTY(EditAnywhere, Category="Tree")
	bool EnableDebug = true;
	UPROPERTY(EditAnywhere, Category="Tree")
	bool EnableDebugStrings = true;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Tree")
	float InitEnergy = 2000;

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

	UPROPERTY(BlueprintReadWrite)
	int DaysWithoutGrowth = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MaxDaysWithoutGrowth = 7;
	
	void RemoveSegment(USegment* Segment);
	
	float SavedEnergy = 0;
	UPROPERTY(BlueprintReadWrite)
	float OverallEnergy = 0;
	UPROPERTY(BlueprintReadOnly)
	float Height = 0;
	UPROPERTY(BlueprintReadOnly)
	int Day = 0;
	UPROPERTY(BlueprintReadOnly)
	float NumberOfSegments = 0;

	UPROPERTY(BlueprintReadOnly)
	bool TreeDead = false;
	UPROPERTY(BlueprintReadOnly)
	bool TreeTooBig = false;
	
	UPROPERTY(EditAnywhere, Category="Segments")
	int MaxSegments = 5000;
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
	UPROPERTY(EditAnywhere, Category="Segments")
	float LeavesEnergyGain = 100.0f;
	UPROPERTY(EditAnywhere, Category="Segments")
	float DistanceFromRootBranchCostMultiplier = 1.5f;

	UPROPERTY()
	UProceduralMeshComponent* Mesh;

	static bool FloatToBool(const float F);
	
protected:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
private:
	static FVector GetRandomDirection(const FVector& From, const float MinAngle, const float MaxAngle);
	static FVector GetDirection(const FVector& From, const float MinAngle, const float MaxAngle, const float Angle);
};
