// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	// template function that creates a components
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// every actor has a RootComponent that defines the transform in the World
	SetRootComponent(Scene);
	StaticMeshComponent->SetupAttachment(Scene);

	// setting the initials property of the tiles
	Status = EStatus::EMPTY;
	TileGridPosition = FVector2D(0, 0);
	Occupier = ' ';

}

void ATile::SetTileStatus(const EStatus TileStatus)
{
	Status = TileStatus;
}

EStatus ATile::GetTileStatus()
{
	return Status;
}

void ATile::SetOccupier(TCHAR pieceID)
{
	Occupier = pieceID;
}

TCHAR ATile::GetOccupier()
{
	return Occupier;
}

UStaticMeshComponent* ATile::GetStatMeshComp()
{
	return StaticMeshComponent;
}

void ATile::SetGridPosition(const double InX, const double InY)
{
	TileGridPosition.Set(InX, InY);
}

FVector2D ATile::GetGridPosition()
{
	return TileGridPosition;
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
/*void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

