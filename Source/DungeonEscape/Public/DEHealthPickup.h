#pragma once

#include "CoreMinimal.h"
#include "DEPickupBase.h"
#include "DEHealthPickup.generated.h"

UCLASS()
class DUNGEONESCAPE_API ADEHealthPickup : public ADEPickupBase
{
    GENERATED_BODY()

public:
    ADEHealthPickup();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health Pickup")
    float HealAmount;

    virtual void ApplyPickupEffect(AActor* Picker) override;
};