#pragma once

#include "CoreMinimal.h"
#include "DEPickupBase.h"
#include "DESpeedPickup.generated.h"

UCLASS()
class DUNGEONESCAPE_API ADESpeedPickup : public ADEPickupBase
{
    GENERATED_BODY()

public:
    ADESpeedPickup();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speed Pickup")
    float SpeedBoostAmount;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speed Pickup")
    float BoostDuration;

    virtual void ApplyPickupEffect(AActor* Picker) override;
};