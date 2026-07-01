#include "DEVictoryZone.h"

#include "Components/BoxComponent.h"
#include "DEPlayerCharacter.h"
#include "DungeonEscapeGameState.h"

ADEVictoryZone::ADEVictoryZone()
{
    PrimaryActorTick.bCanEverTick = false;

    bReplicates = true;

    Root = CreateDefaultSubobject<USceneComponent>(
        TEXT("Root"));
    RootComponent = Root;

    VictoryBox = CreateDefaultSubobject<UBoxComponent>(
        TEXT("VictoryBox"));
    VictoryBox->SetupAttachment(Root);
    VictoryBox->SetBoxExtent(FVector(200.f, 200.f, 150.f));
    VictoryBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    VictoryBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    VictoryBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    RequiredPlayersInZone = 1;
}

void ADEVictoryZone::BeginPlay()
{
    Super::BeginPlay();

    VictoryBox->OnComponentBeginOverlap.AddDynamic(
        this,
        &ADEVictoryZone::OnVictoryBoxBeginOverlap);

    VictoryBox->OnComponentEndOverlap.AddDynamic(
        this,
        &ADEVictoryZone::OnVictoryBoxEndOverlap);
}

void ADEVictoryZone::OnVictoryBoxBeginOverlap(
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

    if (!PlayersInZone.Contains(OtherActor))
    {
        PlayersInZone.Add(OtherActor);
    }

    CheckVictoryCondition();
}

void ADEVictoryZone::OnVictoryBoxEndOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    if (!HasAuthority())
    {
        return;
    }

    if (!OtherActor)
    {
        return;
    }

    PlayersInZone.Remove(OtherActor);
}

void ADEVictoryZone::CheckVictoryCondition()
{
    if (!HasAuthority())
    {
        return;
    }

    ADungeonEscapeGameState* GS =
        GetWorld()->GetGameState<ADungeonEscapeGameState>();

    if (!GS)
    {
        return;
    }

    if (!GS->AreAllLeversActivated())
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            1.f,
            FColor::Red,
            TEXT("Exit Locked: Activate all levers"));
        return;
    }

    if (PlayersInZone.Num() >= RequiredPlayersInZone)
    {
        GS->SetGameWon();
        OnVictoryTriggered();

        GEngine->AddOnScreenDebugMessage(
            -1,
            3.f,
            FColor::Green,
            TEXT("VICTORY"));
    }
}