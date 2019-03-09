// Copyright Preston Schwartz 2019

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay() {
	Super::BeginPlay();
	Owner = GetOwner();
	if(!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("UOpenDoor::BeginPlay() Cannot find PressurePlate"));
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume
	if(GetTotalMassOfActorsOnPlate() > TriggerMass) { // TODO make into a parameter
		OnOpenRequest.Broadcast();
	}

	else {
		OnCloseRequest.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() {
	float TotalMass = 0.0f;

	// Find all the overlapping actors
	TArray<AActor*> OverlappingActors;

	if(!PressurePlate) {
		return TotalMass;
	}

	PressurePlate -> GetOverlappingActors(OUT OverlappingActors);

	// Iterate through them adding their masses
	for(const auto& Actor : OverlappingActors) {
		TotalMass += Actor -> FindComponentByClass<UPrimitiveComponent>() -> GetMass();
		//UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor -> GetName())
	}

	return TotalMass;
}

