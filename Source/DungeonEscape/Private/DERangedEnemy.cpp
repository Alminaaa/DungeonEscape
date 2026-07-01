#include "DERangedEnemy.h"

#include "Kismet/GameplayStatics.h"

ADERangedEnemy::ADERangedEnemy()
{
    MaxHealth = 60.f;
    CurrentHealth = MaxHealth;
    BaseDamage = 15.f;

    DetectionRange = 1400.f;
    AttackRange = 700.f;
    AttackCooldown = 2.f;
    EnemyMoveSpeed = 220.f;

    PreferredDistance = 500.f;
}

void ADERangedEnemy::ChaseTarget(float DeltaTime)
{
    if (!CurrentTarget)
    {
        return;
    }

    FVector Direction =
        CurrentTarget->GetActorLocation() - GetActorLocation();

    Direction.Z = 0.f;

    const float Distance =
        Direction.Size();

    Direction.Normalize();

    const FRotator LookRotation =
        Direction.Rotation();

    SetActorRotation(
        FRotator(
            0.f,
            LookRotation.Yaw,
            0.f));

    if (Distance > PreferredDistance)
    {
        AddMovementInput(Direction, 1.f);
    }
    else if (Distance < PreferredDistance * 0.7f)
    {
        AddMovementInput(-Direction, 1.f);
    }
}

void ADERangedEnemy::PerformAttack()
{
    if (!CurrentTarget)
    {
        return;
    }

    UGameplayStatics::ApplyDamage(
        CurrentTarget,
        BaseDamage,
        GetController(),
        this,
        nullptr);

    OnEnemyAttack();

    GEngine->AddOnScreenDebugMessage(
        -1,
        1.f,
        FColor::Purple,
        TEXT("Ranged Enemy Attack"));
}