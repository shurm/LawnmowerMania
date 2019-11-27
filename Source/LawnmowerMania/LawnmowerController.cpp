#include "LawnmowerController.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "Components/AudioComponent.h"



// Sets default values
ALawnmowerController::ALawnmowerController()
{
	paused = true;
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	lawnmowerLoopComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("lawnmowerComp"));
	lawnmowerLoopComponent->bAutoActivate = false;
	lawnmowerLoopComponent->AutoAttachParent = this->RootComponent;
}


// Called when the game starts or when spawned
void ALawnmowerController::BeginPlay()
{
	Super::BeginPlay();

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "lawnmower begin play");

	original_position = this->GetActorLocation();

	original_forward_direction = this->GetActorForwardVector();

	hud = Cast<AGamePlayHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	
	lawnmowerLoopComponent->SetSound(lawnmowerLoopSfx);

	speed = initSpeed;

	cutTickTime = 5.f;

	
}


//resets the grass levels
void ALawnmowerController::Reset()
{
	
	if (director == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "director is NULL!! Error");
		return;

	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "Reset called");

	paused = true;
	if (hud != nullptr)
	{ 
		hud->SetFertilizerNum(0);
		hud->SetWeedKillerNum(0);
	}
	director->Reset();

	this->SetActorLocation(original_position);

	current_col = 0;

	current_row = 0;

	current_forward_direction = original_forward_direction;

	notMoving = true;

	timeLeftInSeconds = totalTimeInSeconds;

	lastCutTime = timeLeftInSeconds;

	changedTile = false;

	fertNum = 0;

	wkNum = 0;

	speed = initSpeed;

	paused = false;

	lawnmowerLoopComponent->Stop();

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Press arrow keys to start!");
}

void ALawnmowerController::Pause()
{
	if (director->gameOver)
		return;

	if (paused)
	{
		paused = false;
		lawnmowerLoopComponent->SetPaused(false);
	}
	else
	{
		paused = true;
		lawnmowerLoopComponent->SetPaused(true);
	}
		
}

void ALawnmowerController::Sprint()
{
	if (paused || notMoving)
		return;
	speed *= 3.f;

	
}

void ALawnmowerController::StopSprint()
{
	if (paused || notMoving)
		return;
	speed /= 3.f;

}


// Called every frame
void ALawnmowerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LookAt(current_forward_direction);

	if (paused)
		return;

	timeLeftInSeconds -= DeltaTime;
	int seconds = (int)timeLeftInSeconds;
	FString time = FString::Printf(TEXT("%d"), seconds);

	if (hud!= nullptr)
		hud->DisplayTimer(time);
	//GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, FString::Printf(TEXT("Time Left: %d"), seconds));

	if (seconds <= 0)
		EndLevel();

	if (notMoving)
		return;

	if (lastCutTime - timeLeftInSeconds > cutTickTime)
	{
		director->ShaveGrass(current_row, current_col);
		lastCutTime = timeLeftInSeconds;
		
		if (director->AllCut())
		{
			EndLevel();
		}
	}

	FString speedText = FString::Printf(TEXT("%.2f"), speed);
	if (hud != nullptr)
		hud->DisplaySpeed(speedText);
	
	FVector newLocation = this->GetActorLocation() + current_forward_direction * speed;
	//newLocation = FVector(newLocation.X, 136, 130);
	//this->SetActorLocation(newLocation);

	int next_row = current_row, next_col = current_col;

	float x = this->GetActorLocation().X, y = this->GetActorLocation().Y;

	FVector currentGrassSquareLocation = director->GetTileLocation(current_row, current_col);
	
	//going up
	if (current_forward_direction == FVector::RightVector)
	{
		next_col = current_col + 1;
		x = currentGrassSquareLocation.X;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "RightVector");

	}
	else if (current_forward_direction == FVector::LeftVector)
	{
		next_col = current_col - 1;
		x = currentGrassSquareLocation.X;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "LeftVector");


	}
	else if (current_forward_direction == FVector::ForwardVector)
	{
		next_row = current_row + 1;
		y = currentGrassSquareLocation.Y;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "ForwardVector");

	}
	else if (current_forward_direction == FVector::BackwardVector)
	{
		next_row = current_row - 1;
		y = currentGrassSquareLocation.Y;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "BackwardVector");
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "NONE!!!");
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("x: %f , y: %f"), x,y));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("current_col: %d"), current_col));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("current_row: %d"), current_row));

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("next_row: %d"), next_row));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("next_col: %d"), next_col));

	FVector currentLocation = FVector(x, y, original_position.Z);


	float dist_1, dist_2;

	newLocation = currentLocation + this->GetActorForwardVector()*speed;
	
	//if next grass tile is out of bounds or blocked by an obstacle
	if (director->IsBlocked(next_row, next_col))
	{
		if (FVector::Distance(currentGrassSquareLocation, newLocation) > FVector::Distance(currentGrassSquareLocation, currentLocation))
		{
			this->SetActorLocation(FVector(currentGrassSquareLocation.X, currentGrassSquareLocation.Y, original_position.Z));
			if (changedTile)
			{
				/*
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "You Crashed!");
				speed -= 1.f;
				if (speed <= 0.f)
				{
					EndLevel();
				}
				*/
				changedTile = false;
			}
		}

		else
			this->SetActorLocation(newLocation);
		return;	
	}
	
	this->SetActorLocation(newLocation);
	
	
	dist_1 = FVector::Distance(currentGrassSquareLocation, newLocation);
	FVector newTileLocation = director->GetTileLocation(next_row, next_col);
	dist_2 = FVector::Distance(newTileLocation, newLocation);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("dist_1: %.6f"), dist_1));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("dist_2: %.6f"), dist_2));

	if (dist_1 > dist_2)
	{
		current_row = next_row;
		current_col = next_col;

		director->ShaveGrass(current_row, current_col);

		lastCutTime = timeLeftInSeconds;

		director->getPowerup(current_row, current_col);
		
		if (director->AllCut())
		{
			EndLevel();
		}
		changedTile = true;
	}
}

// Called to bind functionality to input
void ALawnmowerController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "SetupPlayerInputComponent");

	
	InputComponent->BindAction("MoveUp", EInputEvent::IE_Pressed, this, &ALawnmowerController::GoForward);
	InputComponent->BindAction("MoveDown", EInputEvent::IE_Pressed, this, &ALawnmowerController::GoBackward);
	InputComponent->BindAction("MoveLeft", EInputEvent::IE_Pressed, this, &ALawnmowerController::GoLeft);
	InputComponent->BindAction("MoveRight", EInputEvent::IE_Pressed, this, &ALawnmowerController::GoRight);
	
	InputComponent->BindAction("Reset", EInputEvent::IE_Pressed, this, &ALawnmowerController::Reset);

	InputComponent->BindAction("UseFertilizer", EInputEvent::IE_Pressed, this, &ALawnmowerController::useFertilizer);
	InputComponent->BindAction("UseWeedkiller", EInputEvent::IE_Pressed, this, &ALawnmowerController::useWeedkiller);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &ALawnmowerController::Sprint);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &ALawnmowerController::StopSprint);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "Reset set");

	InputComponent->BindAction("Finger1", EInputEvent::IE_Pressed, this, &ALawnmowerController::StoreFingerPosition);
	InputComponent->BindAction("Finger1", EInputEvent::IE_Released, this, &ALawnmowerController::CheckForSwipe);

	InputComponent->BindAction("Mouse", EInputEvent::IE_Pressed, this, &ALawnmowerController::StoreFingerPosition);
	InputComponent->BindAction("Mouse", EInputEvent::IE_Released, this, &ALawnmowerController::CheckForSwipe);

}
void ALawnmowerController::StoreFingerPosition()
{
	//bool f;
	//GetWorld()->GetFirstPlayerController()->GetInputTouchState(ETouchIndex::Touch1, fingerX, fingerY, f);
	GetWorld()->GetFirstPlayerController()->GetMousePosition(fingerX, fingerY);
}
void ALawnmowerController::CheckForSwipe()
{
	float newFingerX, newFingerY;
	//bool f;
	//GetWorld()->GetFirstPlayerController()->GetInputTouchState(ETouchIndex::Touch1, newFingerX, newFingerY, f);
	GetWorld()->GetFirstPlayerController()->GetMousePosition(newFingerX, newFingerY);
	TMap<FString, FString> directionMap;

	directionMap.Add(TEXT("01"), TEXT("Down"));
	directionMap.Add(TEXT("0-1"), TEXT("Up"));
	directionMap.Add(TEXT("10"), TEXT("Right"));
	directionMap.Add(TEXT("-10"), TEXT("Left"));

	float deltaX = newFingerX - fingerX, deltaY = newFingerY - fingerY;
	if (abs(deltaX) < swipeThreshold && abs(deltaY) < swipeThreshold)
		return;
	int x, y;
	if (abs(deltaX) < abs(deltaY))
	{
		x = 0;
		y = (int)(deltaY / abs(deltaY));
	}
	else
	{
		x = (int)(deltaX / abs(deltaX));
		y = 0;
	}
	
	FString key = FString::FromInt(x) + FString::FromInt(y);
	
	FString direction = directionMap[key];

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "key is " + key);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "direction is "+ direction);

	if (direction.Equals("Up"))
	{
		GoRight();
	}
	else if (direction.Equals("Down"))
	{
		GoLeft();
	}
	else if (direction.Equals("Left"))
	{
		
		GoBackward();
	}
	else if (direction.Equals("Right"))
	{
		
		GoForward();
	}
}

void ALawnmowerController::GoForward()
{
	if (paused)
		return;

	if (notMoving)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), lawnmowerStartupSfx, GetActorLocation());
		lawnmowerLoopComponent->Play();
		notMoving = false;
	}

	if (current_forward_direction != FVector::RightVector)
	{
		current_forward_direction = FVector::RightVector;
		lastCutTime = timeLeftInSeconds;
	}

}

void ALawnmowerController::GoBackward()
{
	if (paused)
		return;

	if (notMoving)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), lawnmowerStartupSfx, GetActorLocation());
		lawnmowerLoopComponent->Play();
		notMoving = false;
	}
	if (current_forward_direction != FVector::LeftVector)
	{
		current_forward_direction = FVector::LeftVector;
		lastCutTime = timeLeftInSeconds;
	}
}

void ALawnmowerController::GoLeft()
{
	if (paused)
		return;

	if (notMoving)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), lawnmowerStartupSfx, GetActorLocation());
		lawnmowerLoopComponent->Play();
		notMoving = false;
	}
	if (current_forward_direction != FVector::BackwardVector)
	{
		current_forward_direction = FVector::BackwardVector;
		lastCutTime = timeLeftInSeconds;
	}
}

void ALawnmowerController::GoRight()
{
	if (paused)
		return;

	if (notMoving)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), lawnmowerStartupSfx, GetActorLocation());
		lawnmowerLoopComponent->Play();
		notMoving = false;
	}
	if (current_forward_direction != FVector::ForwardVector)
	{
		current_forward_direction = FVector::ForwardVector;
		lastCutTime = timeLeftInSeconds;
	}
}

void ALawnmowerController::LookAt(FVector& direction)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "pressed something");

	FRotator lookAtRotator = FRotationMatrix::MakeFromX(direction).Rotator();
	this->SetActorRotation(lookAtRotator);
}

void ALawnmowerController::EndLevel()
{
	paused = true;
	lawnmowerLoopComponent->Stop();
	director->EndLevel();
}

void ALawnmowerController::getFertilizer()
{
	fertNum += 3;
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), itemSfx, GetActorLocation());
	if (hud != nullptr)
		hud->SetFertilizerNum(fertNum);
}

void ALawnmowerController::getWeedkiller()
{
	wkNum += 3;
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), itemSfx, GetActorLocation());
	if (hud != nullptr)
		hud->SetWeedKillerNum(wkNum);
}

void ALawnmowerController::useFertilizer()
{
	if (fertNum == 0)
		return;

	if (director->placeFertilizer(current_row, current_col))
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), fertilizerSfx, GetActorLocation());
		fertNum--;
		if (hud != nullptr)
			hud->SetFertilizerNum(fertNum);
	}
}
	

void ALawnmowerController::useWeedkiller()
{
	if (wkNum == 0)
		return;
	if (director->placeWeedkiller(current_row, current_col))
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), weedkillerSfx, GetActorLocation());
		wkNum--;
		if (hud != nullptr)
			hud->SetWeedKillerNum(wkNum);
		if (director->AllCut())
		{
			EndLevel();
		}
	}
	
	
}

