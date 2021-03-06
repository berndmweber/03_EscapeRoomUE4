// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SECTION_03_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY (EditAnywhere)
	float Reach = 100.0f;
	
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	// Used for debug output
	FString DebugName;

	// Get the Trace Line start and end vectors
	void GetReachLineStartAndEnd (FVector&, FVector&);

	// Get the first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach ();

	// Find attached physics handle
	void FindPhysicsHandleComponent ();

	// Setup attached input component
	void SetupInputComponent ();

	// Ray-case and grab what's in reach
	void Grab ();

	// Release what has been grabbed previously
	void Release ();
};
