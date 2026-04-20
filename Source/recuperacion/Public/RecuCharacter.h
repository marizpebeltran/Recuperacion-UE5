// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "RecuCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class URecuBatteryComponent;

UCLASS()
class RECUPERACION_API ARecuCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARecuCharacter();

protected:
	// --- ENCHUFES PARA EL BLUEPRINT ---
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ToggleTabletAction;

	// 3. Variables de estado y referencia a la batería
	URecuBatteryComponent* BatteryComponent;
	bool bIsTabletActive;

	// 4. La función de respuesta al botón
	void ToggleTablet(const FInputActionValue& Value);

	// --- FUNCIONES DE RESPUESTA ---
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 5. El "Puente" hacia los Artistas. Esta función NO se implementa en el .cpp
	// El motor sabrá que debe buscarla y ejecutarla directamente en el Blueprint.
	UFUNCTION(BlueprintImplementableEvent, Category = "Tablet Events")
	void OnTabletStateChanged(bool bIsActive);

};
