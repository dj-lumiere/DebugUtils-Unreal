# URepr Documentation

## Overview

URepr is a powerful template-based object representation system for Unreal Engine that provides Python-like `repr()` functionality for C++ types. It generates human-readable string representations of objects, making debugging and logging much more informative and efficient.

## Key Features

- **Type-Safe Templates**: Automatic type detection and formatting
- **Clear Type Suffixes**: Clear indication of numeric types (e.g., `42_i32`, `3.14_f32`)
- **Container Support**: Built-in support for TArray, TSet, TMap, and other Unreal containers
- **Nested Object Support**: Handles complex nested data structures
- **Logging Macros**: Convenient macros for quick debug logging
- **Zero Runtime Cost**: All operations are compile-time template instantiations

## API Reference

### Basic Types

#### Numeric Types

URepr provides specialized representations for all numeric types with clear suffixes:

```cpp
// Integer types
int8 smallInt = 127;
Repr(smallInt);          // "127_i8"

int32 regularInt = 42;
Repr(regularInt);        // "42_i32"

int64 bigInt = 9223372036854775807LL;
Repr(bigInt);            // "9223372036854775807_i64"

uint32 unsignedInt = 3000000000U;
Repr(unsignedInt);       // "3000000000_u32"

// Floating point types
float singlePrecision = 3.14159f;
Repr(singlePrecision);   // "3.141590_f32"

double doublePrecision = 2.71828;
Repr(doublePrecision);   // "2.718280_f64"
```

#### Boolean and String Types

```cpp
// Boolean
bool isActive = true;
Repr(isActive);          // "true"

bool isDisabled = false;
Repr(isDisabled);        // "false"

// String types
FString playerName = TEXT("Alice");
Repr(playerName);        // "\"Alice\""

FName objectName = FName(TEXT("PlayerActor"));
Repr(objectName);        // "PlayerActor"

FText displayText = FText::FromString(TEXT("Health: 100"));
Repr(displayText);       // "Health: 100"
```

### Container Types

#### Arrays (TArray)

```cpp
TArray<int32> numbers = {1, 2, 3, 4, 5};
Repr(numbers);
// Output: "[1_i32, 2_i32, 3_i32, 4_i32, 5_i32]"

TArray<FString> names = {TEXT("Alice"), TEXT("Bob"), TEXT("Charlie")};
Repr(names);
// Output: "[\"Alice\", \"Bob\", \"Charlie\"]"

// Nested arrays
TArray<TArray<int32>> matrix = {{1, 2}, {3, 4}, {5, 6}};
Repr(matrix);
// Output: "[[1_i32, 2_i32], [3_i32, 4_i32], [5_i32, 6_i32]]"
```

#### Sets (TSet)

```cpp
TSet<FString> uniqueNames = {TEXT("Alice"), TEXT("Bob"), TEXT("Alice")}; // Duplicate removed
Repr(uniqueNames);
// Output: "{\"Alice\", \"Bob\"}" (order may vary)

TSet<int32> uniqueNumbers = {1, 2, 3, 2, 1};
Repr(uniqueNumbers);
// Output: "{1_i32, 2_i32, 3_i32}" (order may vary)
```

#### Maps (TMap)

```cpp
TMap<FString, int32> playerScores;
playerScores.Add(TEXT("Alice"), 1500);
playerScores.Add(TEXT("Bob"), 1200);
playerScores.Add(TEXT("Charlie"), 1800);

Repr(playerScores);
// Output: "{\"Alice\": 1500_i32, \"Bob\": 1200_i32, \"Charlie\": 1800_i32}"

// Complex value types
TMap<FString, TArray<int32>> playerInventories;
playerInventories.Add(TEXT("Alice"), {1, 2, 3});
playerInventories.Add(TEXT("Bob"), {4, 5});

Repr(playerInventories);
// Output: "{\"Alice\": [1_i32, 2_i32, 3_i32], \"Bob\": [4_i32, 5_i32]}"
```

### Advanced Types

#### Optional Types (TOptional)

```cpp
TOptional<int32> maybeValue;
Repr(maybeValue);        // "None"

TOptional<int32> hasValue = 42;
Repr(hasValue);          // "Some(42_i32)"

TOptional<FString> maybeName = FString(TEXT("Player"));
Repr(maybeName);         // "Some(\"Player\")"
```

#### Tuples (TTuple)

```cpp
TTuple<int32, FString, bool> playerData = MakeTuple(100, TEXT("Alice"), true);
Repr(playerData);
// Output: "(100_i32, \"Alice\", true)"

TTuple<TArray<int32>, TMap<FString, float>> complexData = 
    MakeTuple(TArray<int32>{1, 2, 3}, TMap<FString, float>{{TEXT("health"), 100.0f}});
Repr(complexData);
// Output: "([1_i32, 2_i32, 3_i32], {\"health\": 100.000000_f32})"
```

#### Pairs (TPair)

```cpp
TPair<FString, int32> keyValue = MakeTuple(TEXT("score"), 1500);
Repr(keyValue);
// Output: "(\"score\", 1500_i32)"

TPair<int32, TArray<FString>> indexedNames = MakeTuple(0, TArray<FString>{TEXT("Alice"), TEXT("Bob")});
Repr(indexedNames);
// Output: "(0_i32, [\"Alice\", \"Bob\"])"
```

## Logging Macros

URepr provides convenient macros for quick debug logging:

### UE_REPR(variable)

Logs a variable with its name and representation at Display level:

```cpp
int32 playerHealth = 100;
UE_REPR(playerHealth);
// Console output: LogTemp: Display: playerHealth = 100_i32

TArray<FString> inventory = {TEXT("Sword"), TEXT("Potion")};
UE_REPR(inventory);
// Console output: LogTemp: Display: inventory = ["Sword", "Potion"]
```

### UE_REPR_WARNING(variable)

Logs a variable with its name and representation at Warning level:

```cpp
float suspiciousValue = -1.0f;
UE_REPR_WARNING(suspiciousValue);
// Console output: LogTemp: Warning: suspiciousValue = -1.000000_f32
```

### UE_REPR_ERROR(variable)

Logs a variable with its name and representation at Error level:

```cpp
TOptional<AActor*> missingActor;
UE_REPR_ERROR(missingActor);
// Console output: LogTemp: Error: missingActor = None
```

## Usage Scenarios

### 1. Debug Logging Complex Data Structures

```cpp
void ProcessGameData()
{
    TMap<FString, TArray<int32>> playerStats;
    playerStats.Add(TEXT("Alice"), {100, 50, 25}); // Health, Mana, Stamina
    playerStats.Add(TEXT("Bob"), {80, 75, 30});
    
    UE_REPR(playerStats);
    // Output: playerStats = {"Alice": [100_i32, 50_i32, 25_i32], "Bob": [80_i32, 75_i32, 30_i32]}
}
```

### 2. Function Parameter Validation

```cpp
void ValidateInputData(const TArray<float>& values, const TOptional<int32>& maxCount)
{
    UE_LOG(LogTemp, Log, TEXT("Validating values: %s"), *Repr(values));
    UE_LOG(LogTemp, Log, TEXT("Max count constraint: %s"), *Repr(maxCount));
    
    if (values.Num() == 0)
    {
        UE_REPR_WARNING(values);
        UE_LOG(LogTemp, Warning, TEXT("Empty values array provided"));
    }
}
```

### 3. State Debugging

```cpp
USTRUCT()
struct FGameState
{
    GENERATED_BODY()
    
    TMap<FString, int32> PlayerScores;
    TArray<FString> ActivePlayers;
    TOptional<FString> CurrentLevel;
    bool bGameActive;
};

void LogGameState(const FGameState& state)
{
    // Note: For USTRUCTs, you'd need custom Repr specialization or use individual fields
    UE_LOG(LogTemp, Log, TEXT("=== Game State Debug ==="));
    UE_REPR(state.PlayerScores);
    UE_REPR(state.ActivePlayers);
    UE_REPR(state.CurrentLevel);
    UE_REPR(state.bGameActive);
}
```

### 4. Algorithm Debugging

```cpp
TArray<int32> SortAndFilter(const TArray<int32>& input, int32 threshold)
{
    UE_LOG(LogTemp, Log, TEXT("Input data: %s"), *Repr(input));
    UE_LOG(LogTemp, Log, TEXT("Threshold: %s"), *Repr(threshold));
    
    TArray<int32> filtered = input.FilterByPredicate([threshold](int32 value) {
        return value > threshold;
    });
    
    UE_LOG(LogTemp, Log, TEXT("After filtering: %s"), *Repr(filtered));
    
    filtered.Sort();
    UE_LOG(LogTemp, Log, TEXT("After sorting: %s"), *Repr(filtered));
    
    return filtered;
}
```

### 5. Network Data Debugging

```cpp
USTRUCT()
struct FNetworkMessage
{
    GENERATED_BODY()
    
    int32 MessageId;
    FString Payload;
    TArray<uint8> BinaryData;
};

void ProcessNetworkMessage(const FNetworkMessage& message)
{
    // Log individual components since USTRUCTs need custom specialization
    UE_LOG(LogTemp, Log, TEXT("Processing network message:"));
    UE_REPR(message.MessageId);
    UE_REPR(message.Payload);
    UE_REPR(message.BinaryData);
}
```

### 6. Configuration Debugging

```cpp
void LoadConfiguration()
{
    TMap<FString, FString> configValues;
    configValues.Add(TEXT("MaxPlayers"), TEXT("4"));
    configValues.Add(TEXT("TimeLimit"), TEXT("300"));
    configValues.Add(TEXT("MapRotation"), TEXT("true"));
    
    UE_LOG(LogTemp, Log, TEXT("Loaded configuration: %s"), *Repr(configValues));
}
```

## Performance Considerations

- **Compile-Time**: All template instantiations happen at compile time
- **String Creation**: Repr functions create FString objects, so avoid in tight loops
- **Memory Usage**: Temporary strings are created during representation generation
- **Debug vs Shipping**: Consider wrapping UE_REPR macros with debug conditionals for shipping builds

```cpp
#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
    UE_REPR(complexDataStructure);
#endif
```

## Best Practices

1. **Use for Development**: Primarily intended for debugging and development builds
2. **Meaningful Variable Names**: The UE_REPR macro shows variable names, so use descriptive names
3. **Custom Types**: Create Repr specializations for frequently debugged custom types
4. **Conditional Compilation**: Wrap expensive Repr calls in debug conditionals
5. **Logging Levels**: Use appropriate logging levels (Display, Warning, Error) based on context
6. **Performance**: Avoid using Repr in performance-critical code paths

## Integration with Other Debug Utils

URepr works excellently with UCallStack for comprehensive debugging:

```cpp
void DebugFunction(const TArray<int32>& data)
{
    FString caller = UCallStack::GetCallerName();
    UE_LOG(LogTemp, Log, TEXT("[%s] Processing data: %s"), *caller, *Repr(data));
    
    if (data.Num() == 0)
    {
        FString callerInfo = UCallStack::GetCallerInfo();
        UE_LOG(LogTemp, Warning, TEXT("[%s] Empty data array received!"), *callerInfo);
    }
}
```

Combined with UWorldNavigator for object debugging:

```cpp
void DebugActorState(AActor* actor)
{
    if (actor)
    {
        FString actorPath = UWorldNavigator::GetWorldPath(actor);
        FString caller = UCallStack::GetCallerName();
        
        UE_LOG(LogTemp, Log, TEXT("[%s] Debugging actor at path: %s"), *caller, *actorPath);
        
        // Debug actor's transform
        FVector location = actor->GetActorLocation();
        FRotator rotation = actor->GetActorRotation();
        
        UE_LOG(LogTemp, Log, TEXT("  Location: %s"), *Repr(location)); // Would need FVector specialization
        UE_LOG(LogTemp, Log, TEXT("  Rotation: %s"), *Repr(rotation)); // Would need FRotator specialization
    }
}
```

URepr transforms debugging from cryptic object addresses and type names into clear, readable representations of your data, making it an essential tool for Unreal Engine development.