// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "DoorsCheatManager.generated.h"

class UMaterialParameterCollection;

/**
 * Custom cheat manager to allow for extra debug actions.
 */
UCLASS()
class DOORS_API UDoorsCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Doors")
	TObjectPtr<UMaterialParameterCollection> NavModifierOverrideVolumeDebugMPC;
	
	UFUNCTION(exec, BlueprintCallable, Category="Doors")
	void ShowNavModifierOverrideVolumeDebug();

	UFUNCTION(exec, BlueprintCallable, Category="Doors")
	void HideNavModifierOverrideVolumeDebug();
};
