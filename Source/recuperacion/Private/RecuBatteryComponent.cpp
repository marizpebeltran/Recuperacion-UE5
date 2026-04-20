#include "RecuBatteryComponent.h"
#include "TimerManager.h" // Necesario para usar los Timers

// Constructor
URecuBatteryComponent::URecuBatteryComponent()
{
	// ¡Apagamos el Tick por rendimiento!
	PrimaryComponentTick.bCanEverTick = false;

	// Valores por defecto AAA
	MaxBattery = 100.0f;
	CurrentBattery = MaxBattery;
	DrainRate = 2.0f; // Pierde 2 unidades por "Tick" de Timer
	BaseSignalStrength = 50.0f; // Rango del Signal Sniffing
}

void URecuBatteryComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentBattery = MaxBattery; // Aseguramos empezar al 100%
}

void URecuBatteryComponent::StartUsingTablet()
{
	if (CurrentBattery > 0.0f)
	{
		// Iniciar un Timer que llame a DrainBattery() cada 1.0 segundos y se repita (true)
		GetWorld()->GetTimerManager().SetTimer(DrainTimerHandle, this, &URecuBatteryComponent::DrainBattery, 1.0f, true);
	}
}

void URecuBatteryComponent::StopUsingTablet()
{
	// Destruimos el Timer. Rendimiento puro, cero consumo.
	GetWorld()->GetTimerManager().ClearTimer(DrainTimerHandle);
}

void URecuBatteryComponent::DrainBattery()
{
	if (CurrentBattery > 0.0f)
	{
		// Restamos la batería y la "clampeamos" para que no baje de cero
		CurrentBattery -= DrainRate;
		CurrentBattery = FMath::Clamp(CurrentBattery, 0.0f, MaxBattery);

		// 1. Gritamos a la UI para que baje la barra de energía
		OnBatteryUpdated.Broadcast(CurrentBattery, MaxBattery);

		// 2. Gritamos a la IA (Signal Sniffing activado)
		OnSignalEmitted.Broadcast(BaseSignalStrength);

		// Si llegamos a cero, forzamos el apagado
		if (CurrentBattery <= 0.0f)
		{
			StopUsingTablet();
			OnBatteryDepleted.Broadcast(); // Gritamos que morimos
		}
	}
}