#include "DEEnemyBase.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

ADEEnemyBase::ADEEnemyBase()
{
    PrimaryActorTick.bCanEverTick = true;

    DetectionRange = 1200.f;
    AttackRange = 150.f;
    AttackCooldown = 1.5f;
    EnemyMoveSpeed = 250.f;

    LastAttackTime = -999.f;
    CurrentTarget = nullptr;
}

void ADEEnemyBase::BeginPlay()
{
    Super::BeginPlay();

    GetCharacterMovement()->MaxWalkSpeed = EnemyMoveSpeed;
}

void ADEEnemyBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!CurrentTarget)
    {
        FindTarget();
    }

    if (CurrentTarget)
    {
        const float Distance =
            FVector::Dist(
                GetActorLocation(),
                CurrentTarget->GetActorLocation());

        if (Distance <= DetectionRange)
        {
            ChaseTarget(DeltaTime);
            TryAttackTarget();
        }
        else
        {
            CurrentTarget = nullptr;
        }
    }
}

void ADEEnemyBase::FindTarget()
{
    APawn* PlayerPawn =
        UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if (!PlayerPawn)
    {
        return;
    }

    const float Distance =
        FVector::Dist(
            GetActorLocation(),
            PlayerPawn->GetActorLocation());

    if (Distance <= DetectionRange)
    {
        CurrentTarget = PlayerPawn;
    }
}

void ADEEnemyBase::ChaseTarget(float DeltaTime)
{
    if (!CurrentTarget)
    {
        return;
    }

    const float Distance =
        FVector::Dist(
            GetActorLocation(),
            CurrentTarget->GetActorLocation());

    if (Distance <= AttackRange)
    {
        return;
    }

    FVector Direction =
        CurrentTarget->GetActorLocation() - GetActorLocation();

    Direction.Z = 0.f;
    Direction.Normalize();

    AddMovementInput(Direction, 1.f);

    const FRotator LookRotation =
        Direction.Rotation();

    SetActorRotation(
        FRotator(
            0.f,
            LookRotation.Yaw,
            0.f));
}

void ADEEnemyBase::TryAttackTarget()
{
    if (!CurrentTarget)
    {
        return;
    }

    const float Distance =
        FVector::Dist(
            GetActorLocation(),
            CurrentTarget->GetActorLocation());

    if (Distance > AttackRange)
    {
        return;
    }

    const float CurrentTime =
        GetWorld()->GetTimeSeconds();

    if (CurrentTime - LastAttackTime < AttackCooldown)
    {
        return;
    }

    LastAttackTime = CurrentTime;
    PerformAttack();
}

void ADEEnemyBase::PerformAttack()
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
        FColor::Red,
        TEXT("Enemy Attack"));
}

void ADEEnemyBase::Die()
{
    OnEnemyDeath();
    Super::Die();
}