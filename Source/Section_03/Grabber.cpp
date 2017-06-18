// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
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
	if (PhysicsHandle == nullptr) {
		UE_LOG (LogTemp, Error, TEXT ("%s - No PhysicsHandle found!"), *DebugName);
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if the physics handle is attached
	if (PhysicsHandle->GrabbedComponent) {
		FVector LineTraceStart; // Unused in this function
		FVector LineTraceEnd;

		GetReachLineStartAndEnd (
			OUT LineTraceStart,
			OUT LineTraceEnd
		);

		// move the object that we're holding
		PhysicsHandle->SetTargetLocation (LineTraceEnd);
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach ()
{
	FVector LineTraceStart;
	FVector LineTraceEnd;

	GetReachLineStartAndEnd (
		OUT LineTraceStart,
		OUT LineTraceEnd
	);

	/// Setup Query parameters
	FCollisionQueryParams TraceParameters = FCollisionQueryParams (FName (TEXT ("")), false, GetOwner ());

	/// Ray-cast out to reach distance
	FHitResult LineTraceHit;

	GetWorld ()->LineTraceSingleByObjectType (
		OUT LineTraceHit,
		LineTraceStart,
		LineTraceEnd,
		FCollisionObjectQueryParams (ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	return LineTraceHit;
}

void UGrabber::GetReachLineStartAndEnd (FVector &out_LineTraceStart, FVector &out_LineTraceEnd)
{
	FRotator PlayerViewPointRotation;

	// Need this to determine the line trace vector
	GetWorld ()->GetFirstPlayerController ()->GetPlayerViewPoint (
		OUT out_LineTraceStart,
		OUT PlayerViewPointRotation
	);

	out_LineTraceEnd = out_LineTraceStart + PlayerViewPointRotation.Vector () * Reach;
}

void UGrabber::Grab ()
{
	/// Line Trace and try and reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach ();
	auto ComponentToGrab = HitResult.GetComponent ();
	auto ActorHit = HitResult.GetActor ();

	/// If we hit something the attache a physics handle
	if (ActorHit) {
		// Attach physics handle
		PhysicsHandle->GrabComponentAtLocationWithRotation (
			ComponentToGrab,
			NAME_None, // No Bones needed
			ComponentToGrab->GetOwner ()->GetActorLocation (),
			FRotator(0.0f) // No rotation
		);
	}
}

void UGrabber::Release ()
{
	PhysicsHandle->ReleaseComponent ();
}
