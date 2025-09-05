# UCallStack Documentation

## Overview

UCallStack is a lightweight Blueprint Function Library that provides stack trace inspection capabilities for Unreal Engine projects. It allows you to identify calling functions and retrieve detailed debug information, making it invaluable for debugging, logging, and runtime analysis.

## Key Features

- **Caller Identification**: Identify which function called the current function
- **Stack Trace Information**: Get detailed file, line, and function information
- **Blueprint Integration**: Full Blueprint support for runtime debugging
- **Lightweight**: Minimal performance overhead with focused functionality
- **Cross-Platform**: Uses Unreal's platform abstraction for stack walking

## API Reference

### `GetCallerName()`

Retrieves the name of the function that called the current function.

**Parameters:** None

**Returns:** 
- `FString`: Name of the calling function, or "[unknown]" if unavailable

**C++ Example:**
```cpp
void MyFunction()
{
    FString CallerName = UCallStack::GetCallerName();
    UE_LOG(LogTemp, Log, TEXT("Called by: %s"), *CallerName);
}

void SomeOtherFunction()
{
    MyFunction(); // This will log "Called by: SomeOtherFunction"
}
```

**Blueprint Usage:**
- Node: "Get Caller Name"
- Category: Call Stack
- Output: String (Caller Name)

### `GetCallerInfo()`

Retrieves comprehensive debug information about the calling function, including file path, function name, and line number.

**Parameters:** None

**Returns:**
- `FString`: Formatted string with file path, function name, and line number, or "[unknown]" if unavailable

**Format:** `FilePath@FunctionName:LineNumber`

**C++ Example:**
```cpp
void DetailedLogging()
{
    FString CallerInfo = UCallStack::GetCallerInfo();
    UE_LOG(LogTemp, Log, TEXT("Detailed caller info: %s"), *CallerInfo);
    // Output: "MyProject/Source/MyClass.cpp@SomeFunction:123"
}

void CallingFunction()
{
    DetailedLogging(); // Line 45 in MyClass.cpp
    // This will log something like: "MyProject/Source/MyClass.cpp@CallingFunction:45"
}
```

**Blueprint Usage:**
- Node: "Get Caller Info" 
- Category: Call Stack
- Output: String (Detailed Info)

## Usage Scenarios

### 1. Debug Logging and Tracing

Track which functions are calling specific utility functions:

```cpp
void UtilityFunction(int32 Value)
{
    FString Caller = UCallStack::GetCallerName();
    UE_LOG(LogTemp, Warning, TEXT("UtilityFunction called with value %d by %s"), Value, *Caller);
    
    // Your utility logic here
}

void GameplayFunction()
{
    UtilityFunction(42); // Will log: "UtilityFunction called with value 42 by GameplayFunction"
}
```

### 2. Error Reporting and Diagnostics

Provide better error context by showing where errors originated:

```cpp
void ReportError(const FString& ErrorMessage)
{
    FString CallerInfo = UCallStack::GetCallerInfo();
    FString DetailedError = FString::Printf(
        TEXT("ERROR: %s\nCalled from: %s"), 
        *ErrorMessage, 
        *CallerInfo
    );
    
    UE_LOG(LogTemp, Error, TEXT("%s"), *DetailedError);
    
    // Could also display on screen or send to crash reporting system
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, DetailedError);
    }
}

void SomeGameplayCode()
{
    if (SomeCondition)
    {
        ReportError(TEXT("Invalid game state detected"));
        // Will provide full context: file, function, and line number
    }
}
```

### 3. Function Call Analytics

Track which parts of your code are calling expensive operations:

```cpp
void ExpensiveOperation()
{
    static TMap<FString, int32> CallCounts;
    
    FString Caller = UCallStack::GetCallerName();
    CallCounts.FindOrAdd(Caller)++;
    
    UE_LOG(LogTemp, Log, TEXT("ExpensiveOperation called by %s (count: %d)"), 
           *Caller, CallCounts[Caller]);
    
    // Perform expensive work...
}
```

### 4. Conditional Behavior Based on Caller

Implement different behavior based on the calling context:

```cpp
float GetDamageMultiplier()
{
    FString Caller = UCallStack::GetCallerName();
    
    if (Caller.Contains(TEXT("CriticalHit")))
    {
        return 2.0f; // Double damage for critical hits
    }
    else if (Caller.Contains(TEXT("Spell")))
    {
        return 1.5f; // Bonus damage for spells
    }
    
    return 1.0f; // Normal damage
}
```

### 5. Blueprint Debugging

Use in Blueprint to track execution flow:

**Blueprint Example Workflow:**
1. Create a custom Blueprint function called "Debug Log With Caller"
2. Add "Get Caller Name" node
3. Combine with your log message
4. Print to screen or log

This allows Blueprint developers to easily see which Blueprint nodes or functions are triggering specific behavior.

### 6. Validation and Assertions

Create smart assertions that provide calling context:

```cpp
void ValidateParameter(int32 Value, const FString& ParameterName)
{
    if (Value < 0)
    {
        FString CallerInfo = UCallStack::GetCallerInfo();
        FString ErrorMsg = FString::Printf(
            TEXT("Invalid parameter '%s' (value: %d) passed from: %s"),
            *ParameterName, Value, *CallerInfo
        );
        
        UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMsg);
        
        // Could also trigger breakpoint in debug builds
        checkf(false, TEXT("%s"), *ErrorMsg);
    }
}

void SomeFunction(int32 Health)
{
    ValidateParameter(Health, TEXT("Health"));
    // If Health is negative, you'll get exact file and line information
}
```

### 7. Profiling and Performance Analysis

Track performance bottlenecks by caller:

```cpp
void ProfiledFunction()
{
    static TMap<FString, double> ExecutionTimes;
    static TMap<FString, int32> CallCounts;
    
    FString Caller = UCallStack::GetCallerName();
    double StartTime = FPlatformTime::Seconds();
    
    // Do work here...
    
    double EndTime = FPlatformTime::Seconds();
    double ExecutionTime = EndTime - StartTime;
    
    ExecutionTimes.FindOrAdd(Caller) += ExecutionTime;
    CallCounts.FindOrAdd(Caller)++;
    
    double AverageTime = ExecutionTimes[Caller] / CallCounts[Caller];
    
    UE_LOG(LogTemp, Log, TEXT("ProfiledFunction called by %s - Execution: %.4fms, Average: %.4fms"), 
           *Caller, ExecutionTime * 1000.0, AverageTime * 1000.0);
}
```

## Technical Details

### Stack Walking Implementation

UCallStack uses Unreal Engine's `FPlatformStackWalk::GetStack()` function with these parameters:
- **IgnoreCount**: Set to 1 to skip the UCallStack function itself
- **MaxDepth**: Set to 1 to get only the immediate caller
- **Context**: nullptr (not used)

### Function Name Processing

The implementation automatically removes trailing parentheses from function names using `LeftChop(2)` for cleaner output.

### Platform Support

UCallStack works on all platforms supported by Unreal Engine's stack walking functionality, including:
- Windows
- Mac
- Linux
- Console platforms (where supported by UE)

### Performance Considerations

- **Debug vs. Shipping**: Stack walking may have limited functionality in shipping builds
- **Call Frequency**: While lightweight, avoid calling these functions in tight loops
- **Memory**: Each call allocates temporary arrays for stack information
- **Symbol Information**: Full symbols may require debug information to be available

## Error Handling

Both functions return `"[unknown]"` when stack information is unavailable. This can happen when:
- Running in shipping builds without debug symbols
- On platforms with limited stack walking support
- When the stack trace fails for any reason

Always handle the unknown case:

```cpp
FString CallerName = UCallStack::GetCallerName();
if (CallerName == TEXT("[unknown]"))
{
    UE_LOG(LogTemp, Warning, TEXT("Could not determine caller - running in shipping build?"));
}
else
{
    UE_LOG(LogTemp, Log, TEXT("Called by: %s"), *CallerName);
}
```

## Best Practices

1. **Use for Debugging**: Primarily intended for debugging and development builds
2. **Cache Results**: If you need caller info multiple times in a function, cache the result
3. **Handle Unknown**: Always handle the "[unknown]" return value gracefully
4. **Avoid Tight Loops**: Don't call these functions in performance-critical code paths
5. **Meaningful Logging**: Combine with other contextual information for better debugging
6. **Blueprint Integration**: Great for Blueprint debugging workflows
7. **Error Context**: Excellent for providing context in error messages and assertions

## Integration Examples

### Custom Logging Macro

```cpp
#define LOG_WITH_CALLER(LogCategory, Verbosity, Format, ...) \
{ \
    FString CallerInfo = UCallStack::GetCallerInfo(); \
    UE_LOG(LogCategory, Verbosity, TEXT("[%s] " Format), *CallerInfo, ##__VA_ARGS__); \
}

// Usage:
LOG_WITH_CALLER(LogTemp, Warning, TEXT("Something went wrong with value %d"), SomeValue);
```

### Blueprint Library Extension

```cpp
UCLASS()
class MYGAME_API UMyDebugLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "My Debug")
    static void LogMessageWithCaller(const FString& Message)
    {
        FString Caller = UCallStack::GetCallerName();
        UE_LOG(LogTemp, Log, TEXT("[%s] %s"), *Caller, *Message);
        
        if (GEngine)
        {
            FString DisplayText = FString::Printf(TEXT("[%s] %s"), *Caller, *Message);
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, DisplayText);
        }
    }
};
```

UCallStack provides essential debugging capabilities that help developers understand code execution flow, track down issues, and create better logging systems. Its simplicity and Blueprint integration make it accessible to both C++ and Blueprint developers.