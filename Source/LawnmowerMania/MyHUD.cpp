// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "Blueprint/UserWidget.h"
#include "Components/PanelWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

#include "Internationalization/Text.h"
#include "HAL/FileManager.h"
#include "Paths.h"
#include "MyProxy.h"
#include "Kismet/GameplayStatics.h"
#include "FileLoader1.h"

void AMyHUD::BeginPlay()
{

	Super::BeginPlay();
	loadGamePlayLevel = false;

	FileLoader1::CreateIfNotExists(levelStatusFileName);


	APlayerController* p = GetWorld()->GetFirstPlayerController();
	p->bShowMouseCursor = true;
	UUserWidget* main_widget = CreateWidget<UUserWidget>(p, main_widget_bp);
	main_widget->AddToViewport();

	UUserWidget* return_widget = CreateWidget<UUserWidget>(p, return_widget_bp);
	return_widget->AddToViewport();
	
	//the displayable list of levels
	FName n = FName("JoyList");
	UPanelWidget* panel = Cast<UPanelWidget>(main_widget->GetWidgetFromName(n));
	
	n = FName("Loading_Text");
	Loading_Text = (main_widget->GetWidgetFromName(n));

	
	//reads the level status file (for completed levels)
	FString fileText = FileLoader1::LoadFile(levelStatusFileName);
	TArray<FString> fileLines;
	fileText.ParseIntoArrayLines(fileLines);

	TMap<FString, FString> levelStatusMap;

	for (int i = 0; i < fileLines.Num(); i++)
	{
		TArray<FString> line;
		fileLines[i].ParseIntoArray(line, TEXT(","));

		levelStatusMap.Add(line[0],FString(""));
	}
	

	//stores all of the file in the Levels directory
	IFileManager& FileManager = IFileManager::Get();
	TArray< FString > FoundFiles;
	FString directory = FPaths::ProjectContentDir() + "/" + "Levels" + "/";
	FileManager.FindFiles(FoundFiles, *directory, NULL);
	
	for (int i = 0;i<FoundFiles.Num();i++)
	{
		FString filestring = FoundFiles[i];
		UUserWidget* button_widget = CreateWidget<UUserWidget>(p, button_widget_bp);

		UPanelWidget* canvas = Cast<UPanelWidget>(button_widget->GetRootWidget());
		UButton* button = Cast<UButton>(canvas->GetChildAt(0));

		//sets the onClick
		
		UMyProxy* proxy = NewObject< UMyProxy >(this);
		proxy->OwnerPtr = this;
		proxy->data = filestring;

		button->OnClicked.AddDynamic(proxy, &UMyProxy::MyProxyHandler);
		
		UPanelWidget* buttonInfo = Cast<UPanelWidget>(button->GetChildAt(0));


		UTextBlock* buttonText = Cast<UTextBlock>(buttonInfo->GetChildAt(0));

		buttonText->SetText(FText::AsNumber(i+1));
		UImage* buttonImage = Cast<UImage>(buttonInfo->GetChildAt(1));

		if(!levelStatusMap.Contains(filestring))
			buttonImage->SetVisibility(ESlateVisibility::Hidden);

		panel->AddChild(button);

	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "pressed something");

	
}
void AMyHUD::Tick(float DeltaTime)
{
	if (loadGamePlayLevel)
	{
		UGameplayStatics::OpenLevel(GetWorld(), "GamePlayLevel", true);
	}
}
void AMyHUD::LoadLevel(FString fileName)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "visiible!!");
	
	Loading_Text->SetVisibility(ESlateVisibility::Visible);

	FileLoader1::SaveFile("test.txt",fileName);

	loadGamePlayLevel = true;
}
