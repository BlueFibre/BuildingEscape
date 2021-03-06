// ©Copyright BlueFibreGames 2018. All rights reserved.

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Gameframework/Actor.h"

#define OUT

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

	// Find the owning Actor
	Owner = GetOwner();
	CloseAngle = GetOwner()->GetActorRotation().Yaw;
	if (!PressurePlate) 
	{
		UE_LOG( LogTemp, Error, TEXT( "%s: NO PRESSURE PLATE ASSIGNED." ), *GetOwner()->GetName() ) return;
	} 
}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	//Poll trigger volume every frame
	if (GetTotalMassOfActorsOnPlate() == TriggerMass) //TODO Create TriggerMass float
	{
		OnOpen.Broadcast();
	}
	else
	{
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	//Find all the overlapping actors
	TArray<AActor*> OverLappingActors;
	if (!PressurePlate) { return 999.0f; } // protects against nullptr
	PressurePlate->GetOverlappingActors( OUT OverLappingActors );

	//Iterate through and add their masses
	for (const auto* Actor : OverLappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}