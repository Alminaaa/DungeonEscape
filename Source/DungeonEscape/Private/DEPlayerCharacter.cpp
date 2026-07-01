#include "DEPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"

#include "DEInteractableBase.h"
#include "DEEnemyBase.h"
#include "DungeonEscapeGameState.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ADEPlayerCharacter::ADEPlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    bReplicates = true;
    SetReplicateMovement(true);

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(
        TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 400.f;
    SpringArm->bUsePawnControlRotation = true;

    Camera = CreateDefaultSubobject<UCameraComponent>(
        TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);

    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;

    WalkSpeed = 500.f;
    SprintSpeed = 800.f;
    DashStrength = 1200.f;
    InteractionDistance = 500.f;

    AttackRange = 180.f;
    AttackRadius = 80.f;
    AttackCooldown = 0.7f;
    bCanAttack = true;
}

void ADEPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PC =
        Cast<APlayerController>(GetController()))
    {
        PC->SetInputMode(FInputModeGameOnly());
        PC->bShowMouseCursor = false;
    }

    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

    if (APlayerController* PC =
        Cast<APlayerController>(GetController()))
    {
        if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
                LocalPlayer->GetSubsystem<
                UEnhancedInputLocalPlayerSubsystem>())
            {
                if (DefaultMappingContext)
                {
                    Subsystem->AddMappingContext(
                        DefaultMappingContext,
                        0);
                }
            }
        }
    }
}

void ADEPlayerCharacter::SetupPlayerInputComponent(
    UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput =
        Cast<UEnhancedInputComponent>(
            PlayerInputComponent))
    {
        EnhancedInput->BindAction(
            MoveAction,
            ETriggerEvent::Triggered,
            this,
            &ADEPlayerCharacter::Move);

        EnhancedInput->BindAction(
            LookAction,
            ETriggerEvent::Triggered,
            this,
            &ADEPlayerCharacter::Look);

        EnhancedInput->BindAction(
            JumpAction,
            ETriggerEvent::Started,
            this,
            &ACharacter::Jump);

        EnhancedInput->BindAction(
            JumpAction,
            ETriggerEvent::Completed,
            this,
            &ACharacter::StopJumping);

        EnhancedInput->BindAction(
            SprintAction,
            ETriggerEvent::Started,
            this,
            &ADEPlayerCharacter::StartSprint);

        EnhancedInput->BindAction(
            SprintAction,
            ETriggerEvent::Completed,
            this,
            &ADEPlayerCharacter::StopSprint);

        EnhancedInput->BindAction(
            DashAction,
            ETriggerEvent::Started,
            this,
            &ADEPlayerCharacter::Dash);

        EnhancedInput->BindAction(
            AttackAction,
            ETriggerEvent::Started,
            this,
            &ADEPlayerCharacter::Attack);

        EnhancedInput->BindAction(
            InteractAction,
            ETriggerEvent::Started,
            this,
            &ADEPlayerCharacter::Interact);
    }
}

void ADEPlayerCharacter::Move(const FInputActionValue& Value)
{
    const FVector2D Movement =
        Value.Get<FVector2D>();

    if (Controller)
    {
        const FRotator Rotation =
            Controller->GetControlRotation();

        const FRotator YawRotation(
            0.f,
            Rotation.Yaw,
            0.f);

        const FVector Forward =
            FRotationMatrix(YawRotation)
            .GetUnitAxis(EAxis::X);

        const FVector Right =
            FRotationMatrix(YawRotation)
            .GetUnitAxis(EAxis::Y);

        AddMovementInput(Forward, Movement.Y);
        AddMovementInput(Right, Movement.X);
    }
}

void ADEPlayerCharacter::Look(
    const FInputActionValue& Value)
{
    const FVector2D LookAxis =
        Value.Get<FVector2D>();

    AddControllerYawInput(LookAxis.X);
    AddControllerPitchInput(LookAxis.Y);
}

void ADEPlayerCharacter::StartSprint()
{
    GetCharacterMovement()->MaxWalkSpeed =
        SprintSpeed;
}

void ADEPlayerCharacter::StopSprint()
{
    GetCharacterMovement()->MaxWalkSpeed =
        WalkSpeed;
}

void ADEPlayerCharacter::Dash()
{
    LaunchCharacter(
        GetActorForwardVector() * DashStrength,
        true,
        true);
}

void ADEPlayerCharacter::Attack()
{
    if (!bCanAttack)
    {
        return;
    }

    if (!HasAuthority())
    {
        Server_Attack();
        return;
    }

    ServerAttackLogic();
}

void ADEPlayerCharacter::Server_Attack_Implementation()
{
    ServerAttackLogic();
}

void ADEPlayerCharacter::ServerAttackLogic()
{
    if (!bCanAttack)
    {
        return;
    }

    bCanAttack = false;

    GetWorldTimerManager().SetTimer(
        AttackCooldownTimerHandle,
        this,
        &ADEPlayerCharacter::ResetAttack,
        AttackCooldown,
        false);

    OnPlayerAttack();

    const FVector Start =
        GetActorLocation() +
        FVector(0.f, 0.f, 50.f);

    const FVector End =
        Start +
        GetActorForwardVector() * AttackRange;

    TArray<FHitResult> HitResults;

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    const bool bHit =
        GetWorld()->SweepMultiByChannel(
            HitResults,
            Start,
            End,
            FQuat::Identity,
            ECC_Pawn,
            FCollisionShape::MakeSphere(AttackRadius),
            Params);

    DrawDebugSphere(
        GetWorld(),
        End,
        AttackRadius,
        16,
        bHit ? FColor::Red : FColor::White,
        false,
        1.f);

    if (!bHit)
    {
        return;
    }

    TSet<AActor*> DamagedActors;

    for (const FHitResult& Hit : HitResults)
    {
        AActor* HitActor = Hit.GetActor();

        if (!HitActor || DamagedActors.Contains(HitActor))
        {
            continue;
        }

        ADEEnemyBase* Enemy =
            Cast<ADEEnemyBase>(HitActor);

        if (!Enemy)
        {
            continue;
        }

        DamagedActors.Add(HitActor);

        UGameplayStatics::ApplyDamage(
            Enemy,
            BaseDamage,
            GetController(),
            this,
            nullptr);

        OnPlayerHitEnemy(Enemy);

        GEngine->AddOnScreenDebugMessage(
            -1,
            1.f,
            FColor::Red,
            TEXT("Enemy Hit"));
    }
}

void ADEPlayerCharacter::ResetAttack()
{
    bCanAttack = true;
}

void ADEPlayerCharacter::Interact()
{
    if (!HasAuthority())
    {
        Server_Interact();
        return;
    }

    ServerInteractLogic();
}

void ADEPlayerCharacter::Server_Interact_Implementation()
{
    ServerInteractLogic();
}

void ADEPlayerCharacter::ServerInteractLogic()
{
    AActor* FocusedActor =
        FindFocusedInteractable();

    if (!FocusedActor)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            1.f,
            FColor::Red,
            TEXT("No Interactable Found"));
        return;
    }

    if (ADEInteractableBase* Interactable =
        Cast<ADEInteractableBase>(FocusedActor))
    {
        Interactable->Interact(this);
    }
}

AActor* ADEPlayerCharacter::FindFocusedInteractable() const
{
    if (!Controller)
    {
        return nullptr;
    }

    FVector Start;
    FRotator ViewRotation;

    Controller->GetPlayerViewPoint(
        Start,
        ViewRotation);

    const FVector End =
        Start +
        ViewRotation.Vector() * InteractionDistance;

    FHitResult HitResult;

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    const bool bHit =
        GetWorld()->LineTraceSingleByChannel(
            HitResult,
            Start,
            End,
            ECC_Visibility,
            Params);

    DrawDebugLine(
        GetWorld(),
        Start,
        End,
        bHit ? FColor::Green : FColor::Red,
        false,
        1.f,
        0,
        2.f);

    if (bHit)
    {
        return HitResult.GetActor();
    }

    return nullptr;
}

void ADEPlayerCharacter::Die()
{
    if (HasAuthority())
    {
        if (ADungeonEscapeGameState* GS =
            GetWorld()->GetGameState<ADungeonEscapeGameState>())
        {
            GS->SetGameLost();
        }
    }

    OnPlayerDeath();

    Super::Die();
}