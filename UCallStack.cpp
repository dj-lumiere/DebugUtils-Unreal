#include "UCallStack.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
#include "HAL/PlatformStackWalk.h"
#include "Misc/OutputDeviceRedirector.h"

FString UCallStack::GetCallerName()
{
	// I set the IgnoreCount to 1 to ignore this function.
	TArray<FProgramCounterSymbolInfo> CurrentInfo = FPlatformStackWalk::GetStack(1, 1, nullptr);
	if (CurrentInfo.Num() == 0)
	{
		return TEXT("[unknown]");
	}
	const FString CallerInfo = CurrentInfo[0].FunctionName;
	// Trim the trailing parenthesis pair
	return CallerInfo.LeftChop(2);
}

FString UCallStack::GetCallerInfo()
{
	// I set the IgnoreCount to 1 to ignore this function.
	TArray<FProgramCounterSymbolInfo> CurrentInfo = FPlatformStackWalk::GetStack(1, 1, nullptr);
	if (CurrentInfo.Num() == 0)
	{
		return TEXT("[unknown]");
	}
	const FString CallerFile = CurrentInfo[0].Filename;
	const FString CallerInfo = CurrentInfo[0].FunctionName;
	const FString CallerNameSanitized = CallerInfo.LeftChop(2);
	const FString CallerLine = FString::Printf(TEXT("%d"), CurrentInfo[0].LineNumber);
	// Trim the trailing parenthesis pair
	return FString::Printf(TEXT("%s@%s:%s"), *CallerFile, *CallerNameSanitized, *CallerLine);
}
