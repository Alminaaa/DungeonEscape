#include "DEEnemyBase.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "DEEnemyHealthWidget.h"

ADEEnemyBase::ADEEnemyBase()
{
    PrimaryActorTick.bCanEverTick = true;

    bReplicates = true;
    SetReplicateMovement(true);

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    HealthBarWidget =
        CreateDefaultSubobject<UWidgetComponent>(
            TEXT("HealthBarWidget"));
    HealthBarWidget->SetupAttachment(RootComponent);
    HealthBarWidget->SetRelativeLocation(
        FVector(0.f, 0.f, 170.f));
    HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
    HealthBarWidget->SetDrawSize(FVector2D(120.f, 14.f));
    HealthBarWidget->SetVisibility(false);

    HealthBarVisibleDuration = 3.f;

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

    HealthBarWidget->SetVisibility(false);

    if (HealthBarWidgetClass)
    {
        HealthBarWidget->SetWidgetClass(
            HealthBarWidgetClass);

        HealthBarWidget->InitWidget();

        if (UDEEnemyHealthWidget* HealthWidget =
            Cast<UDEEnemyHealthWidget>(
                HealthBarWidget->GetUserWidgetObject()))
        {
            HealthWidget->InitializeEnemy(this);
        }
    }
}

void ADEEnemyBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!HasAuthority())
    {
        return;
    }

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
        UGameplayStatics::GetPlayerPawn(
            GetWorld(),
            0);

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
        CurrentTarget->GetActorLocation() -
        GetActorLocation();

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
}

float ADEEnemyBase::TakeDamage(
    float DamageAmount,
    FDamageEvent const& DamageEvent,
    AController* EventInstigator,
    AActor* DamageCauser)
{
    const float DamageApplied =
        Super::TakeDamage(
            DamageAmount,
            DamageEvent,
            EventInstigator,
            DamageCauser);

    if (DamageApplied > 0.f)
    {
        ShowHealthBar();
    }

    return DamageApplied;
}

void ADEEnemyBase::ShowHealthBar()
{
    if (!HealthBarWidget)
    {
        return;
    }

    HealthBarWidget->SetVisibility(true);

    GetWorldTimerManager().ClearTimer(
        HealthBarTimerHandle);

    GetWorldTimerManager().SetTimer(
        HealthBarTimerHandle,
        this,
        &ADEEnemyBase::HideHealthBar,
        HealthBarVisibleDuration,
        false);
}

void ADEEnemyBase::HideHealthBar()
{
    if (HealthBarWidget)
    {
        HealthBarWidget->SetVisibility(false);
    }
}

void ADEEnemyBase::Die()
{
    OnEnemyDeath();
    Super::Die();
}