// ©Copyright BlueFibreGames 2018. All rights reserved.

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Gameframework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	
}


void UOpenDoor::OpenDoor()
{
	// Find the owning Actor
	AActor* Owner = GetOwner();

	// Create a rotator
	FRotator NewRotation = FRotator( 0.0f, OpenAngle, 0.0f );

	// Set the door rotation
	Owner->SetActorRotation( NewRotation );
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...

	//Poll trigger volume every frame
	if (PressurePlate->IsOverlappingActor( ActorThatOpens ))
	{
		OpenDoor();
	}

}