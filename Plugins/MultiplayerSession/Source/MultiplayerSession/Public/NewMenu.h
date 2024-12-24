// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Menu.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "NewMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSION_API UNewMenu : public UMenu
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void MenuSetupNew(int32 NumberOfPublicConnections = 4, FString TypeOfMatch = FString(TEXT("FreeForAll")),
		FString LobbyPath = FString(TEXT("/Game/ThirdPerson/Maps/Lobby")));

protected:
	virtual bool Initialize() override;
	//not support after 5.1
	//virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;
	virtual void NativeDestruct() override;

	//callbacks for the custom delegates on the MultiplayerSessionSubsystem
	void OnCreateSession(bool bWasSuccessful);
	void OnFindSession(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySession(bool bWasSuccessful);
	void OnStartSession(bool bWasSuccessful);

private:

	class UButton* HostButton;

	UButton* JoinButton;

	void HostButtonClicked();
	void JoinButtonClicked();

	void MenuTeardown();

	//The subsystem that will handle the session functionality
	class UMultiplayerSessionSubsystem* MultiplayerSessionSubsystem;

	int32 NumPublicConnections{ 4 };
	FString MatchType{ TEXT("FreeForAll") };
	FString PathToLobby{ TEXT("") };

};
