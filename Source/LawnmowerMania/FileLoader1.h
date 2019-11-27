// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class LAWNMOWERMANIA_API FileLoader1
{
public:
	FileLoader1();

	static FString LoadFile(FString filename);

	static FString LoadLevelFile(FString filename);

	static void CreateIfNotExists(FString filename);

	static void SaveFile(FString filename, FString contents);

	~FileLoader1();
};
