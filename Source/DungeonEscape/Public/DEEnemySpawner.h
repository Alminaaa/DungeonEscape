#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DEEnemySpawner.generated.h"

class UBoxComponent;
class ADEEnemyBase;

UCLASS()
class DUNGEONESCAPE_API ADEEnemySpawner : public AActor
{
    GENERATED_BODY()

public:
    ADEEnemySpawner();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* Root;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* TriggerBox;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
    TArray<TSubclassOf<ADEEnemyBase>> EnemyClasses;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
    int32 SpawnCount;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
    float SpawnRadius;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
    bool bSpawnOnlyOnce;

    UPROPERTY(BlueprintReadOnly, Category = "Spawner")
    bool bHasSpawned;

    UFUNCTION()
    void OnTriggerBeginOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

    void SpawnEnemies();

    UFUNCTION(BlueprintImplementableEvent, Category = "Spawner")
    void OnSpawnerTriggered();
};