#pragma once
#include "UCallStack.generated.h"

UCLASS(BlueprintType, Blueprintable)
class DEBUGUTILS_API UCallStack : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Retrieves information about the calling function using stack trace inspection.
	 * Useful for debugging and logging to identify which function initiated a call.
	 * @return A string representing the calling function, or "[unknown]" if unavailable
	 */
	UFUNCTION(BlueprintCallable, Category = "Call Stack")
	static FString GetCallerName();

	/**
	 * Retrieves detailed stack trace information including file path and line number.
	 * Provides more comprehensive debug info than GetCallerName for deeper analysis.
	 * @return A formatted string with file, line, and function details, or "[unknown]" if unavailable
	 */
	UFUNCTION(BlueprintCallable, Category = "Call Stack")
	static FString GetCallerInfo();
};
