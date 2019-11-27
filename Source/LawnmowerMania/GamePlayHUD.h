// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Components/TextBlock.h"
#include "Components/Widget.h"
#include "Styling/SlateColor.h"
#include "Internationalization/Text.h"
#include "GamePlayHUD.generated.h"

/**
 * 
 */
UCLASS()
class LAWNMOWERMANIA_API AGamePlayHUD : public AHUD
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> main_widget_bp;
	
	UPROPERTY(EditAnywhere)
		FLinearColor failureColor;
	UPROPERTY(EditAnywhere)
		FLinearColor successColor;
		
	UPROPERTY(EditAnywhere)
		FText failureMessage;
	UPROPERTY(EditAnywhere)
		FText successMessage;
	UPROPERTY(EditAnywhere)
		FText perfectSuccessMessage;
		

	void DisplayTimer(FString timerText);
	void DisplaySpeed(FString timerText);
	void ChangeToSuccessColor();
	void DisplayEndgameFeedback(float completionPercent);
	void Reset();
	void SetPerfectMessage();

	void SetPassingMessage();

	void SetFertilizerNum(int num);

	void SetWeedKillerNum(int num);

	void AddNewCompletedLevelToStatusFile(FString status);

private:
	bool won;
	UTextBlock* uiTimerText;
	UTextBlock* speedText;

	UTextBlock* playerFeedbackMessage;
	UTextBlock* percentCompleteText;
	UTextBlock* clarifyingText;

	UTextBlock* Num_Fertilizer;
	UTextBlock* Num_WeedKiller;

	UWidget* endgamePanel;


	
};
