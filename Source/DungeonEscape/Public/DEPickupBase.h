#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DEPickupBase.generated.h"

class USphereComponent;

UCLASS()
class DUNGEONESCAPE_API ADEPickupBase : public AActor
{
    GENERATED_BODY()

public:
    ADEPickupBase();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* Root;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* PickupSphere;

    UFUNCTION()
    virtual void OnPickupOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

    virtual void ApplyPickupEffect(AActor* Picker);

    UFUNCTION(BlueprintImplementableEvent, Category = "Pickup")
    void OnPickupCollected(AActor* Picker);
};