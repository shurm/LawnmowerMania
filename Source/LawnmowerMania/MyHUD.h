// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "MyHUD.generated.h"

/**
 * 
 */


UCLASS()
class LAWNMOWERMANIA_API AMyHUD : public AHUD
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> main_widget_bp;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> button_widget_bp;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> return_widget_bp;
	UPROPERTY(EditAnywhere)
		FString levelStatusFileName = "level_statuses.txt";

	UPROPERTY()
		UWidget* Loading_Text;

	bool loadGamePlayLevel;
	
	UFUNCTION()
		void LoadLevel(FString fileName);
};
