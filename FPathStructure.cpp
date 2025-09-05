#include "FPathStructure.h"

enum ECurrentFillingMode
{
	World,
	Level,
	LevelIdx,
	Actor,
	ActorIdx,
	Component,
	ComponentIndex
};

bool FPathStructure::TryParse(const FString& Path, FString& ErrorMessage, FPathStructure& Result)
{
	FString WorldName;
	FString LevelName;
	int32 LevelIndexNumber = 0;
	FString ActorName;
	int32 ActorIndexNumber = 0;
	TArray<FString> ComponentNames;
	TArray<int32> ComponentIndices;
	FString WorldSection, LevelSection, ActorSection, ComponentSection, LevelIndexSection, ActorIndexSection,
	        ComponentIndexSection;
	ECurrentFillingMode CurrentFillingMode = World;
	bool SkipNext = false;
	for (int32 i = 0; i < Path.Len(); ++i)
	{
		if (SkipNext)
		{
			SkipNext = false;
			continue;
		}
		if (Path[i] == '@')
		{
			WorldName = WorldSection;
			CurrentFillingMode = Level;
			continue;
		}
		if (i < Path.Len() - 1 and Path[i] == ':' and Path[i + 1] == '/')
		{
			LevelName = LevelSection;
			int32 LevelIndex = 0;
			bool ParseResult = TryParseInt(LevelIndexSection, LevelIndex);
			if (!ParseResult)
			{
				ErrorMessage = FString::Printf(TEXT("Failed to parse level index: %s"), *LevelIndexSection);
				return false;
			}
			LevelIndexNumber = LevelIndex;
			CurrentFillingMode = Actor;
			SkipNext = true;
			continue;
		}
		if (i < Path.Len() - 1 and Path[i] == '-' and Path[i + 1] == '>')
		{
			CurrentFillingMode = Component;
			SkipNext = true;
			continue;
		}
		if (Path[i] == '[' and CurrentFillingMode == Level)
		{
			CurrentFillingMode = LevelIdx;
			continue;
		}
		if (Path[i] == ']' and CurrentFillingMode == LevelIdx)
		{
			CurrentFillingMode = Level;
			continue;
		}
		if (Path[i] == '[' and CurrentFillingMode == Actor)
		{
			CurrentFillingMode = ActorIdx;
			continue;
		}
		if (Path[i] == ']' and CurrentFillingMode == ActorIdx)
		{
			ActorName = ActorSection;
			int32 ActorIndex = 0;
			bool ParseResult = TryParseInt(ActorIndexSection, ActorIndex);
			if (!ParseResult)
			{
				ErrorMessage = FString::Printf(TEXT("Failed to parse actor index: %s"), *ActorIndexSection);
				return false;
			}
			ActorIndexNumber = ActorIndex;
			CurrentFillingMode = Actor;
			continue;
		}
		if (Path[i] == '[' and CurrentFillingMode == Component)
		{
			CurrentFillingMode = ComponentIndex;
			continue;
		}
		if (Path[i] == ']' and CurrentFillingMode == ComponentIndex)
		{
			CurrentFillingMode = Component;
			continue;
		}
		if (Path[i] == '/' and CurrentFillingMode == Component)
		{
			ComponentNames.Add(ComponentSection);
			int32 ComponentIndex = 0;
			bool ParseResult = TryParseInt(ComponentIndexSection, ComponentIndex);
			if (!ParseResult)
			{
				ErrorMessage = FString::Printf(TEXT("Failed to parse component index: %s"), *ComponentIndexSection);
				return false;
			}
			ComponentIndices.Add(ComponentIndex);
			ComponentSection = "";
			ComponentIndexSection = "";
			continue;
		}
		if (CurrentFillingMode == World)
		{
			WorldSection += Path[i];
		}
		if (CurrentFillingMode == Level)
		{
			LevelSection += Path[i];
		}
		if (CurrentFillingMode == Actor)
		{
			ActorSection += Path[i];
		}
		if (CurrentFillingMode == Component)
		{
			ComponentSection += Path[i];
		}
		if (CurrentFillingMode == LevelIdx)
		{
			LevelIndexSection += Path[i];
		}
		if (CurrentFillingMode == ActorIdx)
		{
			ActorIndexSection += Path[i];
		}
		if (CurrentFillingMode == ComponentIndex)
		{
			ComponentIndexSection += Path[i];
		}
	}
	if (WorldName.IsEmpty() or LevelName.IsEmpty() or ActorName.IsEmpty())
	{
		ErrorMessage = TEXT("Missing required path components");
		return false;
	}
	if (ComponentNames.Num() != ComponentIndices.Num())
	{
		ErrorMessage = TEXT("Component names and indices must be the same length");
		return false;
	}
	Result.WorldName = WorldName;
	Result.LevelName = LevelName;
	Result.LevelIndex = LevelIndexNumber;
	Result.ActorName = ActorName;
	Result.ActorIndex = ActorIndexNumber;
	Result.ComponentNames = ComponentNames;
	Result.ComponentIndices = ComponentIndices;
	return true;
}

bool FPathStructure::TryParseInt(const FString& NumberString, int32& Result)
{
	Result = 0;
	for (int32 i = 0; i < NumberString.Len(); ++i)
	{
		if (NumberString[i] < '0' or NumberString[i] > '9')
		{
			return false;
		}
		Result *= 10;
		Result += NumberString[i] - '0';
	}
	return true;
}
