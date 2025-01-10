// Fill out your copyright notice in the Description page of Project Settings.

#include "DoorWithNavLink.h"

#include "DoorNavLinkComponent.h"
#include "NavigationSystem.h"
#include "NavLinkRenderingComponent.h"
#include "NavModifierComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "VisualLogger/VisualLogger.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DoorWithNavLink)

ADoorWithNavLink::ADoorWithNavLink(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	NavLinkComponent = CreateDefaultSubobject<UDoorNavLinkComponent>(TEXT("NavLinkComponent"));
	NavLinkComponent->SetMoveReachedLink(this, &ADoorWithNavLink::OnSmartLinkReached);

#if WITH_EDITORONLY_DATA
	NavLinkRenderingComponent = CreateDefaultSubobject<UNavLinkRenderingComponent>(TEXT("NavLinkRenderingComponent"));
	NavLinkRenderingComponent->SetupAttachment(RootComponent);
#endif // WITH_EDITORONLY_DATA
	
	SetActorEnableCollision(false);
	SetCanBeDamaged(false);
}

void ADoorWithNavLink::PostRegisterAllComponents()
{
	Super::PostRegisterAllComponents();

	if (RootComponent)
	{
		RootComponent->TransformUpdated.AddLambda([this](USceneComponent*, EUpdateTransformFlags, ETeleportType)
			{
				FNavigationSystem::UpdateActorData(*this);
			});
	}
}


#if ENABLE_VISUAL_LOG
void ADoorWithNavLink::BeginPlay()
{
	if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
	{
		REDIRECT_OBJECT_TO_VLOG(this, NavSys);
	}

	Super::BeginPlay();
}
#endif // ENABLE_VISUAL_LOG

bool ADoorWithNavLink::GetNavigationLinksClasses(TArray<TSubclassOf<UNavLinkDefinition>>& OutClasses) const
{
	return false;
}

bool ADoorWithNavLink::GetNavigationLinksArray(TArray<FNavigationLink>& OutLink, TArray<FNavigationSegmentLink>& OutSegments) const
{
	if (NavLinkComponent && NavLinkComponent->IsNavigationRelevant())
	{
		OutLink.Add(NavLinkComponent->GetLinkModifier());
		return true;
	}
	return false;
}

FBox ADoorWithNavLink::GetNavigationBounds() const
{
	return GetComponentsBoundingBox();
}

bool ADoorWithNavLink::IsNavigationRelevant() const
{
	return NavLinkComponent->IsNavigationRelevant();
}

FBox ADoorWithNavLink::GetComponentsBoundingBox(bool bNonColliding, bool bIncludeFromChildActors) const
{
	FBox LinksBB(FVector(0.f, 0.f, -10.f), FVector(0.f,0.f,10.f));

	if (NavLinkComponent && NavLinkComponent->IsNavigationRelevant())
	{
		LinksBB += NavLinkComponent->GetStartPoint();
		LinksBB += NavLinkComponent->GetEndPoint();
	}

	return LinksBB;
}

void ADoorWithNavLink::OnSmartLinkReached(UNavLinkCustomComponent* LinkComp, UObject* PathingAgent, const FVector& DestPoint)
{
	if (UPathFollowingComponent* PathComp = Cast<UPathFollowingComponent>(PathingAgent))
	{
		AActor* PathOwner = PathComp->GetOwner();
		if (AController* ControllerOwner = Cast<AController>(PathOwner))
		{
			PathOwner = ControllerOwner->GetPawn();
		}
		OnDoorNavLinkReached(PathOwner, DestPoint);
	}
}

void ADoorWithNavLink::OnDoorNavLinkReached_Implementation(AActor* Agent, const FVector& Destination)
{
}

void ADoorWithNavLink::ResumePathFollowing(AActor* Agent)
{
	if (Agent)
	{
		UPathFollowingComponent* PathComp = Agent->FindComponentByClass<UPathFollowingComponent>();
		if (!PathComp)
		{
			APawn* PawnOwner = Cast<APawn>(Agent);
			if (PawnOwner && PawnOwner->GetController())
			{
				PathComp = PawnOwner->GetController()->FindComponentByClass<UPathFollowingComponent>();
			}
		}

		if (PathComp)
		{
			PathComp->FinishUsingCustomLink(NavLinkComponent);
		}
	}
}

