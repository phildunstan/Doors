﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavLinkCustomComponent.h"
#include "DoorNavLinkComponent.generated.h"

// Derive from UNavLinkCustomComponent so that we can change the obstacle nav area under the nav link from blueprint

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DOORS_API UDoorNavLinkComponent : public UNavLinkCustomComponent
{
	GENERATED_BODY()

public:
	UDoorNavLinkComponent();
	
	virtual bool IsNavigationRelevant() const override { return true; }
	
	UFUNCTION(BlueprintCallable)
	void SetObstacleNavArea(TSubclassOf<UNavArea> NavAreaClass);
	
	UFUNCTION(BlueprintCallable)
	void EnableNavLink();
	
	UFUNCTION(BlueprintCallable)
	void DisableNavLink();
};
