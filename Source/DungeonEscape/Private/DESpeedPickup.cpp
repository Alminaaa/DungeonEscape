#include "DESpeedPickup.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

ADESpeedPickup::ADESpeedPickup()
{
    SpeedBoostAmount = 300.f;
    BoostDuration = 5.f;
}

void ADESpeedPickup::ApplyPickupEffect(AActor* Picker)
{
    ACharacter* Character =
        Cast<ACharacter>(Picker);

    if (!Character)
    {
        return;
    }

    UCharacterMovementComponent* Movement =
        Character->GetCharacterMovement();

    if (!Movement)
    {
        return;
    }

    const float OriginalSpeed =
        Movement->MaxWalkSpeed;

    Movement->MaxWalkSpeed =
        OriginalSpeed + SpeedBoostAmount;

    FTimerHandle ResetSpeedTimerHandle;

    FTimerDelegate ResetDelegate;
    ResetDelegate.BindLambda(
        [Movement, OriginalSpeed]()
        {
            if (Movement)
            {
                Movement->MaxWalkSpeed = OriginalSpeed;
            }
        });

    GetWorld()->GetTimerManager().SetTimer(
        ResetSpeedTimerHandle,
        ResetDelegate,
        BoostDuration,
        false);

    GEngine->AddOnScreenDebugMessage(
        -1,
        1.5f,
        FColor::Blue,
        TEXT("Speed Boost Activated"));
}