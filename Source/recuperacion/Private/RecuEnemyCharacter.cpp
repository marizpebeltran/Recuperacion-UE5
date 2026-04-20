// Fill out your copyright notice in the Description page of Project Settings.


#include "RecuEnemyCharacter.h"

// Sets default values
ARecuEnemyCharacter::ARecuEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARecuEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARecuEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARecuEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

