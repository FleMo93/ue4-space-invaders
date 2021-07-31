

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/Vector.h"
#include "Containers/Array.h"
#include "Net/UnrealNetwork.h"
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
	TArray<AActor*> Enemies;
	bool Alive = false;
	UWorld* MyWorld;
	AActor* MostLeftEnemy;
	AActor* MostRightEnemy;
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
	AActor* GetMostLeftEnemy();
	AActor* GetMostRightEnemy();

	UFUNCTION(Server, Unreliable)
	void MoveEnemies(float DeltaTime);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SpawnBlock(FVector SpawnPosition, int Columns, int Rows, TSubclassOf<AActor> EnemyType);

	UFUNCTION()
	void OnEnemyDestroyed(AActor* Enemy);
};
