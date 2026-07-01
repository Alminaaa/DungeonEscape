#pragma once

#include "CoreMinimal.h"
#include "DEEnemyBase.h"
#include "DERangedEnemy.generated.h"

UCLASS()
class DUNGEONESCAPE_API ADERangedEnemy : public ADEEnemyBase
{
    GENERATED_BODY()

public:
    ADERangedEnemy();

protected:
    virtual void ChaseTarget(float DeltaTime) override;
    virtual void PerformAttack() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranged Enemy")
    float PreferredDistance;
};