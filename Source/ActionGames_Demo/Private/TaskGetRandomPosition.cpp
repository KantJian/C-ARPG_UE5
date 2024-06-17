// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskGetRandomPosition.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"


UTaskGetRandomPosition::UTaskGetRandomPosition()
{
	NodeName = FString("GetRandomPosition");
}

EBTNodeResult::Type UTaskGetRandomPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//�õ��ڰ���AIController
	AAIController* controller = OwnerComp.GetAIOwner();
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
	if (!controller||!blackboard) return EBTNodeResult::Failed;
	//�õ�AIController��Ӧ��Pawn
	APawn* pawn = controller->GetPawn();
	if (!pawn)return EBTNodeResult::Failed;
	FVector location = pawn->GetActorLocation();
	//�õ�����ϵͳ���ǵ���Build.cs����ӵ���ģ�������Ȼ������������������VS��
	UNavigationSystemV1* navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	if(!navSystem)return EBTNodeResult::Failed;
	//���ݵ����������λ��
	FNavLocation navLocation;
	bool isFound=navSystem->GetRandomReachablePointInRadius(location,Radius,navLocation);
	if(!isFound)return EBTNodeResult::Failed;
	//���������λ�ø�ֵ���ڰ��
	blackboard->SetValueAsVector(MovePositionKey.SelectedKeyName,navLocation.Location);
	return EBTNodeResult::Succeeded;
}
