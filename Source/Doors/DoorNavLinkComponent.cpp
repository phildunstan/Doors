// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorNavLinkComponent.h"

#include "NavigationSystem.h"


void RebuildNavMeshCommand()
{
	UWorld* World = GEngine->GetWorldContexts()[0].World();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid world context!"));
		return;
	}

	if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World))
	{
		NavSys->Build();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("NavMesh regeneration triggered!"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to find Navigation System!"));
	}
}
FAutoConsoleCommand RebuildNavMeshCmd(TEXT("RebuildNavMesh"), TEXT("Forces NavMesh regeneration."), FConsoleCommandDelegate::CreateStatic(RebuildNavMeshCommand));


UDoorNavLinkComponent::UDoorNavLinkComponent()
{
	SetNavigationRelevancy(true);
}

void UDoorNavLinkComponent::SetObstacleNavArea(TSubclassOf<UNavArea> NavAreaClass)
{
	ObstacleAreaClass = NavAreaClass;
	RefreshNavigationModifiers();
}

void UDoorNavLinkComponent::EnableNavLink()
{
	SetEnabled(true);
}

void UDoorNavLinkComponent::DisableNavLink()
{
	SetEnabled(false);
}

