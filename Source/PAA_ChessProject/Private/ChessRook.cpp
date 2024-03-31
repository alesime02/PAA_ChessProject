// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessRook.h"

// Sets default values
AChessRook::AChessRook()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// template function that creates a components
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// every actor has a RootComponent that defines the transform in the World
	SetRootComponent(Scene);
	StaticMeshComponent->SetupAttachment(Scene);

	/*RookGridPosition = FVector2D(0, 0); */
}

//aggiunto per i test
UStaticMeshComponent* AChessRook::GetStatMeshComp()
{
	return StaticMeshComponent;
}
/*
//aggiunto per i test
int32 AChessRook::GetColor()
{
	return BitColor;
}

void AChessRook::ChangeBitColor()
{
	BitColor = 0;
}

//aggiunto per i test
void AChessRook::SetGridPosition(const double InX, const double InY)
{
	RookGridPosition.Set(InX, InY);
}

//aggiunto per i test
FVector2D AChessRook::GetGridPosition()
{
	return RookGridPosition;
}*/

// Called when the game starts or when spawned
void AChessRook::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
/*void AChessRook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

