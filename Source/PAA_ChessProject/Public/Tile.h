// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

// 
UENUM()
enum class EStatus : uint8
{
	EMPTY UMETA(DispayName = "Empty"),
	WHITEOCCUPIED UMETA(DispayName = "WhiteOccupied"),
	BLACKOCCUPIED UMETA(DispayName = "BlackOccupied"),
};

UCLASS()
class PAA_CHESSPROJECT_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	// set the player owner and the status of a tile
	void SetTileStatus(const EStatus TileStatus);

	// get the tile status
	EStatus GetTileStatus();

	// set with the Id of the piece on this tile
	void SetOccupier(TCHAR pieceID);

	// get the Id of the piece occupying this tile
	TCHAR GetOccupier();

	// get the static mesh component
	UStaticMeshComponent* GetStatMeshComp();

	// set the (x, y) position
	void SetGridPosition(const double InX, const double InY);

	// get the (x, y) position
	FVector2D GetGridPosition();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EStatus Status;

	// (x, y) position of the tile
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector2D TileGridPosition;

	// Piece in that tile
	TCHAR Occupier;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};
