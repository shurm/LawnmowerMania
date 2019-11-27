// Fill out your copyright notice in the Description page of Project Settings.


#include "FileLoader1.h"
#include "Misc/Paths.h"
#include "FileHelper.h"

FileLoader1::FileLoader1()
{
}

FileLoader1::~FileLoader1()
{
}

FString FileLoader1::LoadLevelFile(FString filename)
{
	FString myFile = FPaths::ProjectContentDir() + "/" + "Levels" + "/" + filename;
	FString result;
	if (FPaths::FileExists(myFile)) {
		FFileHelper::LoadFileToString(result, *myFile);

	}
	return result;
}

void FileLoader1::CreateIfNotExists(FString filename)
{
	FString myFile = FPaths::ProjectContentDir() + "/" + filename;
	if (!FPaths::FileExists(myFile)) {
		FileLoader1::SaveFile(filename, "");

	}
}

FString FileLoader1::LoadFile(FString filename)
{
	FString myFile = FPaths::ProjectContentDir() + "/" +  filename;
	FString result;
	if (FPaths::FileExists(myFile)) {
		FFileHelper::LoadFileToString(result, *myFile);

	}
	return result;
}

void FileLoader1::SaveFile(FString filename, FString contents)
{
	FString myFile = FPaths::ProjectContentDir() + "/" + filename;

	FFileHelper::SaveStringToFile(contents, *myFile);
}