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
	PieceSize = 110;

	FieldStatus = "rnbqkbnr/pppppppp/8/3Q4/8/8/PPPPPPPP/RNBQKBNR";
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
		Obj->SetTileStatus(NOT_ASSIGNED, EStatus::EMPTY);
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
	const float PieceScale = PieceSize / 100;
	for (int32 i = 0; i < FieldStatus.Len(); ++i)
	{
		TCHAR Character = FieldStatus[i];
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
			FVector Location = AGameField::GetPieceRelativeLocationByXYPosition(NewX, NewY);

			if (Character == 'R')
			{
				AChessRook* Obj = GetWorld()->SpawnActor<AChessRook>(RookClass, Location, FRotator::ZeroRotator);
				//const float PieceScale = PieceSize / 100;
				Obj->SetActorScale3D(FVector(PieceScale, PieceScale, 0.2));
				Obj->SetGridPosition(NewX, NewY);
				ATile* Position = TileMap[FVector2D(NewX, NewY)];
				Position->SetTileStatus(EStatus::WHITEOCCUPIED);
				Position->SetOccupier(Character);
				NewY += 1;
				WPieceInGame.Add(Obj);
			}
			if (Character == 'N')
			{
				AChessKnight* Obj = GetWorld()->SpawnActor<AChessKnight>(KnightClass, Location, FRotator::ZeroRotator);
				//const float PieceScale = PieceSize / 100;
				Obj->SetActorScale3D(FVector(PieceScale, PieceScale, 0.2));
				Obj->SetGridPosition(NewX, NewY);
				ATile* Position = TileMap[FVector2D(NewX, NewY)];
				Position->SetTileStatus(EStatus::WHITEOCCUPIED);
				Position->SetOccupier(Character);
				NewY += 1;
				WPieceInGame.Add(Obj);
			}
			if (Character == 'B')
			{
				AChessBishop* Obj = GetWorld()->SpawnActor<AChessBishop>(BishopClass, Location, FRotator::ZeroRotator);
				//const float PieceScale = PieceSize / 100;
				Obj->SetActorScale3D(FVector(PieceScale, PieceScale, 0.2));
				Obj->SetGridPosition(NewX, NewY);
				ATile* Position = TileMap[FVector2D(NewX, NewY)];
				Position->SetTileStatus(EStatus::WHITEOCCUPIED);
				Position->SetOccupier(Character);
				NewY += 1;
				WPieceInGame.Add(Obj);
			}
			if (Character == 'Q')
			{
				AChessQueen* Obj = GetWorld()->SpawnActor<AChessQueen>(QueenClass, Location, FRotator::ZeroRotator);
				//const float PieceScale = PieceSize / 100;
				Obj->SetActorScale3D(FVector(PieceScale, PieceScale, 0.2));
				Obj->SetGridPosition(NewX, NewY);
				ATile* Position = TileMap[FVector2D(NewX, NewY)];
				Position->SetTileStatus(EStatus::WHITEOCCUPIED);
				Position->SetOccupier(Character);
				NewY += 1;
				WPieceInGame.Add(Obj);
			}
			if (Character == 'K')
			{
				AChessKing* Obj = GetWorld()->SpawnActor<AChessKing>(KingClass, Location, FRotator::ZeroRotator);
				//const float PieceScale = PieceSize / 100;
				Obj->SetActorScale3D(FVector(PieceScale, PieceScale, 0.2));
				Obj->SetGridPosition(NewX, NewY);
				ATile* Position = TileMap[FVector2D(NewX, NewY)];
				Position->SetTileStatus(EStatus::WHITEOCCUPIED);
				Position->SetOccupier(Character);
				WhiteKing = Obj;
				NewY += 1;
				WPieceInGame.Add(Obj);
			}
			if (Character == 'P')
			{
				AChessPawn* Obj = GetWorld()->SpawnActor<AChessPawn>(PawnClass, Location, FRotator::ZeroRotator);
				//const float PieceScale = PieceSize / 100;
				Obj->SetActorScale3D(FVector(PieceScale, PieceScale, 0.2));
				Obj->SetGridPosition(NewX, NewY);
				ATile* Position = TileMap[FVector2D(NewX, NewY)];
				Position->SetTileStatus(EStatus::WHITEOCCUPIED);
				Position->SetOccupier(Character);
				NewY += 1;
				WPieceInGame.Add(Obj);
			}


		}
		if (Character >= 'b' && Character <= 'r')
		{
			FVector Location = AGameField::GetPieceRelativeLocationByXYPosition(NewX, NewY);

			if (Character == 'r')
			{
				AChessRook* Obj = GetWorld()->SpawnActor<AChessRook>(RookClass, Location, FRotator::ZeroRotator);
				//const float PieceScale = PieceSize / 100;
				Obj->SetActorScale3D(FVector(PieceScale, PieceScale, 0.2));
				Obj->SetGridPosition(NewX, NewY);
				Obj->ChangeBitColor();
				FString MaterialPath = TEXT("/Game/Materials/M_Brook");
				UMaterialInterface* Material = Cast<UMaterialInterface>(StaticLoadObject(NULL, nullptr, *MaterialPath));
				UStaticMeshComponent* Comp = Obj->GetStatMeshComp();
				Comp->SetMaterial(0, Material);
				ATile* Position = TileMap[FVector2D(NewX, NewY)];
				Position->SetTileStatus(EStatus::BLACKOCCUPIED);
				Position->SetOccupier(Character);
				NewY += 1;
				BPieceInGame.Add(Obj);
			}
			if (Character == 'n')
			{
				AChessKnight* Obj = GetWorld()->SpawnActor<AChessKnight>(KnightClass, Location, FRotator::ZeroRotator);
				//const float PieceScale = PieceSize / 100;
				Obj->SetActorScale3D(FVector(PieceScale, PieceScale, 0.2));
				Obj->SetGridPosition(NewX, NewY);
				Obj->ChangeBitColor();
				FString MaterialPath = TEXT("/Game/Materials/M_Bknight");
				UMaterialInterface* Material = Cast<UMaterialInterface>(StaticLoadObject(NULL, nullptr, *MaterialPath));
				UStaticMeshComponent* Comp = Obj->GetStatMeshComp();
				Comp->SetMaterial(0, Material);
				ATile* Position = TileMap[FVector2D(NewX, NewY)];
				Position->SetTileStatus(EStatus::BLACKOCCUPIED);
				Position->SetOccupier(Character);
				NewY += 1;
				BPieceInGame.Add(Obj);
			}
			if (Character == 'b')
			{
				AChessBishop* Obj = GetWorld()->SpawnActor<AChessBishop>(BishopClass, Location, FRotator::ZeroRotator);
				//const float PieceScale = PieceSize / 100;
				Obj->SetActorScale3D(FVector(PieceScale, PieceScale, 0.2));
				Obj->SetGridPosition(NewX, NewY);
				Obj->ChangeBitColor();
				FString MaterialPath = TEXT("/Game/Materials/M_Bbishop");
				UMaterialInterface* Material = Cast<UMaterialInterface>(StaticLoadObject(NULL, nullptr, *MaterialPath));
				UStaticMeshComponent* Comp = Obj->GetStatMeshComp();
				Comp->SetMaterial(0, Material);
				ATile* Position = TileMap[FVector2D(NewX, NewY)];
				Position->SetTileStatus(EStatus::BLACKOCCUPIED);
				Position->SetOccupier(Character);
				NewY += 1;
				BPieceInGame.Add(Obj);
			}
			if (Character == 'q')
			{
				AChessQueen* Obj = GetWorld()->SpawnActor<AChessQueen>(QueenClass, Location, FRotator::ZeroRotator);
				//const float PieceScale = PieceSize / 100;
				Obj->SetActorScale3D(FVector(PieceScale, PieceScale, 0.2));
				Obj->SetGridPosition(NewX, NewY);
				Obj->ChangeBitColor();
				FString MaterialPath = TEXT("/Game/Materials/M_Bqueen");
				UMaterialInterface* Material = Cast<UMaterialInterface>(StaticLoadObject(NULL, nullptr, *MaterialPath));
				UStaticMeshComponent* Comp = Obj->GetStatMeshComp();
				Comp->SetMaterial(0, Material);
				ATile* Position = TileMap[FVector2D(NewX, NewY)];
				Position->SetTileStatus(EStatus::BLACKOCCUPIED);
				Position->SetOccupier(Character);
				NewY += 1;
				BPieceInGame.Add(Obj);
			}
			if (Character == 'k')
			{
				AChessKing* Obj = GetWorld()->SpawnActor<AChessKing>(KingClass, Location, FRotator::ZeroRotator);
				//const float PieceScale = PieceSize / 100;
				Obj->SetActorScale3D(FVector(PieceScale, PieceScale, 0.2));
				Obj->SetGridPosition(NewX, NewY);
				Obj->ChangeBitColor();
				FString MaterialPath = TEXT("/Game/Materials/M_Bking");
				UMaterialInterface* Material = Cast<UMaterialInterface>(StaticLoadObject(NULL, nullptr, *MaterialPath));
				UStaticMeshComponent* Comp = Obj->GetStatMeshComp();
				Comp->SetMaterial(0, Material);
				ATile* Position = TileMap[FVector2D(NewX, NewY)];
				Position->SetTileStatus(EStatus::BLACKOCCUPIED);
				Position->SetOccupier(Character);
				BlackKing = Obj;
				NewY += 1;
				BPieceInGame.Add(Obj);
			}
			if (Character == 'p')
			{
				AChessPawn* Obj = GetWorld()->SpawnActor<AChessPawn>(PawnClass, Location, FRotator::ZeroRotator);
				//const float PieceScale = PieceSize / 100;
				Obj->SetActorScale3D(FVector(PieceScale, PieceScale, 0.2));
				Obj->SetGridPosition(NewX, NewY);
				Obj->ChangeBitColor();
				FString MaterialPath = TEXT("/Game/Materials/M_Bpawn");
				UMaterialInterface* Material = Cast<UMaterialInterface>(StaticLoadObject(NULL, nullptr, *MaterialPath));
				UStaticMeshComponent* Comp = Obj->GetStatMeshComp();
				Comp->SetMaterial(0, Material);
				ATile* Position = TileMap[FVector2D(NewX, NewY)];
				Position->SetTileStatus(EStatus::BLACKOCCUPIED);
				Position->SetOccupier(Character);
				NewY += 1;
				BPieceInGame.Add(Obj);
			}

		}
	}
}

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

FVector AGameField::GetPieceRelativeLocationByXYPosition(const int32 InX, const int32 InY) const
{
	return TileSize * FVector(InX, InY, 0.1);
}


FVector2D AGameField::GetXYPositionByRelativeLocation(const FVector& Location) const
{
	const double x = Location[0] / (TileSize * NormalizedCellPadding);
	const double y = Location[1] / (TileSize * NormalizedCellPadding);
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("x=%f,y=%f"), x, y));
	return FVector2D(x, y);
}
