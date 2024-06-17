// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"
#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerBaseCharacter.h"
#include "EnemyBaseCharacter.h"
#include "DebugHelper.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	ownerCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
}

void UCharacterAnimInstance::NativeUpdateAnimation(float daltaTime)
{
	if (ownerCharacter) {
		//��ȡ������ͼ��Ӧ��ɫ���ٶ�
		FVector velocity = ownerCharacter->GetVelocity();

		speed = velocity.Length();
		//��ȡ�ƶ�����
		FRotator rotation = ownerCharacter->GetActorRotation();

		direction = CalculateDirection(velocity, rotation);//����ò�Ʊ�Kismet�滻���ˣ���ʱ���ûᾯ����ѣ�����Ҳû�����棬�ݶ�
		//�ж��Ƿ����ڵ���
		UCharacterMovementComponent* movement = ownerCharacter->GetCharacterMovement();

		isFalling = movement->IsFalling();
		//����ǰ��ɫת��Ϊplayer
		APlayerBaseCharacter* player = Cast<APlayerBaseCharacter>(ownerCharacter);
		if (player) {
			isAttackAnim = player->isAttackAnim;//ת���ɹ���ֵ��ǰ�Ķ���״̬
		}
		if (ownerCharacter->GetHealthProgress()<= 0)isDeath = true;
	}
}

void UCharacterAnimInstance::AnimNotify_WeaponToHand(UAnimNotify* Notify)
{
	//Debug::Print("RunWeapon To Hand");
	SwitchWeaponPont(FName("WeaponToHand"));
}

void UCharacterAnimInstance::AnimNotify_SwitchWeaponPose(UAnimNotify* Notify)
{
	//Debug::Print("RunSwitchWeaponPose");
	SwitchWeaponPont(FName("WeaponAttackPoint"));
}

void UCharacterAnimInstance::AnimNotify_UnEquipWeapon(UAnimNotify* Notify)
{
	SwitchWeaponPont(FName("NormalWeaponPoint"));
}
//װ����ɵĶ���֪ͨ����
void UCharacterAnimInstance::AnimNotify_EquipEnd(UAnimNotify* Notify)
{
	APlayerBaseCharacter* playerCharacter = Cast<APlayerBaseCharacter>(ownerCharacter);
	if (playerCharacter) {
		playerCharacter->SetCharacterMovement(EMovementMode::MOVE_Walking);
	}
}

//�л������ҵ㷽��
void UCharacterAnimInstance::SwitchWeaponPont(FName pointName)
{
	APlayerBaseCharacter* playerCharacter = Cast<APlayerBaseCharacter>(ownerCharacter);
	if (playerCharacter) {
		playerCharacter->SwitchWeaponPoint(pointName);
	}
}

//���˹�������
void UCharacterAnimInstance::AnimNotify_AttackEnd(UAnimNotify* Notify)
{
	ABaseCharacter* enemyCharacter= Cast<ABaseCharacter>(ownerCharacter);
	if (enemyCharacter) {
		enemyCharacter->isAttacking = false;
	}
}

void UCharacterAnimInstance::AnimNotify_AttackDamage(UAnimNotify* Notify)
{
	if (ownerCharacter) {
		ownerCharacter->AttackDamage();
	}
}
