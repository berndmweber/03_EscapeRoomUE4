// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"

// Do nothing just indicate that this parameter is used as an output
#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner ();

	if (PressurePlate == nullptr) {
		UE_LOG (LogTemp, Error, TEXT ("%s - No PressurePlate defined!"), *(Owner->GetName ()));
	}
}

void UOpenDoor::OpenDoor ()
{
	if (Owner) {
		Owner->SetActorRotation (FRotator (0.0f, OpenAngle, 00.0f));
	}
}

void UOpenDoor::CloseDoor ()
{
	if (Owner) {
		Owner->SetActorRotation (FRotator (0.0f, 0.0f, 00.0f));
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the trigger volume
	if (GetTotalMassOfActorsOnPlate() > 25.0f) {  // TODO Make into a parameter
		// If the actorthatopens is in the volume
		OpenDoor ();
		LastDoorOpenTime = GetWorld ()->GetTimeSeconds ();
	}

	// Check if it's time to close the door
	if ((GetWorld ()->GetTimeSeconds () - LastDoorOpenTime) > DoorCloseDelay) {
		CloseDoor ();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate ()
{
	float TotalMass = 0.0f;

	// Find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	if (PressurePlate) {
		PressurePlate->GetOverlappingActors (OUT OverlappingActors);
	}

	// Iterate through them adding their masses
	for (const auto* OverlappingActor : OverlappingActors) {
		float ActorMass = OverlappingActor->FindComponentByClass<UPrimitiveComponent> ()->GetMass ();
		TotalMass += ActorMass;
	}

	return TotalMass;
}
