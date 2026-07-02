#pragma once

#include "CoreMinimal.h"
#include "DECharacterBase.h"
#include "DEEnemyBase.generated.h"

class UWidgetComponent;

UCLASS()
class DUNGEONESCAPE_API ADEEnemyBase : public ADECharacterBase
{
    GENERATED_BODY()

public:
    ADEEnemyBase();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UWidgetComponent* HealthBarWidget;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy UI")
    TSubclassOf<UUserWidget> HealthBarWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy UI")
    float HealthBarVisibleDuration;

    FTimerHandle HealthBarTimerHandle;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
    float DetectionRange;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
    float AttackRange;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
    float AttackCooldown;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
    float EnemyMoveSpeed;

    UPROPERTY(BlueprintReadOnly, Category = "Enemy")
    AActor* CurrentTarget;

    float LastAttackTime;

    virtual void FindTarget();
    virtual void ChaseTarget(float DeltaTime);
    virtual void TryAttackTarget();
    virtual void PerformAttack();

    void ShowHealthBar();
    void HideHealthBar();

    UFUNCTION(BlueprintImplementableEvent, Category = "Enemy")
    void OnEnemyAttack();

    UFUNCTION(BlueprintImplementableEvent, Category = "Enemy")
    void OnEnemyDeath();

public:
    virtual float TakeDamage(
        float DamageAmount,
        struct FDamageEvent const& DamageEvent,
        class AController* EventInstigator,
        AActor* DamageCauser) override;

    virtual void Die() override;
};