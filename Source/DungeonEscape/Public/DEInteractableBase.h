#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DEInteractableBase.generated.h"

class USphereComponent;

UCLASS()
class DUNGEONESCAPE_API ADEInteractableBase : public AActor
{
    GENERATED_BODY()

public:
    ADEInteractableBase();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* Root;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* InteractionSphere;

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void Interact(AActor* Interactor);

    virtual void Interact_Implementation(AActor* Interactor);

    virtual void Tick(float DeltaTime) override;
};