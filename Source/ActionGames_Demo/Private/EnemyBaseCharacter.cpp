// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBaseCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnemyAIController.h"
#include "DebugHelper.h"
#include "Components/WidgetComponent.h"
#include "HealthBarWidget.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "CombatGameMode.h"
#include "Particles/ParticleSystem.h"
#include "HealthPack.h"



AEnemyBaseCharacter::AEnemyBaseCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	//��ӿؼ����
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(RootComponent);
}

void AEnemyBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	

	//�����ؼ��������ؼ���ֵ���ؼ����
	UHealthBarWidget* healthBarWidget = CreateWidget<UHealthBarWidget>(GetWorld(), HealthBarWidgetClass);
	HealthBar->SetWidget(healthBarWidget);//��Ѫ����ֵ���ؼ����

	healthBarWidget->ownerCharacter = this;//����ownerCharacter


}



void AEnemyBaseCharacter::DeathRepose()
{
	HealthBar->DestroyComponent();
	AAIController* controller = Cast<AAIController>(GetController());
	if (controller) {
		controller->BrainComponent->StopLogic("");//����Ϊ��ͣ��
		controller->ClearFocus(EAIFocusPriority::Default);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		ACombatGameMode* gameMode = Cast<ACombatGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		gameMode->RemoveEnemy(this);

		MyDelay(3.0f, FName("DestroyEnemy"));


	}
}
void AEnemyBaseCharacter::DestroyEnemy()
{
	if (DeathParticle) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathParticle, GetActorTransform());
	}

	if (HealthPackClass) {
		GetWorld()->SpawnActor<AHealthPack>(HealthPackClass, GetActorTransform());
	}

	Destroy();
}

void AEnemyBaseCharacter::HitRepose()
{
	Super::HitRepose();
	if (!isDamage) {
		isDamage = true;
		AEnemyAIController* controller = Cast<AEnemyAIController>(GetController());
		if (controller) {
			controller->SetAIDamage();
		}
	}
}

void AEnemyBaseCharacter::PlayAttack()
{
	Debug::Print("AttackPlayer!");
	isAttacking = true;
	GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage);
}




void AEnemyBaseCharacter::AttackDamage()
{
	FVector start = GetActorLocation();
	FVector end = start + GetActorForwardVector() * 200;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);
	TArray<AActor*> ignoreList;
	TArray<FHitResult> hitList;
	bool isResult= UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(),start,end,50.0f,ObjectTypes,true,ignoreList,EDrawDebugTrace::ForOneFrame,hitList,true);
	if (isResult) {
		APlayerBaseCharacter* player = GertAttackRangePlayer(hitList);
		if (player) {
			player->TakeHitDamage(10.0f);
		}
	}
}

APlayerBaseCharacter* AEnemyBaseCharacter::GertAttackRangePlayer(const TArray<FHitResult> hitList)
{
	APlayerBaseCharacter* player=nullptr;
	for (FHitResult hitRes:hitList) {
		player = Cast<APlayerBaseCharacter>(hitRes.GetActor());
		if (player) {
			break;
		}
	}
	return player;
}