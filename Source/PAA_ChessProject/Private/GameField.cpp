// Fill out your copyright notice in the Description page of Project Settings.


#include "GameField.h"

// Sets default values
AGameField::AGameField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// size of the field (8x8)
	Size = 8;
	// tile dimension
	TileSize = 120;
	// tile padding dimension
	CellPadding = 0;
	// pawns dimendion
	PieceSize = 80;

	FieldStatus = "rnbqkbnr/8/8/8/8/RNBQKBNR";
}

// Called when the game starts or when spawned
void AGameField::BeginPlay()
{
	Super::BeginPlay();
	GenerateField();
	SpawnPawns();
	
}

// Called every frame
//void AGameField::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);

//}

void AGameField::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	//normalized tilepadding
	NormalizedCellPadding = FMath::RoundToDouble(((TileSize + CellPadding) / TileSize) * 100) / 100;
}

void AGameField::ResetField()
{
	/*for (ATile* Obj : TileArray)
	{
		Obj->SetTileStatus(NOT_ASSIGNED, ETileStatus::EMPTY);
	}

	// send broadcast event to registered objects 
	OnResetEvent.Broadcast();

	ATTT_GameMode* GameMode = Cast<ATTT_GameMode>(GetWorld()->GetAuthGameMode());
	GameMode->IsGameOver = false;
	GameMode->MoveCounter = 0;
	GameMode->ChoosePlayerAndStartGame();*/
}


void AGameField::GenerateField()
{
	for (int32 x = 0; x < Size; x++)
	{
		for (int32 y = 0; y < Size; y++)
		{
			FVector Location = AGameField::GetRelativeLocationByXYPosition(x, y);
			ATile* Obj = GetWorld()->SpawnActor<ATile>(TileClass, Location, FRotator::ZeroRotator);
			const float TileScale = TileSize / 100;
			Obj->SetActorScale3D(FVector(TileScale, TileScale, 0.2));
			Obj->SetGridPosition(x, y);
			if (((x + y) % 2) == 0)
			{
				FString MaterialPath = TEXT("/Game/Materials/MI_Black");
				UMaterialInterface* Material = Cast<UMaterialInterface>(StaticLoadObject(NULL, nullptr, *MaterialPath));
				UStaticMeshComponent* Comp = Obj->GetStatMeshComp();
				Comp->SetMaterial(0, Material);
			}
			TileArray.Add(Obj);
			TileMap.Add(FVector2D(x, y), Obj);
		}
	}
}

//spawn per test
void AGameField::SpawnPawns()
{
	int32 NewX = 7;
	int32 NewY = 0;
	FVector2D WhereToSpawn(NewX, NewY);
	FString Status = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
	for (int32 i = 0; i < Status.Len(); ++i)
	{
		TCHAR Character = Status[i];
		if (Character == '/')
		{
			NewX -= 1;
			NewY = 0;
		}
		if (Character >= '1' && Character <= '8')
		{
			int32 NumericValue = Character - '0';
			NewY += NumericValue;
		}
		if (Character >= 'B' && Character <= 'R')
		{
			FVector Location = AGameField::GetRelativeLocationByXYPosition(NewX, NewY);
			AChessRook* Obj = GetWorld()->SpawnActor<AChessRook>(RookClass, Location, FRotator::ZeroRotator);
			const float TileScale = TileSize / 100;
			Obj->SetActorScale3D(FVector(TileScale, TileScale, 0.4));
			Obj->SetGridPosition(NewX, NewY);
			NewY += 1;
		}
		if (Character >= 'b' && Character <= 'r')
		{
			FVector Location = AGameField::GetRelativeLocationByXYPosition(NewX, NewY);
			AChessRook* Obj = GetWorld()->SpawnActor<AChessRook>(RookClass, Location, FRotator::ZeroRotator);
			const float TileScale = TileSize / 100;
			Obj->SetActorScale3D(FVector(TileScale, TileScale, 0.4));
			Obj->SetGridPosition(NewX, NewY);
			Obj->ChangeBitColor();
			FString MaterialPath = TEXT("/Game/Materials/M_Brook");
			UMaterialInterface* Material = Cast<UMaterialInterface>(StaticLoadObject(NULL, nullptr, *MaterialPath));
			UStaticMeshComponent* Comp = Obj->GetStatMeshComp();
			Comp->SetMaterial(0, Material);
			NewY += 1;
		}
		
	}
}


/*void AGameField::SpawnPawns()
{
	float NewX = 7.0f;
	float NewY = 7.0f;
	FVector2D WhereToSpawn(NewX, NewY);
	for (int32 Index = 0; Index < FieldStatus.Len(); ++Index) 
	{
		TCHAR Character = FieldStatus.Mid(Index, 1).GetCharArray()[0];
		//caso in cui è una lettera maiuscola
		if (Character >= 'B' || Character <= 'R')
		{
			if (Character == 'R')
			{
				FVector Location = AGameField::GetRelativeLocationByXYPosition(NewX, NewY);
				AChessRook* Obj = GetWorld()->SpawnActor<AChessRook>(TileClass, Location, FRotator::ZeroRotator);
				const float Scale = TileSize / 100;
				Obj->SetActorScale3D(FVector(Scale, Scale, 0.2));
				Obj->SetGridPosition(NewX, NewY);
			}
			if (Character == 'N')
			{

			}
			if (Character == 'B')
			{

			}
			if (Character == 'Q')
			{

			}
			if (Character == 'K')
			{

			}
			if (Character == 'P')
			{

			}

		}
		//caso in cui è una lettera minuscola
		if (Character >= 'b' || Character <= 'r')
		{
			if (Character == 'r')
			{

			}
			if (Character == 'n')
			{

			}
			if (Character == 'b')
			{

			}
			if (Character == 'q')
			{

			}
			if (Character == 'k')
			{

			}
			if (Character == 'p')
			{

			}

		}
		//caso in cui è un numero
		if (Character >= '1' || Character <= '8')
		{

		}
		//caso carattere separatore
		if (Character == '/')
		{
			continue;
		}
		
	}
}*/

FVector2D AGameField::GetPosition(const FHitResult& Hit)
{
	return Cast<ATile>(Hit.GetActor())->GetGridPosition();
}

TArray<ATile*>& AGameField::GetTileArray()
{
	return TileArray;
}

FString& AGameField::GetStatus()
{
	return FieldStatus;
}



FVector AGameField::GetRelativeLocationByXYPosition(const int32 InX, const int32 InY) const
{
	return TileSize * NormalizedCellPadding * FVector(InX, InY, 0);
}

FVector2D AGameField::GetXYPositionByRelativeLocation(const FVector& Location) const
{
	const double x = Location[0] / (TileSize * NormalizedCellPadding);
	const double y = Location[1] / (TileSize * NormalizedCellPadding);
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("x=%f,y=%f"), x, y));
	return FVector2D(x, y);
}
