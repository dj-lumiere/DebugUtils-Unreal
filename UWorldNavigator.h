#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UWorldNavigator.generated.h"

UCLASS()
class DEBUGUTILS_API UWorldNavigator : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	//FindUActorByPath
	UFUNCTION(BlueprintCallable, Category = "World Navigator")
	static AActor* FindUActorByPath(FString& Path);
	//FindUComponentByPath
	UFUNCTION(BlueprintCallable, Category = "World Navigator")
	static UActorComponent* FindActorComponentByPath(FString& Path);
	UFUNCTION(BlueprintCallable, Category = "World Navigator")
	static USceneComponent* FindSceneComponentByPath(FString& Path);
	//GetWorldPath
	UFUNCTION(BlueprintCallable, Category = "World Navigator")
	static FString GetWorldPath(AActor* Actor);
	UFUNCTION(BlueprintCallable, Category = "World Navigator")
	static FString GetWorldPath(UActorComponent* Component);
	UFUNCTION(BlueprintCallable, Category = "World Navigator")
	static FString GetWorldPath(USceneComponent* Component);

private:
	static TArray<FString> GetSceneComponentHierarchy(USceneComponent* Component);
	static TArray<ULevel*> GetAllLevelsInWorld(UWorld* World);
	static TArray<AActor*> GetAllActorsInLevel(ULevel* Level);
	static TArray<UActorComponent*> GetAllComponentsInActor(AActor* Actor);
	static int32 GetIndexOfSameLevel(ULevel* Level);
	static int32 GetIndexOfSameActor(AActor* Actor);
	static int32 GetIndexOfSameComponent(UActorComponent* Component);
	static int32 GetIndexOfSameComponent(USceneComponent* Component);
};
