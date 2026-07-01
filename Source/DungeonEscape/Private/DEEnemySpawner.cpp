#include "DEEnemySpawner.h"

#include "Components/BoxComponent.h"
#include "DEEnemyBase.h"
#include "DEPlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

ADEEnemySpawner::ADEEnemySpawner()
{
    PrimaryActorTick.bCanEverTick = false;

    bReplicates = true;
    SetReplicateMovement(true);

    Root = CreateDefaultSubobject<USceneComponent>(
        TEXT("Root"));
    RootComponent = Root;

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(
        TEXT("TriggerBox"));
    TriggerBox->SetupAttachment(Root);
    TriggerBox->SetBoxExtent(FVector(300.f, 300.f, 150.f));
    TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    SpawnCount = 3;
    SpawnRadius = 400.f;
    bSpawnOnlyOnce = true;
    bHasSpawned = false;
}

void ADEEnemySpawner::BeginPlay()
{
    Super::BeginPlay();

    TriggerBox->OnComponentBeginOverlap.AddDynamic(
        this,
        &ADEEnemySpawner::OnTriggerBeginOverlap);
}

void ADEEnemySpawner::OnTriggerBeginOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (!HasAuthority())
    {
        return;
    }

    if (!OtherActor || !OtherActor->IsA<ADEPlayerCharacter>())
    {
        return;
    }

    if (bSpawnOnlyOnce && bHasSpawned)
    {
        return;
    }

    bHasSpawned = true;

    SpawnEnemies();
    OnSpawnerTriggered();
}

void ADEEnemySpawner::SpawnEnemies()
{
    if (EnemyClasses.Num() <= 0)
    {
        return;
    }

    for (int32 i = 0; i < SpawnCount; i++)
    {
        const int32 ClassIndex =
            FMath::RandRange(
                0,
                EnemyClasses.Num() - 1);

        TSubclassOf<ADEEnemyBase> EnemyClass =
            EnemyClasses[ClassIndex];

        if (!EnemyClass)
        {
            continue;
        }

        const FVector RandomOffset =
            UKismetMathLibrary::RandomUnitVector() *
            SpawnRadius;

        FVector SpawnLocation =
            GetActorLocation() +
            FVector(
                RandomOffset.X,
                RandomOffset.Y,
                50.f);

        FRotator SpawnRotation =
            FRotator::ZeroRotator;

        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride =
            ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        GetWorld()->SpawnActor<ADEEnemyBase>(
            EnemyClass,
            SpawnLocation,
            SpawnRotation,
            SpawnParams);
    }
}