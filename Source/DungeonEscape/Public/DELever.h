#pragma once

#include "CoreMinimal.h"
#include "DEInteractableBase.h"
#include "DELever.generated.h"

UCLASS()
class DUNGEONESCAPE_API ADELever : public ADEInteractableBase
{
    GENERATED_BODY()

public:
    ADELever();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Activated, Category = "Lever")
    bool bActivated;

    UFUNCTION()
    void OnRep_Activated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Lever")
    void OnLeverActivated();

public:
    virtual void Interact_Implementation(AActor* Interactor) override;

    UFUNCTION(BlueprintPure)
    bool IsActivated() const;

    virtual void GetLifetimeReplicatedProps(
        TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};