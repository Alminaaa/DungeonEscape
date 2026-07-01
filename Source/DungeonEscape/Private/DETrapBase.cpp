#include "DETrapBase.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

ADETrapBase::ADETrapBase()
{
    PrimaryActorTick.bCanEverTick = false;

    bReplicates = true;
    SetReplicateMovement(true);

    Root = CreateDefaultSubobject<USceneComponent>(
        TEXT("Root"));
    RootComponent = Root;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(
        TEXT("Mesh"));
    Mesh->SetupAttachment(Root);

    DamageBox = CreateDefaultSubobject<UBoxComponent>(
        TEXT("DamageBox"));
    DamageBox->SetupAttachment(Root);
    DamageBox->SetBoxExtent(FVector(100.f, 100.f, 50.f));
    DamageBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    DamageBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    DamageBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    DamageAmount = 20.f;
    DamageCooldown = 1.f;
}

void ADETrapBase::BeginPlay()
{
    Super::BeginPlay();

    DamageBox->OnComponentBeginOverlap.AddDynamic(
        this,
        &ADETrapBase::OnDamageBoxOverlap);
}

void ADETrapBase::OnDamageBoxOverlap(
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

    if (!OtherActor || OtherActor == this)
    {
        return;
    }

    const float CurrentTime =
        GetWorld()->GetTimeSeconds();

    if (LastDamageTimes.Contains(OtherActor))
    {
        const float LastTime =
            LastDamageTimes[OtherActor];

        if (CurrentTime - LastTime < DamageCooldown)
        {
            return;
        }
    }

    LastDamageTimes.Add(OtherActor, CurrentTime);

    UGameplayStatics::ApplyDamage(
        OtherActor,
        DamageAmount,
        nullptr,
        this,
        nullptr);

    OnTrapTriggered(OtherActor);

    GEngine->AddOnScreenDebugMessage(
        -1,
        1.f,
        FColor::Orange,
        TEXT("Trap Damage"));
}