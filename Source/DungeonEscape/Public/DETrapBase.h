#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DETrapBase.generated.h"

class UBoxComponent;

UCLASS()
class DUNGEONESCAPE_API ADETrapBase : public AActor
{
    GENERATED_BODY()

public:
    ADETrapBase();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* Root;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* DamageBox;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trap")
    float DamageAmount;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trap")
    float DamageCooldown;

    UPROPERTY()
    TMap<AActor*, float> LastDamageTimes;

    UFUNCTION()
    virtual void OnDamageBoxOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

    UFUNCTION(BlueprintImplementableEvent, Category = "Trap")
    void OnTrapTriggered(AActor* DamagedActor);
};