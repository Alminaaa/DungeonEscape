#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DEVictoryZone.generated.h"

class UBoxComponent;

UCLASS()
class DUNGEONESCAPE_API ADEVictoryZone : public AActor
{
    GENERATED_BODY()

public:
    ADEVictoryZone();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* Root;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* VictoryBox;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Victory")
    int32 RequiredPlayersInZone;

    UPROPERTY(BlueprintReadOnly, Category = "Victory")
    TArray<AActor*> PlayersInZone;

    UFUNCTION()
    void OnVictoryBoxBeginOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

    UFUNCTION()
    void OnVictoryBoxEndOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex);

    void CheckVictoryCondition();

    UFUNCTION(BlueprintImplementableEvent, Category = "Victory")
    void OnVictoryTriggered();
};