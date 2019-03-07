// Copyright Preston Schwartz 2019

#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

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

	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));

	/// Look for attached physics handle
	PhysicsHandle = GetOwner() -> FindComponentByClass<UPhysicsHandleComponent>();
	if(PhysicsHandle) {
		
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Grabber BeginPlay() error: Cannot find PhysicsHandle from owner: %s"), *(GetOwner() -> GetName()));
	}

	/// Look for attached input component
	InputComponent = GetOwner() -> FindComponentByClass<UInputComponent>();
	if(InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("InputComponent found"));
		/// Bind input action
		InputComponent -> BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Grabber BeginPlay() error: Cannot find InputComponent from owner: %s"), *(GetOwner() -> GetName()));
	}
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed!"));
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld() -> GetFirstPlayerController() -> GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.0f,
		0.0f,
		10.0f
	);

	/// Setup query params
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	/// Line-tracce out to reach distance 
	FHitResult Hit;
	GetWorld() -> LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

		/// See what we hit
	AActor* ActorHit = Hit.GetActor();
	if(ActorHit) {
		// UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit -> GetName()));
	}
}

