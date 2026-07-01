#include "DEPickupBase.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

ADEPickupBase::ADEPickupBase()
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

    PickupSphere = CreateDefaultSubobject<USphereComponent>(
        TEXT("PickupSphere"));
    PickupSphere->SetupAttachment(Root);
    PickupSphere->SetSphereRadius(100.f);
    PickupSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    PickupSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    PickupSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ADEPickupBase::BeginPlay()
{
    Super::BeginPlay();

    PickupSphere->OnComponentBeginOverlap.AddDynamic(
        this,
        &ADEPickupBase::OnPickupOverlap);
}

void ADEPickupBase::OnPickupOverlap(
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

    ApplyPickupEffect(OtherActor);
    OnPickupCollected(OtherActor);
    Destroy();
}

void ADEPickupBase::ApplyPickupEffect(AActor* Picker)
{
}