// ©Copyright BlueFibreGames 2018. All rights reserved.

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "DrawDebugHelpers.h"

#define OUT // Does nothing, used to annotate out variables in void functions

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

	FindPhysicsHandleComponent();
	SetupInputComponent();	
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!PhysicsHandle) { return; } // Protects against calling a nullptr
	/// If the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		/// move the object we're holding
		PhysicsHandle->SetTargetLocation( GetReachLineEnd() );
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FHitResult Hit;
	FCollisionQueryParams TraceParameters( FName( TEXT( "" ) ), false, GetOwner() );
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams( ECollisionChannel::ECC_PhysicsBody ),
		TraceParameters
	);

	/// See what we hit
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG( LogTemp, Warning, TEXT( "Actor Hit: %s" ), *(ActorHit->GetName()) );
	}

	return Hit;
}

void UGrabber::FindPhysicsHandleComponent()
{
	UGrabber::PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG( LogTemp, Error, TEXT( "No Physics handle component found on %s" ), *GetOwner()->GetName() );
	}
}

void UGrabber::SetupInputComponent()
{
	/// Look for attached input componet
	InputComponet = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponet)
	{
		//Bind the input
		InputComponet->BindAction( "Interact", IE_Pressed, this, &UGrabber::Grab );
		InputComponet->BindAction( "Interact", IE_Released, this, &UGrabber::Release );
	}
	else
	{
		UE_LOG( LogTemp, Error, TEXT( "No input component found on %s" ), *GetOwner()->GetName() );
	}
}

void UGrabber::Grab()
{
	/// Try and reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent(); // Gets the mesh
	auto ActorHit = HitResult.GetActor();

	/// If we hit something then attach a physics handle
	if (ActorHit)
	{
		if (!PhysicsHandle) { return; } // Protects against calling a nullptr
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None, // No bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // Constrain Rotation
		);
	}
}

void UGrabber::Release()
{
	if (!PhysicsHandle) { return; } // Protects against calling a nullptr
	PhysicsHandle->ReleaseComponent();
}

FVector UGrabber::GetReachLineStart() const
{
	/// Get the player viewpoint this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint( OUT PlayerViewPointLocation, OUT PlayerViewPointRotation );

	return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd() const
{
	/// Get the player viewpoint this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint( OUT PlayerViewPointLocation, OUT PlayerViewPointRotation );

	/// Calculate reach vector
	return PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
}

