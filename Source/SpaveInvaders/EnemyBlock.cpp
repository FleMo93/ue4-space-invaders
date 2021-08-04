

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
	Alive = true;
	CharacterMovementComponent = Cast<UCharacterMovementComponent>(GetComponentByClass(UCharacterMovementComponent::StaticClass()));
	CharacterMovementComponent->GravityScale = 0;
	CharacterMovementComponent->MovementMode = EMovementMode::MOVE_Flying;
}

void AEnemyBlock::MoveBlock_Implementation()
{
	FVector Origin, Extend;
	GetActorBounds(true, Origin, Extend, true);
	auto Location = GetActorLocation();
	auto BoundsOffset = FVector(
		Location.X - Origin.X + Extend.X,
		Location.Y - Origin.Y + Extend.Y,
		Location.Z - Origin.Z + Extend.Z);

	auto BoundHit = false;
	FVector NewLocation;

	if (CurrentMovementDirection == Left)
	{
		float LeftPosition = Origin.X - Extend.X;
		if (LeftPosition <= LeftBound)
		{
			CurrentMovementDirection = MovementDirection::Right;
			NewLocation = FVector(LeftPosition - BoundsOffset.X, Location.Y, Location.Z - RowHeightMovement);
			BoundHit = true;
		}
	}
	else if (CurrentMovementDirection == Right)
	{
		float RightPosition = Origin.X + Extend.X;
		if (RightPosition >= RightBound)
		{
			CurrentMovementDirection = MovementDirection::Left;
			NewLocation = FVector(RightPosition - (2 * Extend.X), Location.Y, Location.Z - RowHeightMovement);
			BoundHit = true;
		}
	}

	if (BoundHit)
	{
		CharacterMovementComponent->StopMovementImmediately();
		SetActorLocation(NewLocation, false, nullptr, ETeleportType::None);
		CharacterMovementComponent->MaxFlySpeed += MovementSpeedIncrease;
	}

	float Movement = (CurrentMovementDirection == Left) ? -1 : 1;
	AddMovementInput(FVector(Movement, 0, 0));
}

void AEnemyBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetNetMode() == ENetMode::NM_Client)
		return;

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