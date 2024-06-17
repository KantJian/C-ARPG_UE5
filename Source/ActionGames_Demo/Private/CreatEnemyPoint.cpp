// Fill out your copyright notice in the Description page of Project Settings.


#include "CreatEnemyPoint.h"

#include "Kismet/GameplayStatics.h"
#include "CombatGameMode.h"


// Sets default values
ACreatEnemyPoint::ACreatEnemyPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACreatEnemyPoint::BeginPlay()
{
	Super::BeginPlay();

	ACombatGameMode* gameMode = Cast<ACombatGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (gameMode) {
		if (IsBoss) {
			gameMode->BossCreatePoint = this;
		}
		else {
			gameMode->AddPointToList(this);
		}
	}
}

// Called every frame
void ACreatEnemyPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

