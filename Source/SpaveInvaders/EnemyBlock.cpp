

#include "EnemyBlock.h"

AEnemyBlock::AEnemyBlock()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyBlock::BeginPlay()
{
	MyWorld = GetWorld();
	Super::BeginPlay();
}

AActor *AEnemyBlock::GetMostLeftEnemy()
{
	AActor *MostLeft = NULL;

	for (AActor *Enemy : Enemies)
	{
		if (MostLeft == NULL)
			MostLeft = Enemy;
		else if (Enemy->GetActorLocation().X < MostLeft->GetActorLocation().X)
			MostLeft = Enemy;
	}

	return MostLeft;
}

AActor *AEnemyBlock::GetMostRightEnemy()
{
	AActor *MostRight = NULL;

	for (AActor *Enemy : Enemies)
	{
		if (MostRight == NULL)
			MostRight = Enemy;
		else if (Enemy->GetActorLocation().X > MostRight->GetActorLocation().X)
			MostRight = Enemy;
	}

	return MostRight;
}

void AEnemyBlock::MoveEnemies_Implementation(float DeltaTime)
{
	bool MoveDown = false;
	float Movement = CurrentMovementSpeed * DeltaTime;
	FVector MostLeftLocation = MostLeftEnemy->GetActorLocation();
	FVector MostRightLocation = MostRightEnemy->GetActorLocation();

	if (CurrentMovementDirection == MovementDirection::Left && MostLeftLocation.X <= LeftBound)
	{
		CurrentMovementDirection = MovementDirection::Right;
		MoveDown = true;
	}
	else if (CurrentMovementDirection == MovementDirection::Right && MostRightLocation.X >= RightBound)
	{
		CurrentMovementDirection = MovementDirection::Left;
		MoveDown = true;
	}

	if (CurrentMovementDirection == MovementDirection::Left)
		Movement *= -1;

	if (CurrentMovementDirection == MovementDirection::Left && MostLeftLocation.X + Movement <= LeftBound)
		Movement = -(MostLeftLocation.X - LeftBound);
	else if (CurrentMovementDirection == MovementDirection::Right && MostRightLocation.X + Movement >= RightBound)
		Movement = RightBound - MostRightLocation.X;

	for (AActor *Enemy : Enemies)
	{
		FVector Location = Enemy->GetActorLocation();
		Location.X += Movement;

		if (MoveDown)
			Location.Z -= RowHeightMovement;

		Enemy->SetActorLocation(Location);
	}

	if (MoveDown)
		CurrentMovementSpeed += MovementSpeedIncrease;
}

void AEnemyBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Alive || MyWorld->IsPaused())
		return;

	if(GetNetMode() != ENetMode::NM_Client)
		MoveEnemies(DeltaTime);
}

void AEnemyBlock::OnEnemyDestroyed(AActor *Enemy)
{
	int32 RemovedEnemiesCount = Enemies.RemoveSwap(Enemy);

	if (RemovedEnemiesCount == 0)
		return;

	if (Enemies.Num() >= 0)
	{
		if (Enemy == MostLeftEnemy)
			MostLeftEnemy = GetMostLeftEnemy();
		else
			MostRightEnemy = GetMostRightEnemy();
	}
	else
		Alive = false;

	Enemy->OnDestroyed.Remove(MyOnEnemyDestroyed);
}

void AEnemyBlock::SpawnBlock(FVector SpawnPosition, int Columns, int Rows, TSubclassOf<AActor> EnemyType)
{
	float OriginalXStart = SpawnPosition.X;

	for (int32 z = 0; z < Rows; z++)
	{
		for (int32 x = 0; x < Columns; x++)
		{
			AActor *Enemy = MyWorld->SpawnActor<AActor>(*EnemyType);
			Enemy->SetActorLocation(SpawnPosition, false, nullptr, ETeleportType::TeleportPhysics);

			MyOnEnemyDestroyed.BindUFunction(this, "OnEnemyDestroyed");
			Enemy->OnDestroyed.Add(MyOnEnemyDestroyed);

			SpawnPosition.X += ColumnOffset;
			Enemies.Emplace(Enemy);
		}

		SpawnPosition.X = OriginalXStart;
		SpawnPosition.Z -= RowOffset;
	}

	Enemies.Shrink();
	MostLeftEnemy = GetMostLeftEnemy();
	MostRightEnemy = GetMostRightEnemy();
	Alive = true;
	CurrentMovementSpeed = MovementSpeed;
}
