// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProxy.h"

void UMyProxy::MyProxyHandler()
{
	OwnerPtr->LoadLevel(data);
}
