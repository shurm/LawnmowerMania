// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "GamePlayHUD.h"
#include "Camera/CameraActor.h"
#include "Director.generated.h"

USTRUCT()
struct FActor2DArray {
	GENERATED_BODY()
public:

	TArray<AActor*> Ar;

	AActor* operator[] (int32 i) {
		return Ar[i];
	}

	void Add(AActor* rune) {
		Ar.Add(rune);
	}
};

USTRUCT()
struct FBoolean2DArray {
	GENERATED_BODY()
public:

	TArray<bool> Ar;

	bool operator[] (int32 i) {
		return Ar[i];
	}

	void Add(bool rune) {
		Ar.Add(rune);
	}
};

USTRUCT()
struct FInt2DArray {
	GENERATED_BODY()
public:

	TArray<int> Ar;

	int operator[] (int32 i) {
		return Ar[i];
	}
	void Set(int32 i, int32 e) {
		Ar[i] = e;
	}

	void Add(int rune) {
		Ar.Add(rune);
	}
};

UCLASS()
class LAWNMOWERMANIA_API ADirector : public AActor
{
	GENERATED_BODY()


public:	
	// Sets default values for this actor's properties
	ADirector();

	UPROPERTY()
		AGamePlayHUD* hud;

	UPROPERTY()
		bool gameOver;

	UPROPERTY(EditAnywhere)
		float passingPercent;

	UPROPERTY(EditAnywhere)
		TArray<UMaterial*> grass_level_materials;

	UPROPERTY(EditAnywhere)
		TArray<UStaticMesh*> grass_level_meshes;

	UPROPERTY(EditAnywhere)
		UMaterial* blockedMaterial;

	UPROPERTY(EditAnywhere)
		UStaticMesh* blockedMesh;

	UPROPERTY(EditAnywhere)
		UMaterial* rockMaterial;

	UPROPERTY(EditAnywhere)
		TArray<UMaterial*> indicator_level_materials;

	UPROPERTY(EditAnywhere)
		FString levelFileName;

	UPROPERTY()
		TArray<FInt2DArray> current_grass_levels;

	UPROPERTY()
		TArray<FInt2DArray> original_grass_levels;

	UPROPERTY()
		TArray<FActor2DArray> grid;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ALawnmowerController> lawnmower_bp;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AActor>  grass_bp;

	
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AActor>  fence_bp;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AActor> fertilizer_bp;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AActor> weedkiller_bp;

	UPROPERTY(EditAnywhere)
		float space_between;

	UPROPERTY(EditAnywhere)
		float tile_size;

	UPROPERTY(EditAnywhere)
		float fence_z;

	UPROPERTY(EditAnywhere)
		float lawnmover_z;
	
	UPROPERTY(EditAnywhere)
		int totalRows;

	UPROPERTY(EditAnywhere)
		int totalCols;

	UPROPERTY(EditAnywhere)
		int startingRow;
		
	UPROPERTY(EditAnywhere)
		int startingCol;

	UPROPERTY(EditAnywhere)
		float timer;

	UPROPERTY(EditAnywhere)
		ALawnmowerController* lawnmower;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACameraActor> camera_bp;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void Reset();

	void DisplayGrassLevel(int row, int col);

	FVector GetTileLocation(int row, int col);

	bool IsBlocked(int row, int col);

	void ShaveGrass(int row, int col);

	bool AllCut();

	float CompletionPercentage();

	void EndLevel();

	void getPowerup(int row, int col);

	bool placeFertilizer(int row, int col);

	bool placeWeedkiller(int row, int col);

private:
	TMap<int32, TTuple<FString, AActor*>> resourceMap;
};
