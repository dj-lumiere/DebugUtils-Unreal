#pragma once

struct FPathStructure
{
	FString WorldName;
	FString LevelName;
	int32 LevelIndex;
	FString ActorName;
	int32 ActorIndex;
	TArray<FString> ComponentNames;
	TArray<int32> ComponentIndices;

	static bool TryParse(const FString& Path, FString& ErrorMessage, FPathStructure& Result);

private:
	static bool TryParseInt(const FString& NumberString, int32& Result);
};
