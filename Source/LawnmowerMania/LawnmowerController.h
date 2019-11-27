// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlayHUD.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Director.h"
#include "Sound/SoundCue.h"
#include "GamePlayHUD.h"
#include "LawnmowerController.generated.h"



UCLASS()
class LAWNMOWERMANIA_API ALawnmowerController : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY()
		AGamePlayHUD* hud;
	UPROPERTY()
		float timeLeftInSeconds;
public:
	// Sets default values for this pawn's properties
	ALawnmowerController();

	UPROPERTY(EditAnywhere)
		float initSpeed;

	UPROPERTY(EditAnywhere)
		float swipeThreshold;

	UPROPERTY()
		float speed;

	UPROPERTY(EditAnywhere)
		float totalTimeInSeconds;

	UPROPERTY()
		float lastCutTime;

	UPROPERTY()
		float cutTickTime;

	UPROPERTY()
		ADirector* director;

	UPROPERTY()
		FVector current_forward_direction;

	UPROPERTY()
		FVector original_forward_direction;

	UPROPERTY()
		FVector original_position;

	UPROPERTY(EditAnywhere)
		bool paused;

	UPROPERTY(EditAnywhere)
		float speedDelta;

	UPROPERTY()
		bool notMoving;

	UPROPERTY()
		int current_row;

	UPROPERTY()
		int current_col;

	UPROPERTY()
		bool changedTile;

	UPROPERTY()
		int fertNum;

	UPROPERTY()
		int wkNum;

	UPROPERTY(EditAnywhere)
		USoundCue* crashSfx;

	UPROPERTY(EditAnywhere)
		USoundCue* fertilizerSfx;

	UPROPERTY(EditAnywhere)
		USoundCue* weedkillerSfx;

	UPROPERTY(EditAnywhere)
		USoundCue* lawnmowerStartupSfx;

	UPROPERTY(EditAnywhere)
		USoundCue* lawnmowerLoopSfx;

	UPROPERTY()
		UAudioComponent* lawnmowerLoopComponent;

	UPROPERTY(EditAnywhere)
		USoundCue* itemSfx;

	UPROPERTY()
		float fingerX;

	UPROPERTY()
		float fingerY;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
		void Reset();

	UFUNCTION(BlueprintCallable)
		void Pause();

	UFUNCTION(BlueprintCallable)
		void Sprint();

	UFUNCTION(BlueprintCallable)
		void StopSprint();

	void LookAt(FVector& direction);

	void OnTouchBegin(ETouchIndex::Type touchType, FVector newTouchLocation);

	void OnTouchEnd(ETouchIndex::Type touchType, FVector newTouchLocation);

	void GoForward();

	void GoBackward();

	void GoLeft();

	void GoRight();

	void EndLevel();

	void getFertilizer();

	void getWeedkiller();

	void useFertilizer();

	void useWeedkiller();

	void StoreFingerPosition();
	
	void CheckForSwipe();
};
