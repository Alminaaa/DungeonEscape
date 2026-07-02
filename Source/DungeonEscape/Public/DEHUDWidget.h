#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DEHUDWidget.generated.h"

UCLASS()
class DUNGEONESCAPE_API UDEHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, Category = "HUD")
    float GetHealthPercent() const;

    UFUNCTION(BlueprintPure, Category = "HUD")
    FText GetHealthText() const;

    UFUNCTION(BlueprintPure, Category = "HUD")
    FText GetSpeedBoostText() const;

    UFUNCTION(BlueprintPure, Category = "HUD")
    bool IsSpeedBoostActive() const;
};