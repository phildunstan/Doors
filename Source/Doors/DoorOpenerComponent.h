// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UObject/WeakObjectPtr.h"

#include "DoorOpenerComponent.generated.h"

class INavMovementInterface;
class UPathFollowingComponent;

// This component looks ahead on the path that an AI character is following to determine if it is approaching a door.
// If so, it will try to trigger that door to open.
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DOORS_API UDoorOpenerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDoorOpenerComponent();

	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Doors")
	float TimeAheadOnPathSeconds = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Doors")
	float MaxCharacterSpeed = 6.0f;

private:
	UPROPERTY()
	TWeakObjectPtr<UPathFollowingComponent> PathFollowingComponent;
	
	INavMovementInterface* NavMovementInterface = nullptr;

	AActor* IsApproachingClosedDoorNavLink() const;
};