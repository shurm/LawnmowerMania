// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayHUD.h"
#include "Blueprint/UserWidget.h"
#include "Components/PanelWidget.h"
#include "Components/Button.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "HAL/FileManager.h"
#include "Paths.h"
#include "MyProxy.h"
#include "Kismet/GameplayStatics.h"
#include "FileLoader1.h"

void AGamePlayHUD::BeginPlay()
{

	Super::BeginPlay();
	APlayerController* p = GetWorld()->GetFirstPlayerController();
	p->bShowMouseCursor = true;
	UUserWidget* main_widget = CreateWidget<UUserWidget>(p, main_widget_bp);
	main_widget->AddToViewport();

	//the displayable list of levels
	FName n = FName("Timer");
	uiTimerText = Cast<UTextBlock>(main_widget->GetWidgetFromName(n));

	n = FName("Speed");
	speedText = Cast<UTextBlock>(main_widget->GetWidgetFromName(n));

	
	n = FName("EndgamePanel");
	endgamePanel = main_widget->GetWidgetFromName(n);

	n = FName("PlayerFeedbackMessage");
	playerFeedbackMessage = Cast<UTextBlock>(main_widget->GetWidgetFromName(n));

	n = FName("PercentCompleteText");
	percentCompleteText = Cast<UTextBlock>(main_widget->GetWidgetFromName(n));

	n = FName("ClarifyingText");
	clarifyingText = Cast<UTextBlock>(main_widget->GetWidgetFromName(n));

	n = FName("Num_Fertilizer");
	Num_Fertilizer = Cast<UTextBlock>(main_widget->GetWidgetFromName(n));

	n = FName("Num_WeedKiller");
	Num_WeedKiller = Cast<UTextBlock>(main_widget->GetWidgetFromName(n));

}

void AGamePlayHUD::DisplayEndgameFeedback(float completionPercent)
{
	FText t = FText::AsPercent(completionPercent);
	percentCompleteText->SetText(t);
	endgamePanel->SetVisibility(ESlateVisibility::Visible);
	playerFeedbackMessage->SetVisibility(ESlateVisibility::Visible);
}

void AGamePlayHUD::SetPerfectMessage()
{
	playerFeedbackMessage->SetText(perfectSuccessMessage);

}

void AGamePlayHUD::SetPassingMessage()
{
	playerFeedbackMessage->SetText(successMessage);

}

void AGamePlayHUD::ChangeToSuccessColor()
{
	//playerFeedbackMessage.setcol
	//FSlateColor g = FSlateColor::FSlateColor(successColor);
	percentCompleteText->SetShadowColorAndOpacity(successColor);
	clarifyingText->SetShadowColorAndOpacity(successColor);
	playerFeedbackMessage->SetShadowColorAndOpacity(successColor);

	won = true;

}

void AGamePlayHUD::Reset()
{
	won = false;
	//FSlateColor::FSlateColor(failureColor)
	percentCompleteText->SetShadowColorAndOpacity(failureColor);
	clarifyingText->SetShadowColorAndOpacity(failureColor);
	playerFeedbackMessage->SetShadowColorAndOpacity(failureColor);

	playerFeedbackMessage->SetText(failureMessage);
	playerFeedbackMessage->SetVisibility(ESlateVisibility::Hidden);
	percentCompleteText->SetText(FText::FromString(""));
	endgamePanel->SetVisibility(ESlateVisibility::Collapsed);
}

void AGamePlayHUD::DisplaySpeed(FString speed)
{
	if (speedText != nullptr)
		speedText->SetText(FText::FromString(speed));
}

void AGamePlayHUD::DisplayTimer(FString timerText)
{
	if(uiTimerText!=nullptr)
		uiTimerText->SetText(FText::FromString(timerText));
}

void AGamePlayHUD::SetFertilizerNum(int num)
{
	if (Num_Fertilizer != nullptr)
		Num_Fertilizer->SetText(FText::AsNumber(num));
}

void AGamePlayHUD::SetWeedKillerNum(int num)
{
	if (Num_WeedKiller != nullptr)
		Num_WeedKiller->SetText(FText::AsNumber(num));
}

void AGamePlayHUD::AddNewCompletedLevelToStatusFile(FString status)
{
	FString oldContents = FileLoader1::LoadFile("level_statuses.txt");
	FString levelJustPlayed = FileLoader1::LoadFile("test.txt");

	oldContents = oldContents+levelJustPlayed+","+status+"\n";

	FileLoader1::SaveFile("level_statuses.txt", oldContents);
}