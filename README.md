# DebugUtils for Unreal Engine

[![Unreal 5+](https://img.shields.io/badge/Unreal-5.0%2B-blue.svg)](https://unrealengine.com/)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Version](https://img.shields.io/badge/Version-1.0.0-orange.svg)]()

A comprehensive collection of debugging utilities for Unreal Engine developers that provides world navigation and call stack analysis tools for enhanced debugging and development workflow.

## 📋 Table of Contents

- [Core Features](#core-features)
- [Installation](#installation)
- [Quick Start](#quick-start)
- [Features](#features)
- [Configuration Options](#configuration-options)
- [Real-World Unreal Use Cases](#real-world-unreal-use-cases)
- [Performance Considerations](#performance-considerations)
- [Troubleshooting](#troubleshooting)- [Unreal Version Compatibility](#unreal-version-compatibility)
- [Dependencies](#dependencies)
- [Roadmap](#roadmap)
- [Contributing](#contributing)
- [License](#license)

## Core Features

## 🔍 Object Representation ([`URepr` Documentation](URepr_Documentation.md))

Stop getting useless `ToString()` output. See actual object contents with Python-like `repr()` functionality for Unreal Engine types. Get clear, readable representations of your data with Rust-style type suffixes.

## 📍 Call Stack Tracking ([`UCallStack` Documentation](UCallStack_Documentation.md))

Know exactly where your code is executing and which functions are calling what in your Unreal Engine project. Track function calls with detailed file and line information.

## 🎮 World Navigation ([`UWorldNavigator` Documentation](UWorldNavigator_Documentation.md))

Navigate Unreal Engine world hierarchies with structured path strings. Find actors, components, and scene components using readable path formats instead of manual object traversal.

## Installation

### Manual Installation

1. Download or clone this repository
2. Copy the header and source files (`UWorldNavigator.h/.cpp`, `UCallStack.h/.cpp`, `FPathStructure.h/.cpp`) to your Unreal Engine project's Source folder
3. Add the module dependency to your project's Build.cs file if needed
4. Build your project

## Quick Start
### Simple Examples

```cpp
#include "URepr.h"

// See what's actually in your objects
TArray<int32> playerStats = {100, 50, 25};
UE_REPR(playerStats);
// Output: playerStats = [100_i32, 50_i32, 25_i32]

TMap<FString, float> itemPrices = {{TEXT("Sword"), 150.0f}, {TEXT("Potion"), 25.5f}};
UE_LOG(LogTemp, Log, TEXT("Item prices: %s"), *Repr(itemPrices));
// Output: Item prices: {"Sword": 150.000000_f32, "Potion": 25.500000_f32}

// Track where your code is executing
void OnPlayerDamage(int32 Damage) 
{
    FString CallerName = UCallStack::GetCallerName();
    UE_LOG(LogTemp, Log, TEXT("[%s] Player took %d damage"), *CallerName, Damage);
    // Output: [AEnemy::Attack] Player took 25 damage
}

// Navigate Unreal worlds easily
FString ActorPath = TEXT("MyWorld@PersistentLevel[0]:/PlayerCharacter[0]");
AActor* FoundActor = UWorldNavigator::FindUActorByPath(ActorPath);
if (FoundActor)
{
    UE_LOG(LogTemp, Log, TEXT("Found actor: %s"), *FoundActor->GetName());
}
```

### Complete Example

```cpp
// GameDebugActor.h
#include "URepr.h"
#include "UCallStack.h"
#include "UWorldNavigator.h"

UCLASS()
class DEBUGUTILSTEST_API AGameDebugActor : public AActor
{
    GENERATED_BODY()

public:
    AGameDebugActor();

protected:
    virtual void BeginPlay() override;
    void InitializeGame();
    void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
    
    UPROPERTY()
    TArray<int32> PlayerScores;
    
    UPROPERTY()
    TMap<FString, float> ItemValues;
};

// GameDebugActor.cpp
void AGameDebugActor::BeginPlay()
{
    Super::BeginPlay();
    
    // 🔍 See your data clearly
    PlayerScores = {1500, 1200, 1800, 900};
    ItemValues = {{TEXT("Sword"), 150.0f}, {TEXT("Shield"), 75.5f}, {TEXT("Potion"), 25.0f}};
    
    UE_REPR(PlayerScores);  // PlayerScores = [1500_i32, 1200_i32, 1800_i32, 900_i32]
    UE_REPR(ItemValues);    // ItemValues = {"Sword": 150.000000_f32, "Shield": 75.500000_f32, "Potion": 25.000000_f32}
    
    InitializeGame();
}

void AGameDebugActor::InitializeGame()
{
    FString CallerName = UCallStack::GetCallerName();
    UE_LOG(LogTemp, Log, TEXT("[%s] Initializing game systems..."), *CallerName);
    // Output: [AGameDebugActor::BeginPlay] Initializing game systems...
    
    // 🎮 Find actors easily
    FString ActorPath = TEXT("MyWorld@PersistentLevel[0]:/StartButton[0]");
    AActor* StartButton = UWorldNavigator::FindUActorByPath(ActorPath);
    if (StartButton)
    {
        FString ButtonPath = UWorldNavigator::GetWorldPath(StartButton);
        UE_LOG(LogTemp, Log, TEXT("Found button at: %s"), *ButtonPath);
    }
}

void AGameDebugActor::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    FString CallerInfo = UCallStack::GetCallerInfo();
    FString OtherActorPath = UWorldNavigator::GetWorldPath(OtherActor);
    
    UE_LOG(LogTemp, Log, TEXT("[%s] Collision with actor at: %s"), *CallerInfo, *OtherActorPath);
    UE_LOG(LogTemp, Log, TEXT("Impact force: %s"), *Repr(NormalImpulse)); // Would need FVector specialization
    
    // Show hit location with clear formatting
    UE_LOG(LogTemp, Log, TEXT("Hit location: %s"), *Hit.Location.ToString());
}

## Features

### 🔍 Object Representation (`URepr`)

See what's actually inside your objects with Python-like repr() functionality.

#### Basic Types and Containers

```cpp
// Numeric types with clear type information
int32 health = 100;
UE_REPR(health);  // health = 100_i32

float damage = 25.5f;
UE_REPR(damage);  // damage = 25.500000_f32

// Container types
TArray<FString> inventory = {TEXT("Sword"), TEXT("Potion"), TEXT("Key")};
UE_REPR(inventory);  // inventory = ["Sword", "Potion", "Key"]

TMap<FString, int32> stats = {{TEXT("Strength"), 15}, {TEXT("Dexterity"), 12}};
UE_REPR(stats);  // stats = {"Strength": 15_i32, "Dexterity": 12_i32}
```

#### Debugging Complex Data

```cpp
void ProcessPlayerData(const TMap<FString, TArray<int32>>& playerStats)
{
    UE_LOG(LogTemp, Log, TEXT("Processing: %s"), *Repr(playerStats));
    // Shows complete nested structure clearly
}
```

### 📍 Call Stack Tracking (`UCallStack`)

Track function execution and get detailed debug information about calling functions.

```cpp
void UtilityFunction(int32 Value)
{
    FString CallerName = UCallStack::GetCallerName();
    UE_LOG(LogTemp, Log, TEXT("[%s] UtilityFunction called with: %s"), *CallerName, *Repr(Value));
    // Output: [GameplayFunction] UtilityFunction called with: 42_i32
}
```

### 🎮 World Navigation (`UWorldNavigator`)

Navigate Unreal Engine world hierarchies with structured path strings.

#### Finding Objects by Path

```cpp
// Find actors by path
FString ActorPath = TEXT("MyWorld@PersistentLevel[0]:/PlayerCharacter[0]");
AActor* FoundActor = UWorldNavigator::FindUActorByPath(ActorPath);
if (FoundActor)
{
    UE_LOG(LogTemp, Log, TEXT("Found actor: %s"), *FoundActor->GetName());
}

// Find components by path
FString ComponentPath = TEXT("MyWorld@PersistentLevel[0]:/PlayerCharacter[0]->HealthComponent[0]");
UActorComponent* HealthComponent = UWorldNavigator::FindActorComponentByPath(ComponentPath);

// Find scene components with hierarchy
FString SceneComponentPath = TEXT("MyWorld@PersistentLevel[0]:/PlayerCharacter[0]->RootComponent[0]/MeshComponent[0]");
USceneComponent* MeshComponent = UWorldNavigator::FindSceneComponentByPath(SceneComponentPath);
```

#### Generating World Paths

```cpp
// Get path for actors
AActor* MyActor = GetWorld()->SpawnActor<AActor>();
FString ActorPath = UWorldNavigator::GetWorldPath(MyActor);
UE_LOG(LogTemp, Log, TEXT("Actor path: %s"), *ActorPath);
// Output: "MyWorld@PersistentLevel[0]:/Actor[0]"

// Get path for components
UActorComponent* MyComponent = MyActor->GetComponentByClass<UStaticMeshComponent>();
FString ComponentPath = UWorldNavigator::GetWorldPath(MyComponent);
UE_LOG(LogTemp, Log, TEXT("Component path: %s"), *ComponentPath);
// Output: "MyWorld@PersistentLevel[0]:/MyActor[0]->StaticMeshComponent[0]"
```

**Use cases:**

- **Data Visualization** - See exactly what's in your containers and objects
- **Function debugging** - Track C++ and Blueprint function execution with clear parameter display
- **Event debugging** - Know exactly which function triggered an event
- **Algorithm debugging** - Monitor data transformations with before/after snapshots
- **Physics debugging** - Trace collision and overlap events with detailed object information
- **Network debugging** - Log serialized data structures clearly
- **State machine debugging** - Monitor state transitions with readable state data

### Path Structure

UWorldNavigator uses a hierarchical path format:

```
WorldName@LevelName[LevelIndex]:/ActorName[ActorIndex]->ComponentName[ComponentIndex]
```

**Components:**
- `WorldName`: Name of the UWorld object
- `LevelName[LevelIndex]`: Level name with index for duplicate names
- `ActorName[ActorIndex]`: Actor name with index for duplicate names  
- `ComponentName[ComponentIndex]`: Component name with index for duplicate names

**Examples:**
- Actor path: `MyWorld@PersistentLevel[0]:/PlayerPawn[0]`
- Component path: `MyWorld@PersistentLevel[0]:/PlayerPawn[0]->MeshComponent[0]`
- Scene component hierarchy: `MyWorld@PersistentLevel[0]:/PlayerPawn[0]->RootComponent[0]/MeshComponent[1]`

## Blueprint Integration

Both UCallStack and UWorldNavigator are fully Blueprint-compatible:

### Blueprint Nodes

**UCallStack:**
- **Get Caller Name** - Returns the name of the calling function
- **Get Caller Info** - Returns detailed caller information with file and line

**UWorldNavigator:**
- **Find UActor By Path** - Finds an actor using a world path string
- **Find Actor Component By Path** - Finds a component using a world path string
- **Find Scene Component By Path** - Finds a scene component using a world path string
- **Get World Path** (multiple variants) - Generates world path strings for actors and components

**URepr:**
- URepr is C++-only and provides template functions and macros
- Use the logging macros (UE_REPR, UE_REPR_WARNING, UE_REPR_ERROR) in C++ code
- Results appear in the Output Log and can be viewed in Blueprint debugging

### Blueprint Integration Examples

```cpp
// In Blueprint, you can:
// 1. Use "Get Caller Name" node to track which Blueprint function called this one
// 2. Use "Find UActor By Path" with a string input to find actors dynamically
// 3. Use "Get World Path" to serialize actor references as strings
// 4. In C++ components, use UE_REPR to log data that's visible in Blueprint debugging
```

## Real-World Unreal Engine Use Cases

### Game Development Debugging

Debug game mechanics and state instantly:

```cpp
UCLASS()
class MYGAME_API AGameManager : public AActor
{
    GENERATED_BODY()
    
public:
    UPROPERTY()
    TArray<APlayerController*> ActivePlayers;
    
    UPROPERTY()
    TMap<FString, int32> LevelScores;
    
    virtual void Tick(float DeltaTime) override
    {
        if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::F1))
        {
            FString Caller = UCallStack::GetCallerInfo();
            UE_LOG(LogTemp, Log, TEXT("[%s] Active players: %s"), *Caller, *Repr(ActivePlayers));
            UE_LOG(LogTemp, Log, TEXT("[%s] Level scores: %s"), *Caller, *Repr(LevelScores));
        }
    }
    
    UFUNCTION()
    void ProcessGameTick()
    {
        FString Caller = UCallStack::GetCallerName();
        UE_LOG(LogTemp, Log, TEXT("[%s] Processing game tick"), *Caller);
        UE_REPR(LevelScores);  // Full structure logging
    }
};
```

### Component Interaction Debugging

```cpp
UCLASS()
class MYGAME_API UCollisionHandler : public UActorComponent
{
    GENERATED_BODY()
    
public:
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
    {
        FString CallerInfo = UCallStack::GetCallerInfo();
        FString OtherActorPath = UWorldNavigator::GetWorldPath(OtherActor);
        
        UE_LOG(LogTemp, Log, TEXT("[%s] Collision with actor at: %s"), *CallerInfo, *OtherActorPath);
        UE_REPR(NormalImpulse);  // Shows impact force clearly
        
        TArray<FVector> ContactPoints;
        for (int32 i = 0; i < Hit.GetActor()->GetRootComponent()->GetNumChildrenComponents(); ++i)
        {
            ContactPoints.Add(Hit.Location);
        }
        UE_REPR(ContactPoints);
    }
};
```

### Unreal Engine Testing

```cpp
// In a test class or automation test
void TestPlayerMovement()
{
    APlayerPawn* Player = CreateTestPlayer();
    FVector InitialPos = Player->GetActorLocation();
    FVector InputVector = FVector(1.0f, 0.0f, 0.0f);
    
    Player->ProcessInput(InputVector);
    FVector FinalPos = Player->GetActorLocation();
    
    // Amazing error messages when tests fail
    FString Caller = UCallStack::GetCallerInfo();
    UE_LOG(LogTemp, Log, TEXT("[%s] Initial: %s"), *Caller, *Repr(InitialPos));
    UE_LOG(LogTemp, Log, TEXT("[%s] Input: %s"), *Caller, *Repr(InputVector));
    UE_LOG(LogTemp, Log, TEXT("[%s] Final: %s"), *Caller, *Repr(FinalPos));
    
    // TestNotEqual would be used in actual UE testing framework
    check(InitialPos != FinalPos);
}
```

## Unreal Engine Version Compatibility

- **Unreal Engine 5.0** and higher
- Compatible with all rendering paths and platforms
- Works in both Editor and Runtime builds
- Supports Blueprint and C++ integration

## Dependencies

- **Unreal Engine 5.0+**
- **CoreMinimal** module (standard UE dependency)
- **Engine** module for UWorld and Actor functionality
- No external dependencies required

## Roadmap

**Current Features:**

✅ `URepr` - Comprehensive object representation with Rust-style type suffixes  
✅ `UCallStack` - Call stack tracking with function names and detailed info  
✅ `UWorldNavigator` - World hierarchy navigation with path-based object finding  
✅ Template-based type system for extensibility  
✅ Blueprint integration for UCallStack and UWorldNavigator  
✅ Logging macros (UE_REPR, UE_REPR_WARNING, UE_REPR_ERROR)  
✅ Support for Unreal Engine container types (TArray, TMap, TSet, etc.)  
✅ Zero external dependencies  
✅ Header-only template system for URepr

**Planned Features:**

- 🔄 Parsing with helpful error messages, down to column number
- 🔄 Blueprint-callable URepr functionality
- 🔄 FVector, FRotator, and other UE math type specializations for URepr
- 🔄 

*This library started as a solution for Unreal Engine debugging pain points and is growing into a comprehensive UE debugging toolkit.*

## Contributing

Built out of frustration with Unreal Engine debugging challenges. If you have ideas for additional UE debugging utilities or find bugs, contributions are welcome!

**Ideas for new features:**

- Unreal Engine-specific performance profiling
- Actor lifecycle tracking
- Component state change monitoring
- UE event system debugging (delegates, multicast delegates)
- Custom Editor utility integration
- Blueprint debugging enhancements
- Animation Blueprint state visualization
- Gameplay Ability System debugging tools

## License

This project follows MIT license.

---

**Stop debugging blind in Unreal Engine. See your actual data with crystal clarity and know exactly where your code executes. 🎮**
