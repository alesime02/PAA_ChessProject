// Fill out your copyright notice in the Description page of Project Settings.


#include "Piece.h"

// Sets default values
APiece::APiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

/*UStaticMeshComponent* APiece::GetStatMeshComp()
{
	return StaticMeshComponent;
}*/

void APiece::SetGridPosition(const double InX, const double InY)
{
	PieceGridPosition.Set(InX, InY);
}

FVector2D APiece::GetGridPosition()
{
	return PieceGridPosition;
}

void APiece::ChangeBitColor()
{
	BitColor = 1;
}

/*int32 APiece::GetColor()
{
	return BitColor;
}*/

// Called when the game starts or when spawned
void APiece::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
/*void APiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

