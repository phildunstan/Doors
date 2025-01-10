// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorNavLinkComponent.h"


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

