#include "DEHealthPickup.h"

#include "DECharacterBase.h"

ADEHealthPickup::ADEHealthPickup()
{
    HealAmount = 30.f;
}

void ADEHealthPickup::ApplyPickupEffect(AActor* Picker)
{
    ADECharacterBase* Character =
        Cast<ADECharacterBase>(Picker);

    if (!Character)
    {
        return;
    }

    Character->Heal(HealAmount);

    GEngine->AddOnScreenDebugMessage(
        -1,
        1.5f,
        FColor::Green,
        TEXT("Health Pickup"));
}