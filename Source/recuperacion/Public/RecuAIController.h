#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h" // Necesario para la estructura FAIStimulus
#include "RecuAIController.generated.h"

// Forward Declarations de los sentidos AAA
class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;

UCLASS()
class RECUPERACION_API ARecuAIController : public AAIController
{
	GENERATED_BODY()

public:
	ARecuAIController();

protected:
	virtual void BeginPlay() override;

	// --- COMPONENTES DE PERCEPCIÓN ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Perception")
	UAIPerceptionComponent* AIPerception;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Perception")
	UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Perception")
	UAISenseConfig_Hearing* HearingConfig;

	// --- FUNCIÓN DE RESPUESTA SENSORIAL ---
	// Esta función se dispara cuando el enemigo ve o escucha algo
	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus);
};