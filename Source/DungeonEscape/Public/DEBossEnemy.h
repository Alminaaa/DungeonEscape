#pragma once

#include "CoreMinimal.h"
#include "DEMeleeEnemy.h"
#include "DEBossEnemy.generated.h"

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

    UFUNCTION(BlueprintImplementableEvent, Category = "Boss")
    void OnBossSpawned();
};