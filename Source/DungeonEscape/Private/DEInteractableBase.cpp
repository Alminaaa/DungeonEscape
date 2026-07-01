#include "DEInteractableBase.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

ADEInteractableBase::ADEInteractableBase()
{
    PrimaryActorTick.bCanEverTick = false;

    Root = CreateDefaultSubobject<USceneComponent>(
        TEXT("Root"));
    RootComponent = Root;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(
        TEXT("Mesh"));
    Mesh->SetupAttachment(Root);

    InteractionSphere =
        CreateDefaultSubobject<USphereComponent>(
            TEXT("InteractionSphere"));

    InteractionSphere->SetupAttachment(Root);
    InteractionSphere->SetSphereRadius(200.f);
}

void ADEInteractableBase::BeginPlay()
{
    Super::BeginPlay();
}

void ADEInteractableBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ADEInteractableBase::Interact_Implementation(
    AActor* Interactor)
{
}