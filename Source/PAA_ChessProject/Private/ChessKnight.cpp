// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessKnight.h"

// Sets default values
AChessKnight::AChessKnight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// template function that creates a components
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// every actor has a RootComponent that defines the transform in the World
	SetRootComponent(Scene);
	StaticMeshComponent->SetupAttachment(Scene);

}

UStaticMeshComponent* AChessKnight::GetStatMeshComp()
{
	return StaticMeshComponent;
}

void AChessKnight::PossibleMoves(AGameField* Field)
{
}

// Called when the game starts or when spawned
void AChessKnight::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
/*void AChessKnight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

