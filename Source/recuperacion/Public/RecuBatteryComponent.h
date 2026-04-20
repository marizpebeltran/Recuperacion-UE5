#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RecuBatteryComponent.generated.h"

// --- DECLARACIÓN DE DELEGADOS (Eventos para Blueprint) ---
// Estos son los "Megáfonos"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBatteryUpdated, float, CurrentBattery, float, MaxBattery);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBatteryDepleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSignalEmitted, float, SignalStrength);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RECUPERACION_API URecuBatteryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URecuBatteryComponent();

protected:
	virtual void BeginPlay() override;

	// --- VARIABLES DE DATOS ---
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Battery Settings")
	float MaxBattery;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battery Settings")
	float CurrentBattery;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Battery Settings")
	float DrainRate; // Cuánta batería pierde por segundo

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Signal Settings")
	float BaseSignalStrength; // El "ruido" electromagnético que rastreará la IA

	// --- OPTIMIZACIÓN (Timers) ---
	FTimerHandle DrainTimerHandle;
	void DrainBattery(); // Función interna que ejecuta el Timer

public:
	// --- EVENTOS EXPUESTOS AL BLUEPRINT ---
	UPROPERTY(BlueprintAssignable, Category = "Battery Events")
	FOnBatteryUpdated OnBatteryUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Battery Events")
	FOnBatteryDepleted OnBatteryDepleted;

	UPROPERTY(BlueprintAssignable, Category = "Battery Events")
	FOnSignalEmitted OnSignalEmitted;

	// --- FUNCIONES PÚBLICAS DE CONTROL ---
	UFUNCTION(BlueprintCallable, Category = "Battery Actions")
	void StartUsingTablet();

	UFUNCTION(BlueprintCallable, Category = "Battery Actions")
	void StopUsingTablet();

	UFUNCTION(BlueprintPure, Category = "Battery Actions")
	float GetCurrentBattery() const { return CurrentBattery; }
};