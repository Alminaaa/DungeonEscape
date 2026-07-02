#include "DEBossEnemy.h"

#include "DEDoor.h"

ADEBossEnemy::ADEBossEnemy()
{
    MaxHealth = 350.f;
    CurrentHealth = MaxHealth;
    BaseDamage = 40.f;

    DetectionRange = 1800.f;
    AttackRange = 200.f;
    AttackCooldown = 1.5f;
    EnemyMoveSpeed = 220.f;

    BossScale = 1.7f;
    DoorToOpenOnDeath = nullptr;
}

void ADEBossEnemy::BeginPlay()
{
    Super::BeginPlay();

    SetActorScale3D(
        FVector(
            BossScale,
            BossScale,
            BossScale));

    OnBossSpawned();
}

void ADEBossEnemy::Die()
{
    if (HasAuthority() && DoorToOpenOnDeath)
    {
        DoorToOpenOnDeath->OpenDoor();
    }

    OnBossDefeated();

    Super::Die();
}