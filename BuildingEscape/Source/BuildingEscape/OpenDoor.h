// ©Copyright BlueFibreGames 2018. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FOnOpenRequest );

UCLASS( ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOpenDoor();

	void OpenDoor();
	void CloseDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UPROPERTY( BlueprintAssignable )
		FOnOpenRequest OnOpenRequest;


private:

	UPROPERTY( EditAnywhere )
		float OpenAngle = 90.0f;
		
		float CloseAngle;

	UPROPERTY( EditAnywhere )
		ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY( EditAnywhere )
		//Set to 0.0 for a door that doesn't close
		float DoorCloseDelay = 3.0f; 

	UPROPERTY( EditAnywhere )
		//Set to 0.0 for a door that doesn't close
		float TriggerMass = 50.f;

		float LastDoorOpenTime = 0.0f;

		AActor* Owner = nullptr; // The owning door

		float UOpenDoor::GetTotalMassOfActorsOnPlate();
};
