#include "DEDoor.h"

#include "DungeonEscapeGameState.h"
#include "Net/UnrealNetwork.h"

ADEDoor::ADEDoor()
{
    PrimaryActorTick.bCanEverTick = true;

    bReplicates = true;
    SetReplicateMovement(true);

    bIsOpen = false;
    OpenHeight = 300.f;
    OpenSpeed = 2.f;
}

void ADEDoor::BeginPlay()
{
    Super::BeginPlay();

    ClosedLocation = GetActorLocation();
    OpenLocation = ClosedLocation + FVector(0.f, 0.f, OpenHeight);
}

void ADEDoor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (HasAuthority() && !bIsOpen)
    {
        if (ADungeonEscapeGameState* GS =
            GetWorld()->GetGameState<ADungeonEscapeGameState>())
        {
            if (GS->AreAllLeversActivated())
            {
                OpenDoor();
            }
        }
    }

    if (bIsOpen)
    {
        const FVector NewLocation =
            FMath::VInterpTo(
                GetActorLocation(),
                OpenLocation,
                DeltaTime,
                OpenSpeed);

        SetActorLocation(NewLocation);
    }
}

void ADEDoor::OpenDoor()
{
    if (!HasAuthority())
    {
        return;
    }

    if (bIsOpen)
    {
        return;
    }

    bIsOpen = true;

    OnDoorOpened();

    GEngine->AddOnScreenDebugMessage(
        -1,
        2.f,
        FColor::Cyan,
        TEXT("Door Opened"));
}

void ADEDoor::OnRep_DoorOpen()
{
    if (bIsOpen)
    {
        OnDoorOpened();
    }
}

bool ADEDoor::IsOpen() const
{
    return bIsOpen;
}

void ADEDoor::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ADEDoor, bIsOpen);
}