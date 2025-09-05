#include "UWorldNavigator.h"
#include "FPathStructure.h"

UActorComponent* UWorldNavigator::FindActorComponentByPath(FString& Path)
{
	if (Path.IsEmpty())
	{
		return nullptr;
	}
	if (Path == TEXT("[invalid world]"))
	{
		return nullptr;
	}
	if (Path == TEXT("[invalid level]"))
	{
		return nullptr;
	}
	if (Path == TEXT("[invalid actor]"))
	{
		return nullptr;
	}
	auto ErrorMessage = FString();
	FPathStructure Result;
	if (!FPathStructure::TryParse(Path, ErrorMessage, Result))
	{
		UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMessage);
	}
	const auto World = FindObject<UWorld>(ANY_PACKAGE, *Result.WorldName);
	if (World == nullptr)
	{
		return nullptr;
	}
	int32 sameNumberHit = 0;
	const auto Levels = GetAllLevelsInWorld(World);
	ULevel* MatchingLevel = nullptr;
	for (auto Level : Levels)
	{
		if (Level->GetName() != *Result.LevelName)
		{
			continue;
		}
		if (sameNumberHit == Result.LevelIndex)
		{
			MatchingLevel = Level;
			break;
		}
		sameNumberHit += 1;
	}
	if (MatchingLevel == nullptr)
	{
		return nullptr;
	}
	sameNumberHit = 0;
	const auto Actors = GetAllActorsInLevel(MatchingLevel);
	AActor* MatchingActor = nullptr;
	for (auto Actor : Actors)
	{
		if (Actor->GetName() != *Result.ActorName)
		{
			continue;
		}
		if (sameNumberHit == Result.ActorIndex)
		{
			MatchingActor = Actor;
			break;
		}
		sameNumberHit += 1;
	}
	if (MatchingActor == nullptr)
	{
		return nullptr;
	}
	const auto Components = GetAllComponentsInActor(MatchingActor);
	UActorComponent* MatchingComponent = nullptr;
	if (Result.ComponentNames.Num() != Result.ComponentIndices.Num())
	{
		throw FString(TEXT("Component names and indices must be the same length"));
	}
	if (Result.ComponentNames.Num() != 1)
	{
		throw FString(TEXT("Only one component layer should be specified"));
	}
	for (int32 i = 0; i < Result.ComponentNames.Num(); ++i)
	{
		sameNumberHit = 0;
		const auto ComponentName = Result.ComponentNames[i];
		for (auto Component : Components)
		{
			if (Component->GetName() != *ComponentName)
			{
				continue;
			}
			if (sameNumberHit == Result.ComponentIndices[i])
			{
				MatchingComponent = Component;
				break;
			}
			++sameNumberHit;
		}
		if (sameNumberHit < Result.ComponentIndices[i])
		{
			return nullptr;
		}
	}
	if (MatchingComponent == nullptr)
	{
		return nullptr;
	}
	return MatchingComponent;
}

USceneComponent* UWorldNavigator::FindSceneComponentByPath(FString& Path)
{
	if (Path.IsEmpty())
	{
		return nullptr;
	}
	if (Path == TEXT("[invalid world]"))
	{
		return nullptr;
	}
	if (Path == TEXT("[invalid level]"))
	{
		return nullptr;
	}
	if (Path == TEXT("[invalid actor]"))
	{
		return nullptr;
	}
	auto ErrorMessage = FString();
	FPathStructure Result;
	if (!FPathStructure::TryParse(Path, ErrorMessage, Result))
	{
		UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMessage);
	}
	const auto World = FindObject<UWorld>(ANY_PACKAGE, *Result.WorldName);
	if (World == nullptr)
	{
		return nullptr;
	}
	int32 sameNumberHit = 0;
	const auto Levels = GetAllLevelsInWorld(World);
	ULevel* MatchingLevel = nullptr;
	for (auto Level : Levels)
	{
		if (Level->GetName() != *Result.LevelName)
		{
			continue;
		}
		if (sameNumberHit == Result.LevelIndex)
		{
			MatchingLevel = Level;
			break;
		}
		sameNumberHit += 1;
	}
	if (MatchingLevel == nullptr)
	{
		return nullptr;
	}
	sameNumberHit = 0;
	const auto Actors = GetAllActorsInLevel(MatchingLevel);
	AActor* MatchingActor = nullptr;
	for (auto Actor : Actors)
	{
		if (Actor->GetName() != *Result.ActorName)
		{
			continue;
		}
		if (sameNumberHit == Result.ActorIndex)
		{
			MatchingActor = Actor;
			break;
		}
		sameNumberHit += 1;
	}
	if (MatchingActor == nullptr)
	{
		return nullptr;
	}
	const auto Components = GetAllComponentsInActor(MatchingActor);
	USceneComponent* MatchingComponent = nullptr;
	if (Result.ComponentNames.Num() != Result.ComponentIndices.Num())
	{
		throw FString(TEXT("Component names and indices must be the same length"));
	}
	if (Result.ComponentNames.Num() == 0)
	{
		throw FString(TEXT("At least one component layer should be specified"));
	}
	for (int32 i = 0; i < Result.ComponentNames.Num(); ++i)
	{
		sameNumberHit = 0;
		const auto ComponentName = Result.ComponentNames[i];
		for (auto Component : Components)
		{
			if (USceneComponent* SceneComponent = Cast<USceneComponent>(Component))
			{
				if (SceneComponent->GetName() != *ComponentName)
				{
					continue;
				}
				if (sameNumberHit == Result.ComponentIndices[i])
				{
					MatchingComponent = SceneComponent;
					break;
				}
				++sameNumberHit;
			}
		}
		if (sameNumberHit < Result.ComponentIndices[i])
		{
			return nullptr;
		}
	}
	if (MatchingComponent == nullptr)
	{
		return nullptr;
	}
	return MatchingComponent;
}

AActor* UWorldNavigator::FindUActorByPath(FString& Path)
{
	if (Path.IsEmpty())
	{
		return nullptr;
	}
	if (Path == TEXT("[invalid world]"))
	{
		return nullptr;
	}
	if (Path == TEXT("[invalid level]"))
	{
		return nullptr;
	}
	auto ErrorMessage = FString();
	FPathStructure Result;
	if (!FPathStructure::TryParse(Path, ErrorMessage, Result))
	{
		UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMessage);
	}
	const auto World = FindObject<UWorld>(ANY_PACKAGE, *Result.WorldName);
	if (World == nullptr)
	{
		return nullptr;
	}
	const auto Level = FindObject<ULevel>(World, *Result.LevelName);
	if (Level == nullptr)
	{
		return nullptr;
	}
	const auto Actor = FindObject<AActor>(Level, *Result.ActorName);
	if (Actor == nullptr)
	{
		return nullptr;
	}
	return Actor;
}

TArray<FString> UWorldNavigator::GetSceneComponentHierarchy(USceneComponent* Component)
{
	TArray<FString> Result;
	auto StartComponent = Component;
	const auto StartComponentName = StartComponent->GetName();
	const auto StartComponentIndex = GetIndexOfSameComponent(StartComponent);
	Result.Add(FString::Printf(TEXT("%s[%d]"), *StartComponentName, StartComponentIndex));
	while (true)
	{
		const auto ParentComponent = StartComponent->GetAttachParent();
		if (ParentComponent == nullptr)
		{
			break;
		}
		auto ComponentName = ParentComponent->GetName();
		const auto ComponentIndex = GetIndexOfSameComponent(ParentComponent);
		Result.Add(FString::Printf(TEXT("%s[%d]"), *ComponentName, ComponentIndex));
		StartComponent = ParentComponent;
	}
	Algo::Reverse(Result);
	return Result;
}

FString UWorldNavigator::GetWorldPath(AActor* Actor)
{
	if (Actor == nullptr)
	{
		return TEXT("[invalid actor]");
	}
	const auto World = Actor->GetWorld();
	if (World == nullptr)
	{
		return TEXT("[invalid world]");
	}
	const auto Level = Actor->GetLevel();
	if (Level == nullptr)
	{
		return TEXT("[invalid level]");
	}
	const auto WorldName = World->GetName();
	const auto LevelName = Level->GetName();
	const auto LevelIndex = GetIndexOfSameLevel(Level);
	const auto ActorName = Actor->GetName();
	const auto ActorIndex = GetIndexOfSameActor(Actor);
	return FString(FString::Printf(TEXT("%s@%s[%d]:/%s[%d]"), *WorldName, *LevelName, LevelIndex, *ActorName,
	                               ActorIndex));
}

FString UWorldNavigator::GetWorldPath(UActorComponent* Component)
{
	if (Component == nullptr)
	{
		return TEXT("[invalid component]");
	}
	const auto World = Component->GetWorld();
	if (World == nullptr)
	{
		return TEXT("[invalid world]");
	}
	const auto Level = Component->GetComponentLevel();
	if (Level == nullptr)
	{
		return TEXT("[invalid level]");
	}
	const auto Actor = Component->GetOwner();
	if (Actor == nullptr)
	{
		return TEXT("[invalid actor]");
	}
	if (USceneComponent* SceneComponent = Cast<USceneComponent>(Component))
	{
		return GetWorldPath(SceneComponent);
	}
	const auto WorldName = World->GetName();
	const auto LevelName = Level->GetName();
	const auto LevelIndex = GetIndexOfSameLevel(Level);
	const auto ActorName = Actor->GetName();
	const auto ActorIndex = GetIndexOfSameActor(Actor);
	const auto ComponentName = Component->GetName();
	const auto ComponentIndex = GetIndexOfSameComponent(Component);
	return FString(FString::Printf(TEXT("%s@%s[%d]:/%s[%d]->%s[%d]"), *WorldName, *LevelName, LevelIndex, *ActorName,
	                               ActorIndex, *ComponentName, ComponentIndex));
}

FString UWorldNavigator::GetWorldPath(USceneComponent* Component)
{
	if (Component == nullptr)
	{
		return TEXT("[null Component]");
	}
	const auto World = Component->GetWorld();
	if (World == nullptr)
	{
		return TEXT("[null World]");
	}
	const auto Level = Component->GetComponentLevel();
	if (Level == nullptr)
	{
		return TEXT("[null Level]");
	}
	const auto Actor = Component->GetOwner();
	if (Actor == nullptr)
	{
		return TEXT("[null Actor]");
	}
	const auto WorldName = World->GetName();
	const auto LevelName = Level->GetName();
	const auto LevelIndex = GetIndexOfSameLevel(Level);
	const auto ActorName = Actor->GetName();
	const auto ActorIndex = GetIndexOfSameActor(Actor);
	const auto ComponentHierarchy = GetSceneComponentHierarchy(Component);
	const auto ComponentString = FString::Join(ComponentHierarchy, TEXT("/"));
	return FString(FString::Printf(TEXT("%s@%s[%d]:/%s[%d]->%s"), *WorldName, *LevelName, LevelIndex, *ActorName,
	                               ActorIndex, *ComponentString));
}

int32 UWorldNavigator::GetIndexOfSameLevel(ULevel* Level)
{
	auto Levels = GetAllLevelsInWorld(Level->GetWorld());
	for (int32 i = 0; i < Levels.Num(); ++i)
	{
		if (Levels[i] == Level)
		{
			return i;
		}
	}
	return -1;
}

int32 UWorldNavigator::GetIndexOfSameActor(AActor* Actor)
{
	auto Levels = GetAllLevelsInWorld(Actor->GetWorld());
	for (int32 i = 0; i < Levels.Num(); ++i)
	{
		auto Actors = GetAllActorsInLevel(Levels[i]);
		for (int32 j = 0; j < Actors.Num(); ++j)
		{
			if (Actors[j] == Actor)
			{
				return i;
			}
		}
	}
	return -1;
}

int32 UWorldNavigator::GetIndexOfSameComponent(UActorComponent* Component)
{
	auto Components = GetAllComponentsInActor(Component->GetOwner());
	for (int32 i = 0; i < Components.Num(); ++i)
	{
		if (Components[i] == Component)
		{
			return i;
		}
	}
	return -1;
}

int32 UWorldNavigator::GetIndexOfSameComponent(USceneComponent* Component)
{
	auto Components = GetAllComponentsInActor(Component->GetOwner());
	for (int32 i = 0; i < Components.Num(); ++i)
	{
		if (Components[i] == Component)
		{
			return i;
		}
	}
	return -1;
}

TArray<ULevel*> UWorldNavigator::GetAllLevelsInWorld(UWorld* World)
{
	TArray<ULevel*> Levels;
	Levels.Add(World->PersistentLevel);
	Levels.Append(World->GetLevels()); // Streaming levels
	return Levels;
}

TArray<AActor*> UWorldNavigator::GetAllActorsInLevel(ULevel* Level)
{
	return Level->Actors;
}

TArray<UActorComponent*> UWorldNavigator::GetAllComponentsInActor(AActor* Actor)
{
	TArray<UActorComponent*> Results = Actor->GetComponents().Array();
	Results.Sort([](const UActorComponent& A, const UActorComponent& B)
	{
		return A.GetUniqueID() < B.GetUniqueID();
	});
	return Results;
}
