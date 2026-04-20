// Fill out your copyright notice in the Description page of Project Settings.


#include "RecuCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "RecuBatteryComponent.h"

// Sets default values
ARecuCharacter::ARecuCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ARecuCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Inyectamos el Mapping Context al jugador local al iniciar
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
	BatteryComponent = FindComponentByClass<URecuBatteryComponent>();
	bIsTabletActive = false;
}


void ARecuCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Casteamos el componente normal al "Enhanced"
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Vinculamos la acción de Mover (Se dispara mientras se presiona)
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARecuCharacter::Move);
		}

		// Vinculamos la acción de Mirar
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARecuCharacter::Look);
		}
		if (ToggleTabletAction)
		{
			EnhancedInputComponent->BindAction(ToggleTabletAction, ETriggerEvent::Started, this, &ARecuCharacter::ToggleTablet);
		}
	}
}

void ARecuCharacter::Move(const FInputActionValue& Value)
{
	// Obtenemos el Vector2D de tu teclado
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Descubrimos hacia dónde está mirando el controlador
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Obtenemos el vector Adelante y Derecha matemáticamente
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Aplicamos el movimiento (Y = W/S, X = A/D)
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ARecuCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// El motor se encarga de girar la cámara si marcaste "Use Pawn Control Rotation"
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ARecuCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void ARecuCharacter::ToggleTablet(const FInputActionValue& Value)
{
	// Seguridad AAA: Si por alguna razón el personaje no tiene el componente, abortamos para evitar un Crash (NullPointer)
	if (!BatteryComponent) return;

	// Invertimos el estado (Si era false, ahora es true, y viceversa)
	bIsTabletActive = !bIsTabletActive;

	if (bIsTabletActive)
	{
		BatteryComponent->StartUsingTablet();
	}
	else
	{
		BatteryComponent->StopUsingTablet();
	}

	// Avisamos al Blueprint (Arte/Audio) que el estado cambió
	OnTabletStateChanged(bIsTabletActive);
}