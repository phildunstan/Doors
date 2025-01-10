// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DoorInterface.h"
#include "NavLinkCustomComponent.h"
#include "NavLinkHostInterface.h"
#include "GameFramework/Actor.h"
#include "DoorWithNavLink.generated.h"

class UDoorNavLinkComponent;
class UNavLinkRenderingComponent;
class UNavModifierComponent;
class USceneComponent;

UCLASS(Blueprintable, autoCollapseCategories=(SmartLink, Actor), hideCategories=(Input), MinimalAPI)
class ADoorWithNavLink : public AActor, public INavLinkHostInterface, public INavRelevantInterface, public IDoorInterface
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(VisibleAnywhere, Category=Components)
	TObjectPtr<USceneComponent> SceneComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Navigation)
	TObjectPtr<UDoorNavLinkComponent> NavLinkComponent;

#if WITH_EDITORONLY_DATA
private:
	/** Editor Preview */
	UPROPERTY()
	TObjectPtr<UNavLinkRenderingComponent> NavLinkRenderingComponent;
public:
#endif // WITH_EDITORONLY_DATA

	virtual void PostRegisterAllComponents() override;
	
#if ENABLE_VISUAL_LOG
	// used to set up redirection of log owner redirection to the visual logger
	virtual void BeginPlay() override;
#endif // ENABLE_VISUAL_LOG

	// BEGIN INavLinkHostInterface - used for rendering the navlink in the editor
	virtual bool GetNavigationLinksClasses(TArray<TSubclassOf<UNavLinkDefinition> >& OutClasses) const override;
	virtual bool GetNavigationLinksArray(TArray<FNavigationLink>& OutLink, TArray<FNavigationSegmentLink>& OutSegments) const override;
	// END INavLinkHostInterface

	// BEGIN INavRelevantInterface
	virtual FBox GetNavigationBounds() const override;
	virtual bool IsNavigationRelevant() const override;
	// END INavLinkHostInterface
	
	virtual FBox GetComponentsBoundingBox(bool bNonColliding = false, bool bIncludeFromChildActors = false) const override;

	UFUNCTION()
	void OnSmartLinkReached(UNavLinkCustomComponent* LinkComp, UObject* PathingAgent, const FVector& DestPoint);

	UFUNCTION(BlueprintNativeEvent)
	void OnDoorNavLinkReached(AActor* Agent, const FVector& Destination);
	
	UFUNCTION(BlueprintCallable)
	void ResumePathFollowing(AActor *Agent);
};
