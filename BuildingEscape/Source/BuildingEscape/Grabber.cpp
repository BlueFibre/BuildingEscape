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

void UGrabber::Grab()
{
	//TODO grab function
	UE_LOG( LogTemp, Log, TEXT( "Interact key pressed" ) );

	/// Try and reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

		/// If we hit something then attach a physics handle
	if (ActorHit)
	{
		//Attach physics handle
		PhysicsHandle->GrabComponent( 
		ComponentToGrab,
		NAME_None,
		ComponentToGrab->GetOwner()->GetActorLocation(),
		true
		);
	}
}

void UGrabber::Release()
{
	UE_LOG( LogTemp, Log, TEXT( "Interact key released" ) );
	// TODO Release physics handle
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::FindPhysicsHandleComponent()
{
	UGrabber::PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		//Physics handle is found
	}
	else
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
		//Input Component found
		UE_LOG( LogTemp, Log, TEXT( "%s - Input component sucessfully found." ), *GetOwner()->GetName() );

		//Bind the input
		InputComponet->BindAction( "Interact", IE_Pressed, this, &UGrabber::Grab );
		InputComponet->BindAction( "Interact", IE_Released, this, &UGrabber::Release );
	}
	else
	{
		UE_LOG( LogTemp, Error, TEXT( "No input component found on %s" ), *GetOwner()->GetName() );
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	/// Get the player viewpoint this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint( OUT PlayerViewPointLocation, OUT PlayerViewPointRotation );

	/// Calculate reach vector
	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);

	/// Setup query paramameters
	FCollisionQueryParams TraceParameters( FName( TEXT( "" ) ), false, GetOwner() );

	/// Line-trace (ray-cast) out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
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

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/// Get the player viewpoint this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint( OUT PlayerViewPointLocation, OUT PlayerViewPointRotation );

	/// Calculate reach vector
	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);

	/// If the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		/// move the object we're holding
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

