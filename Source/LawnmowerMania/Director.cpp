// Fill out your copyright notice in the Description page of Project Settings.


#include "Director.h"
#include "LawnmowerController.h"
#include "FileLoader1.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ADirector::ADirector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADirector::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* p = GetWorld()->GetFirstPlayerController();
	hud = Cast<AGamePlayHUD>(p->GetHUD());
	gameOver = false;

	AActor* cam = GetWorld()->SpawnActor<AActor>(camera_bp, FVector(0, 0, 800.f), FRotator::MakeFromEuler(FVector(0, -90.f, 0)));

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, p->Data);
	//levelFileName = p->Data;
	FString chosenLevel = FileLoader1::LoadFile("test.txt");

	FString fileText = FileLoader1::LoadLevelFile(chosenLevel);
	TArray<FString> fileLines;
	fileText.ParseIntoArrayLines(fileLines);
	
	TArray<FString> line;
	fileLines[0].ParseIntoArray(line, TEXT(","));
	
	totalRows = FCString::Atoi(*line[0]);
	totalCols = FCString::Atoi(*line[1]);
	timer = FCString::Atof(*line[2]);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, line[0]);
	cam->SetActorLocation(FVector(0, 0, 300.f + 120.f * totalRows));

	FVector upperLeftCorner(totalRows/-2.0*(tile_size + space_between) + tile_size/2.0, totalCols/-2.0*(tile_size + space_between) + tile_size / 2.0, 0);
	FVector spawnLocation;

	int currentLineIndex = 1;
	for (int r = 0; r < totalRows; r++)
	{
		FActor2DArray row = FActor2DArray();
		for (int c = 0; c < totalCols; c++)
		{
			spawnLocation = FVector(upperLeftCorner.X + r*(tile_size + space_between), upperLeftCorner.Y + c*(tile_size + space_between), 0);

			AActor* NewActor = GetWorld()->SpawnActor<AActor>(grass_bp, spawnLocation, FRotator::ZeroRotator);
			
			row.Add(NewActor);
		}
		grid.Add(row);
	}

	for (int r = 0; r < totalRows; r++)
	{
		fileLines[r+currentLineIndex].ParseIntoArray(line, TEXT(","));

		FInt2DArray row = FInt2DArray();
		for (int c = 0; c < totalCols; c++)
		{
			row.Add(FCString::Atoi(*line[c]));
		}

		original_grass_levels.Add(row);
	}

	for (int r = 0; r < totalRows; r++)
	{
		FInt2DArray row = FInt2DArray();
		for (int c = 0; c < totalCols; c++)
		{
			row.Add(0);
		}
		current_grass_levels.Add(row);
	}


	


	upperLeftCorner = FVector(totalRows / -2.0*(tile_size + space_between) - space_between/2, totalCols / -2.0*(tile_size + space_between) + tile_size/2, 0);

	FRotator lookAtRotator = FRotator::MakeFromEuler(FVector(0.0f, 0.0f, -90.0f));

	currentLineIndex += totalRows;
	for (int r = 0; r < totalRows + 1; r++)
	{
		fileLines[r + currentLineIndex].ParseIntoArray(line, TEXT(","));

		for (int c = 0; c < totalCols; c++)
		{
			if (line[c].Equals("t"))
			{
				spawnLocation = FVector(upperLeftCorner.X + r * (tile_size + space_between), upperLeftCorner.Y + c * (tile_size + space_between), fence_z);
				AActor* NewActor = GetWorld()->SpawnActor<AActor>(fence_bp, spawnLocation, lookAtRotator);
			}
		}
	}

	upperLeftCorner = FVector(totalRows / -2.0*(tile_size + space_between) + tile_size/2.0, totalCols / -2.0*(tile_size + space_between) - space_between / 2, 0);

	
	lookAtRotator = FRotator::ZeroRotator;
	currentLineIndex += (totalRows+1);
	for (int r = 0; r < totalRows; r++)
	{
		fileLines[r + currentLineIndex].ParseIntoArray(line, TEXT(","));

		for (int c = 0; c < totalCols + 1; c++)
		{
			if (line[c].Equals("t"))
			{
				spawnLocation = FVector(upperLeftCorner.X + r * (tile_size + space_between), upperLeftCorner.Y + c * (tile_size + space_between), fence_z);
				AActor* NewActor = GetWorld()->SpawnActor<AActor>(fence_bp, spawnLocation, lookAtRotator);
			}
		}
	}
	currentLineIndex += totalRows;
	
	
	while (currentLineIndex < fileLines.Num())
	{
		fileLines[currentLineIndex].ParseIntoArray(line, TEXT(","));
		
		FString type = line[0];

		int32 row = FCString::Atoi(*line[1])-1;
		int32 col = FCString::Atoi(*line[2])-1;

		if (row < 0 || col < 0)
			break;
		int32 key = row * totalCols + col;

		
		spawnLocation = GetTileLocation(row, col);
		spawnLocation.Z = fence_z + 30.f;


		TSubclassOf<class AActor> resourceBlueprint = fertilizer_bp;
		FVector rotVec = FVector(0, 0, 90.f);
		if (type.Equals(FString("Weedkiller")))
		{
			resourceBlueprint = weedkiller_bp;
			rotVec[2] = 0;
		}
				

		AActor* resource = GetWorld()->SpawnActor<AActor>(resourceBlueprint, spawnLocation, FRotator::MakeFromEuler(rotVec));
		TTuple<FString, AActor*> tuple = MakeTuple(type, resource);

		resourceMap.Add(key, tuple);
		
		currentLineIndex++;
	}
	
	Reset();




	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("grid len: %d"), grid.Num()));
	upperLeftCorner = FVector(totalRows / -2.0*(tile_size + space_between), totalCols / -2.0*(tile_size + space_between) + tile_size / 2, 0);

	lookAtRotator = FRotator::MakeFromEuler(FVector(0.0f, 0.0f, -90.0f));

	AActor* NewActor;
	FVector newScale = FVector(1.0f, 0.5f, 1.0f);
	for (int r = 0; r < totalRows; r++)
	{
		for (int c = 0; c < totalCols; c++)
		{
			if (original_grass_levels[r][c] < 0)
			{
				spawnLocation = FVector(upperLeftCorner.X + r * (tile_size + space_between), upperLeftCorner.Y + c * (tile_size + space_between), fence_z);
				NewActor = GetWorld()->SpawnActor<AActor>(fence_bp, spawnLocation, lookAtRotator);
				NewActor->SetActorScale3D(newScale);
				NewActor->SetActorHiddenInGame(true);

				spawnLocation = FVector(upperLeftCorner.X + (r+1) * (tile_size + space_between) - space_between , upperLeftCorner.Y + c * (tile_size + space_between), fence_z);
				NewActor = GetWorld()->SpawnActor<AActor>(fence_bp, spawnLocation, lookAtRotator);
				NewActor->SetActorScale3D(newScale);
				NewActor->SetActorHiddenInGame(true);
			}
		}
	}

	upperLeftCorner = FVector(totalRows / -2.0*(tile_size + space_between) + tile_size / 2.0, totalCols / -2.0*(tile_size + space_between), 0);


	lookAtRotator = FRotator::ZeroRotator;

	for (int r = 0; r < totalRows; r++)
	{
		
		for (int c = 0; c < totalCols; c++)
		{
			if (original_grass_levels[r][c] < 0)
			{
				spawnLocation = FVector(upperLeftCorner.X + r * (tile_size + space_between), upperLeftCorner.Y + c * (tile_size + space_between), fence_z);
				NewActor = GetWorld()->SpawnActor<AActor>(fence_bp, spawnLocation, lookAtRotator);
				NewActor->SetActorScale3D(newScale);
				NewActor->SetActorHiddenInGame(true);

				spawnLocation = FVector(upperLeftCorner.X + r * (tile_size + space_between), upperLeftCorner.Y + (c+1) * (tile_size + space_between)- space_between, fence_z);
				NewActor = GetWorld()->SpawnActor<AActor>(fence_bp, spawnLocation, lookAtRotator);
				NewActor->SetActorScale3D(newScale);
				NewActor->SetActorHiddenInGame(true);
			}
		}
	}


	spawnLocation = FVector(totalRows / -2.0*(tile_size + space_between) + tile_size / 2.0, totalCols / -2.0*(tile_size + space_between) + tile_size / 2.0, lawnmover_z);

	lawnmower = GetWorld()->SpawnActor<ALawnmowerController>(lawnmower_bp, spawnLocation, FRotator::ZeroRotator);
	lawnmower->director = this;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "director set");
	
	lawnmower->totalTimeInSeconds = timer;
	lawnmower->Reset();

}

// Called every frame
void ADirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADirector::Reset()
{
	for (int r = 0; r < totalRows; r++)
	{
		FInt2DArray row = current_grass_levels[r];
		for (int c = 0; c < totalCols; c++)
		{
			row.Set(c, original_grass_levels[r][c]);
		}
		current_grass_levels[r] = row;
	}

	for (int r = 0; r < totalRows; r++)
	{
		for (int c = 0; c < totalCols; c++)
		{
			DisplayGrassLevel(r, c);
		}
	}

	for (auto &entry : resourceMap)
	{
		TTuple<FString, AActor*> tuple = entry.Value;
		AActor* powerUp = tuple.Value;
		powerUp->SetActorHiddenInGame(false);
	}

	if (gameOver)
	{
		if(hud!=nullptr)
			hud->Reset();
	}

	gameOver = false;
}

FVector ADirector::GetTileLocation(int row, int col)
{
	return grid[row][col]->GetActorLocation();
}

bool ADirector::IsBlocked(int row, int col)
{
	//|| current_grass_levels[row][col] < 0
	if (row < 0 || row >= totalRows || col < 0 || col >= totalCols )
		return true;
	return false;
}

void ADirector::ShaveGrass(int row, int col)
{
	int current_level = current_grass_levels[row][col];
	current_level--;
	if (current_level < 0)
		return;

	current_grass_levels[row].Set(col, current_level);
	
	DisplayGrassLevel(row, col);
}

void ADirector::DisplayGrassLevel(int row, int col)
{
	TArray<UActorComponent*> meshes = grid[row][col]->GetComponentsByClass(UStaticMeshComponent::StaticClass());


	UStaticMeshComponent* cubeMesh = Cast<UStaticMeshComponent>(meshes[0]);
	UStaticMeshComponent* grassMesh = Cast<UStaticMeshComponent>(meshes[1]);
	UStaticMeshComponent* indicatorMesh = Cast<UStaticMeshComponent>(meshes[2]);

	int current_level = current_grass_levels[row][col];
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("current_level: %d"), current_level));

	UMaterial* current_grass_material = blockedMaterial;
	UStaticMesh* current_grass_mesh = blockedMesh;
	if (current_level >= 0)
	{
		current_grass_material = grass_level_materials[current_level];
		current_grass_mesh = grass_level_meshes[current_level];

		indicatorMesh->SetMaterial(0, indicator_level_materials[current_level]);
	}
	cubeMesh->SetMaterial(0, current_grass_material);
	grassMesh->SetStaticMesh(current_grass_mesh);

	if (current_level < 0)
		grassMesh->SetMaterial(0, rockMaterial);


}

bool ADirector::AllCut()
{
	bool allCut = true;
	for (int r = 0; r < totalRows; r++)
	{
		for (int c = 0; c < totalCols; c++)
		{
			if (current_grass_levels[r][c] > 1)
			{
				allCut = false;
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("R: %i, C: %i, Level: %d"), r, c, current_grass_levels[r][c]));
			}
		}
	}
	return allCut;
}

float ADirector::CompletionPercentage()
{
	float totalTiles = totalRows * totalCols;
	float cutTiles = 0.f;
	for (int r = 0; r < totalRows; r++)
	{
		for (int c = 0; c < totalCols; c++)
		{
			if (current_grass_levels[r][c] == 1)
			{
				cutTiles++;
			}
			else if (current_grass_levels[r][c] < 0) // obstacles dont count as cuttable tiles
			{
				totalTiles--;
			}
		}
	}
	return cutTiles / totalTiles;
}

void ADirector::EndLevel()
{
	gameOver = true;
	float completionPercent = CompletionPercentage();
	if (hud == nullptr)
		return;
	FString status = "";
	if (completionPercent >= passingPercent)
	{
		hud->ChangeToSuccessColor();
		if (completionPercent == 1.0f)
		{
			hud->SetPerfectMessage();
			status = "perfect";
		}
		else
		{
			hud->SetPassingMessage();
			status = "decent";
		}
	}

	hud->DisplayEndgameFeedback(completionPercent);
	if(status.Len()>0)
		hud->AddNewCompletedLevelToStatusFile(status);
}

void ADirector::getPowerup(int row, int col)
{
	int32 key = row * totalCols + col;
	if (!resourceMap.Contains(key))
		return;
	TTuple<FString, AActor*> tuple = resourceMap[key];

	AActor* powerUp = tuple.Value;
	if (powerUp->bHidden)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Used before")));
		return;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("getting")));
	FString type = tuple.Key;

	if(type.Equals("Weedkiller"))
		lawnmower->getWeedkiller();
	else
		lawnmower->getFertilizer();

	powerUp->SetActorHiddenInGame(true);
	
}

bool ADirector::placeFertilizer(int row, int col)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("fert used")));

	int r = row;
	int c = col;
	if (r < 0)
	{
		r = 0;
	}
	else if (r >= totalRows)
	{
		r = totalRows - 1;
	}
	if (c < 0)
	{
		c = 0;
	}
	else if (c >= totalCols)
	{
		c = totalCols - 1;
	}
	int current_level = current_grass_levels[r][c];
	if (current_level >= 0 && current_level < 3)
	{
		current_level++;

		current_grass_levels[r].Set(c, current_level);

		DisplayGrassLevel(r, c);
		return true;
	}
			
	return false;
}

bool ADirector::placeWeedkiller(int row, int col)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("wk used")));

	int r = row;
	int c = col;
	if (r < 0)
	{
		r = 0;
	}
	else if (r >= totalRows)
	{
		r = totalRows - 1;
	}
	if (c < 0)
	{
		c = 0;
	}
	else if (c >= totalCols)
	{
		c = totalCols - 1;
	}
	int current_level = current_grass_levels[r][c];
	if (current_level > 0)
	{
		current_level--;
		current_grass_levels[r].Set(c, current_level);

		DisplayGrassLevel(r, c);
		return true;
	}
	return false;


}
