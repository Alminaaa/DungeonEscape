#pragma once

#include "CoreMinimal.h"
#include "DEMeleeEnemy.h"
#include "DEBossEnemy.generated.h"

class ADEDoor;

UCLASS()
class DUNGEONESCAPE_API ADEBossEnemy : public ADEMeleeEnemy
{
    GENERATED_BODY()

public:
    ADEBossEnemy();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss")
    float BossScale;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss")
    ADEDoor* DoorToOpenOnDeath;

    UFUNCTION(BlueprintImplementableEvent, Category = "Boss")
    void OnBossSpawned();

    UFUNCTION(BlueprintImplementableEvent, Category = "Boss")
    void OnBossDefeated();

public:
    virtual void Die() override;
};