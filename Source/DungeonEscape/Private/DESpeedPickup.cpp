#include "DESpeedPickup.h"

#include "DEPlayerCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

ADESpeedPickup::ADESpeedPickup()
{
    SpeedBoostAmount = 300.f;
    BoostDuration = 5.f;
}

void ADESpeedPickup::ApplyPickupEffect(AActor* Picker)
{
    ADEPlayerCharacter* Player =
        Cast<ADEPlayerCharacter>(Picker);

    if (Player)
    {
        Player->AddSpeedBoost(
            SpeedBoostAmount,
            BoostDuration);

        return;
    }

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

    Movement->MaxWalkSpeed += SpeedBoostAmount;
}