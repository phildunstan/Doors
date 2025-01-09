﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DoorInterface.generated.h"

UINTERFACE()
class UDoorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DOORS_API IDoorInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsDoorOpen() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OpenDoor();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void CloseDoor();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ToggleDoorOpenOrClosed();
};