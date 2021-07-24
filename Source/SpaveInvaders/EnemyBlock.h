

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/Vector.h"
#include "Containers/Array.h"
#include "Enemy.h"
#include "EnemyBlock.generated.h"

UENUM(BlueprintType)
enum MovementDirection
{
	Left = 0 UMETA(DisplayName = "Left"),
	Right = 1 UMETA(DUsplaceName = "Right"),
};

UCLASS()
class SPAVEINVADERS_API AEnemyBlock : public AActor
{
	GENERATED_BODY()

private:
	TArray<AEnemy*> Enemies;
	bool Alive = false;
	UWorld* MyWorld;
	AEnemy* MostLeftEnemy;
	AEnemy* MostRightEnemy;
	FScriptDelegate MyOnEnemyDestroyed;
	TEnumAsByte<MovementDirection> CurrentMovementDirection = InitialMovementDirection;
	float CurrentMovementSpeed = MovementSpeed;

public:
	AEnemyBlock();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bound Area")
	float LeftBound = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bound Area")
	float RightBound = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float ColumnOffset = 150;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float RowOffset = 150;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviour")
	float MovementSpeed = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviour")
	float MovementSpeedIncrease = 50;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Behaviour")
	TEnumAsByte<MovementDirection> InitialMovementDirection;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Behaviour")
	float RowHeightMovement = 100;

private:
	AEnemy* GetMostLeftEnemy();
	AEnemy* GetMostRightEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SpawnBlock(FVector SpawnPosition, int Columns, int Rows, TSubclassOf<AEnemy> EnemyType);

	UFUNCTION()
	void OnEnemyDestroyed(AEnemy* Enemy);
};
