// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGameInstance.h"

FString UChessGameInstance::GetTurnMessage()
{
	return CurrentTurnMessage;
}

void UChessGameInstance::SetTurnMessage(FString Message)
{
	CurrentTurnMessage = Message;
}
