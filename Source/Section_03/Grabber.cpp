// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

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
	UE_LOG (LogTemp, Warning, TEXT ("%s - Location: %s . Rotation: %s"), 
		*ObjectName,
		*(PlayerViewPointLocation.ToString ()),
		*(PlayerViewPointRotation.ToString ())
	);

	// Ray-cast out to reach distance

	// See what we hit

}

