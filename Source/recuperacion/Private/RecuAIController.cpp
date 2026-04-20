
#include "RecuAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"

ARecuAIController::ARecuAIController()
{
	// 1. Instanciar el componente principal
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);

	// 2. Configurar la VISTA
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1500.f; // Distancia máxima de visión
	SightConfig->LoseSightRadius = 1800.f; // Distancia a la que te pierde si te alejas
	SightConfig->PeripheralVisionAngleDegrees = 60.f; // Ángulo del cono visual
	SightConfig->SetMaxAge(5.f); // Cuánto tiempo recuerda haberte visto

	// Por defecto la IA de Unreal solo ve "enemigos". Forzamos a que detecte todo por ahora.
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerception->ConfigureSense(*SightConfig);

	// 3. Configurar el OÍDO (Signal Sniffing)
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = 2000.f; // Rango de intercepción de la tablet
	HearingConfig->SetMaxAge(3.f);
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerception->ConfigureSense(*HearingConfig);

	// El sentido dominante
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());
}

void ARecuAIController::BeginPlay()
{
	Super::BeginPlay();

	// Enlazar la función al evento interno del motor
	if (AIPerception)
	{
		AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &ARecuAIController::OnTargetDetected);
	}
}

void ARecuAIController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
	{
		// Obtenemos las "Identificaciones" de cada sentido en Unreal
		FAISenseID SightID = UAISense::GetSenseID<UAISense_Sight>();
		FAISenseID HearingID = UAISense::GetSenseID<UAISense_Hearing>();

		// ¿El sensor se encendió (te detectó) o se apagó (te perdió)?
		if (Stimulus.WasSuccessfullySensed())
		{
			if (Stimulus.Type == SightID)
			{
				// CONTACTO VISUAL: Prioridad Máxima
				BlackboardComp->SetValueAsObject(FName("TargetActor"), Actor);
				BlackboardComp->SetValueAsBool(FName("HasLineOfSight"), true);
				UE_LOG(LogTemp, Warning, TEXT("CONTACTO VISUAL - Iniciando cacería."));
			}
			else if (Stimulus.Type == HearingID)
			{
				// RUIDO DE TABLET: Prioridad Secundaria
				BlackboardComp->SetValueAsVector(FName("TargetLocation"), Stimulus.StimulusLocation);
				UE_LOG(LogTemp, Warning, TEXT("RUIDO DETECTADO - Inyectando coordenadas."));
			}
		}
		else
		{
			// El sensor falló (Ej. Doblaste una esquina y te perdió de vista)
			if (Stimulus.Type == SightID)
			{
				BlackboardComp->SetValueAsBool(FName("HasLineOfSight"), false);
				UE_LOG(LogTemp, Warning, TEXT("CONTACTO VISUAL PERDIDO."));
			}
		}
	}
}