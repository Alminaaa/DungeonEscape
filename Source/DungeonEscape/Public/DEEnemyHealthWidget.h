#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DEEnemyHealthWidget.generated.h"

class ADEEnemyBase;

UCLASS()
class DUNGEONESCAPE_API UDEEnemyHealthWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Enemy Health")
    void InitializeEnemy(ADEEnemyBase* Enemy);

    UFUNCTION(BlueprintPure, Category = "Enemy Health")
    float GetHealthPercent() const;

    UFUNCTION(BlueprintPure, Category = "Enemy Health")
    FText GetHealthText() const;

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Enemy Health")
    ADEEnemyBase* OwningEnemy;
};