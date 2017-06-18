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
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	DebugName = GetOwner ()->GetName ();

	FindPhysicsHandleComponent ();

	SetupInputComponent ();
}

void UGrabber::SetupInputComponent ()
{
	/// Look for attached Input Component
	InputComponent = GetOwner ()->FindComponentByClass<UInputComponent> ();
	if (InputComponent) {
		UE_LOG (LogTemp, Warning, TEXT ("%s - InputComponent found!"), *DebugName);
		/// Bind input axis
		InputComponent->BindAction ("Grab", EInputEvent::IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction ("Grab", EInputEvent::IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG (LogTemp, Error, TEXT ("%s - No InputComponent found!"), *DebugName);
	}
}

void UGrabber::FindPhysicsHandleComponent ()
{
	/// Look for attached Physics Handle
	PhysicsHandle = GetOwner ()->FindComponentByClass<UPhysicsHandleComponent> ();
	if (!PhysicsHandle) {
		UE_LOG (LogTemp, Error, TEXT ("%s - No PhysicsHandle found!"), *DebugName);
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if the pysics handle is attached
		// move the object that we're holding

}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach ()
{
	/// Get player viewpoint this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	// Need this to determine the line trace vector
	GetWorld ()->GetFirstPlayerController ()->GetPlayerViewPoint (
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	//UE_LOG (LogTemp, Warning, TEXT ("%s - Location: %s . Rotation: %s"), 
	//	*DebugName,
	//	*(PlayerViewPointLocation.ToString ()),
	//	*(PlayerViewPointRotation.ToString ())
	//);

	/// Draw red trace in the world to visualize
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector () * Reach;
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
		FCollisionObjectQueryParams (ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	/// Debug only - See what we hit
	AActor* ActorHit = LineTraceHit.GetActor ();
	if (ActorHit) {
		FString HitResult = ActorHit->GetName ();
		UE_LOG (LogTemp, Warning, TEXT ("%s - line trace is hitting: %s"), *DebugName, *HitResult);
	}

	return LineTraceHit;
}

void UGrabber::Grab ()
{
	UE_LOG (LogTemp, Warning, TEXT ("%s - Grab key pressed."), *DebugName);

	/// Line Trace and try and reach any actors with physics body collision channel set
	GetFirstPhysicsBodyInReach ();

	/// If we hit something the attache a physics handle
	// TODO Attach physics handle
}

void UGrabber::Release ()
{
	UE_LOG (LogTemp, Warning, TEXT ("%s - Grab key released."), *DebugName);

	// TODO Release physics handle
}

