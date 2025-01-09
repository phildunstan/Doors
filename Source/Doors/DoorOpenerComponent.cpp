// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorOpenerComponent.h"

#include "DoorInterface.h"
#include "DoorNavLinkComponent.h"
#include "NavLinkCustomInterface.h"
#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "NavMesh/NavMeshPath.h"
#include "Navigation/PathFollowingComponent.h"

UDoorOpenerComponent::UDoorOpenerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UDoorOpenerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	PathFollowingComponent = Cast<UPathFollowingComponent>(GetOwner()->GetComponentByClass(UPathFollowingComponent::StaticClass()));
	
	const APawn* MyPawn = Cast<APawn>(GetOwner());
	if (!MyPawn)
	{
		if (AController* MyController = Cast<AController>(GetOwner()))
		{
			MyPawn = MyController->GetPawn();
		}
	}
	if (MyPawn)
	{
		NavMovementInterface = MyPawn->FindComponentByInterface<INavMovementInterface>();
	}
}

// This function uses a very simple model of movement to project ahead on the path by TimeAheadOnPathSeconds
// This function is very tightly coupled to the implementation of the PathFollowingComponent
void UDoorOpenerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PathFollowingComponent.IsValid())
		return;
	if (!NavMovementInterface)
		return;
	
	if (PathFollowingComponent->HasValidPath())
	{
		if (AActor* DoorActor = IsApproachingClosedDoorNavLink())
		{
			check(DoorActor->Implements<UDoorInterface>());
			IDoorInterface::Execute_OpenDoor(DoorActor);
		}
	}
}


AActor* UDoorOpenerComponent::IsApproachingClosedDoorNavLink() const
{
	FNavPathSharedPtr Path = PathFollowingComponent->GetPath();
	if (!Path.IsValid())
		return nullptr;
	const TArray<FNavPathPoint>& PathPoints = Path->GetPathPoints();

	// Walk along the path segments until we reach TimeAheadOnPathSeconds ahead.
	// We can't easily project ahead by TimeAheadOnPathSeconds though since we don't know the actual speed or the
	// max speed of the character in the current movement context.
	// Instead, approximate the movement speed with the MaxCharacterSpeed value on this component.
	// We also assume that only one door needs to be triggered to be open in the immediate future.
	
	FVector CurrentLocation = NavMovementInterface->GetFeetLocation();
	uint32 MoveSegmentStartIndex = PathFollowingComponent->GetCurrentPathIndex();
	uint32 MoveSegmentEndIndex = PathFollowingComponent->GetNextPathIndex();
	float RemainingTimeAheadSeconds = TimeAheadOnPathSeconds;

	while (RemainingTimeAheadSeconds > 0.0f)
	{
		if (!PathPoints.IsValidIndex(MoveSegmentStartIndex))
			// There is no more path to follow
			return nullptr;
			
		FNavMeshNodeFlags SegmentFlags = FNavMeshNodeFlags(PathPoints[MoveSegmentStartIndex].Flags);
		if (SegmentFlags.IsNavLink())
		{
			// we are on a nav link segment
			const UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
			if (INavLinkCustomInterface* MoveSegmentCustomLink = NavSys->GetCustomLink(PathPoints[MoveSegmentStartIndex].CustomNavLinkId))
			{
				// if there is a door component attached to the object containing the INavLinkCustomInterface
				if (UActorComponent* NavLinkComponent = Cast<UActorComponent>(MoveSegmentCustomLink))
				{
					if (UDoorNavLinkComponent* DoorNavLinkComponent = Cast<UDoorNavLinkComponent>(NavLinkComponent->GetOwner()->FindComponentByClass(UDoorNavLinkComponent::StaticClass())))
					{
						// check that there is a door attached that implements UDoorInterface
						if (AActor* DoorActor = DoorNavLinkComponent->LinkedDoor.Get())
						{
							if (DoorActor->Implements<UDoorInterface>())
							{
								return DoorActor;
							}
						}
					}
				}
			}
		}

		const FVector SegmentEndLocation = *Path->GetPathPointLocation(MoveSegmentEndIndex);
		const float DistanceToSegmentEnd = FVector::Dist(CurrentLocation, SegmentEndLocation);
		RemainingTimeAheadSeconds = TimeAheadOnPathSeconds - DistanceToSegmentEnd / MaxCharacterSpeed;

		MoveSegmentStartIndex = MoveSegmentEndIndex;
		MoveSegmentEndIndex = MoveSegmentEndIndex + 1;
		CurrentLocation = SegmentEndLocation;
	}
	
	return nullptr;
}



