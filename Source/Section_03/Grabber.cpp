// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Do nothing just indicate that this parameter is used as an output
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FString ObjectName = GetOwner ()->GetName ();
	UE_LOG (LogTemp, Warning, TEXT ("%s - Grabber is reporting for duty!"), *ObjectName);

	/// Look for attached Physics Handle
	PhysicsHandle = GetOwner ()->FindComponentByClass<UPhysicsHandleComponent> ();
	if (!PhysicsHandle) {
		UE_LOG (LogTemp, Error, TEXT ("%s - No PhysicsHandle found!"), *ObjectName);
	}
	/// Look for attached Input Component
	InputComponent = GetOwner ()->FindComponentByClass<UInputComponent> ();
	if (InputComponent) {
		UE_LOG (LogTemp, Warning, TEXT ("%s - InputComponent found!"), *ObjectName);
		/// Bind input axis
		InputComponent->BindAction ("Grab", EInputEvent::IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction ("Grab", EInputEvent::IE_Released, this, &UGrabber::Release);
	} else {
		UE_LOG (LogTemp, Error, TEXT ("%s - No InputComponent found!"), *ObjectName);
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get player viewpoint this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld ()->GetFirstPlayerController ()->GetPlayerViewPoint (
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	
	FString ObjectName = GetOwner ()->GetName ();
	//UE_LOG (LogTemp, Warning, TEXT ("%s - Location: %s . Rotation: %s"), 
	//	*ObjectName,
	//	*(PlayerViewPointLocation.ToString ()),
	//	*(PlayerViewPointRotation.ToString ())
	//);

	/// Draw red trace in the world to visualize
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	DrawDebugLine (
		GetWorld (),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor (255, 0, 0),
		false,
		0.0f,
		0,
		10.0f
		);
	
	/// Setup Query parameters
	FCollisionQueryParams TraceParameters = FCollisionQueryParams (FName (TEXT ("")), false, GetOwner ());

	/// Ray-cast out to reach distance
	FHitResult LineTraceHit;

	GetWorld ()->LineTraceSingleByObjectType (
		OUT LineTraceHit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	/// See what we hit
	AActor* ActorHit = LineTraceHit.GetActor ();
	if (ActorHit) {
		FString HitResult = ActorHit->GetName ();
		UE_LOG (LogTemp, Warning, TEXT ("%s - line trace is hitting: %s"), *ObjectName, *HitResult);
	}
}

void UGrabber::Grab ()
{
	FString ObjectName = GetOwner ()->GetName ();
	UE_LOG (LogTemp, Warning, TEXT ("%s - Grab key pressed."), *ObjectName);
}

void UGrabber::Release ()
{
	FString ObjectName = GetOwner ()->GetName ();
	UE_LOG (LogTemp, Warning, TEXT ("%s - Grab key released."), *ObjectName);
}

