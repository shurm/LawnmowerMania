// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyHUD.h"
#include "MyProxy.generated.h"

/**
 * 
 */
UCLASS()
class LAWNMOWERMANIA_API UMyProxy : public UObject
{
	GENERATED_BODY()

public:
	AMyHUD* OwnerPtr;
	FString data;

	UFUNCTION()
		void MyProxyHandler();
	
};
