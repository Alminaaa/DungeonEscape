#pragma once

#include "CoreMinimal.h"
#include "DEInteractableBase.h"
#include "DEDoor.generated.h"

UCLASS()
class DUNGEONESCAPE_API ADEDoor : public ADEInteractableBase
{
    GENERATED_BODY()

public:
    ADEDoor();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_DoorOpen, Category = "Door")
    bool bIsOpen;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
    bool bOpenWhenLeversActivated;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
    float OpenHeight;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
    float OpenSpeed;

    FVector ClosedLocation;
    FVector OpenLocation;

    UFUNCTION()
    void OnRep_DoorOpen();

    UFUNCTION(BlueprintImplementableEvent, Category = "Door")
    void OnDoorOpened();

public:
    UFUNCTION(BlueprintCallable)
    void OpenDoor();

    UFUNCTION(BlueprintPure)
    bool IsOpen() const;

    virtual void GetLifetimeReplicatedProps(
        TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};