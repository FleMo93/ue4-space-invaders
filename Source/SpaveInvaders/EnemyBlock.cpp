

#include "EnemyBlock.h"

AEnemyBlock::AEnemyBlock()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyBlock::BeginPlay()
{
	Super::BeginPlay();
	MyWorld = GetWorld();
	CurrentMovementDirection = InitialMovementDirection;
	FString Message = "Move: " + FString::SanitizeFloat(InitialMovementDirection);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
	Alive = true;
}

void AEnemyBlock::MoveBlock_Implementation()
{
	FVector Origin, Extend;
	GetActorBounds(true, Origin, Extend, true);
	auto Location = GetActorLocation();


	if (CurrentMovementDirection == Left)
	{
		float LeftPosition = Origin.X - (Extend.X / 2);
		if (LeftPosition <= LeftBound)
		{
			// CurrentMovementDirection = MovementDirection::Right;
			FVector NewLocation = FVector(LeftPosition - (Extend.X / 2), Location.Y, Location.Z - RowHeightMovement);
			// SetActorLocation(NewLocation, false, nullptr, ETeleportType::ResetPhysics);
		}
	}
	else if (CurrentMovementDirection == Right)
	{
		float RightPosition = Origin.X + (Extend.X / 2);
		if (RightPosition >= RightBound)
		{
			// CurrentMovementDirection = MovementDirection::Left;
			FVector NewLocation = FVector(RightPosition - (Extend.X / 2), Location.Y, Location.Z - RowHeightMovement);
			// SetActorLocation(NewLocation, false, nullptr, ETeleportType::ResetPhysics);
		}
	}

	float Movement = (InitialMovementDirection == Left) ? -1 : 1;
	// FString Message = "Move: " + FString::SanitizeFloat(Movement);
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);

	if(CurrentMovementDirection == Right) {
		AddMovementInput(FVector(1, 0, 0));
	} else {
		AddMovementInput(FVector(-1, 0, 0));
	}
}

void AEnemyBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Alive || MyWorld->IsPaused())
		return;

	if (GetNetMode() != ENetMode::NM_Client)
		MoveBlock_Implementation();
}

void AEnemyBlock::OnEnemyDestroyed(ABaseEnemy *Enemy)
{
	int32 RemovedEnemiesCount = Enemies.RemoveSwap(Enemy);

	if (RemovedEnemiesCount == 0)
		return;

	if (Enemies.Num() == 0)
		Alive = false;

	Enemy->OnDestroyed.Remove(MyOnEnemyDestroyed);
}