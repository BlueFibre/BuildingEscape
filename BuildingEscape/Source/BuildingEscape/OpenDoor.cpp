// ©Copyright BlueFibreGames 2018. All rights reserved.

#include "OpenDoor.h"
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

	// ...
	
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
	AActor* OwningDoor = GetOwner();
	FRotator DoorRotation = OwningDoor->GetActorRotation();
	
	if (DoorRotation.Yaw < 150.0f)
	{
		OwningDoor->SetActorRotation( DoorRotation.Add( 0.0f, 0.5f, 0.0f ) );
	}
}
