# UWorldNavigator Documentation

## Overview

UWorldNavigator is a powerful Blueprint Function Library that provides utilities for navigating and manipulating Unreal Engine's world hierarchy. It allows you to find actors, components, and scene components by their world paths, as well as generate world paths for existing objects.

## Key Features

- **Path-based Object Finding**: Find actors, components, and scene components using structured path strings
- **World Path Generation**: Generate unique path identifiers for actors and components
- **Hierarchical Navigation**: Navigate through world → level → actor → component hierarchies
- **Blueprint Integration**: All functions are Blueprint-callable for easy integration

## Path Structure Format

UWorldNavigator uses a specific path format to identify objects in the world hierarchy:

```
WorldName@LevelName[LevelIndex]:/ActorName[ActorIndex]->ComponentName[ComponentIndex]
```

**Components:**
- `WorldName`: Name of the UWorld object
- `LevelName[LevelIndex]`: Level name with index for duplicate level names
- `ActorName[ActorIndex]`: Actor name with index for duplicate actor names  
- `ComponentName[ComponentIndex]`: Component name with index for duplicate component names

**Examples:**
- Actor path: `MyWorld@PersistentLevel[0]:/PlayerPawn[0]`
- Component path: `MyWorld@PersistentLevel[0]:/PlayerPawn[0]->MeshComponent[0]`
- Scene component path: `MyWorld@PersistentLevel[0]:/PlayerPawn[0]->RootComponent[0]/MeshComponent[1]`

## API Reference

### Finding Objects by Path

#### `FindUActorByPath(FString& Path)`

Finds an actor in the world using its path string.

**Parameters:**
- `Path`: String path identifying the actor

**Returns:** 
- `AActor*`: Pointer to the found actor, or nullptr if not found

**C++ Example:**
```cpp
FString ActorPath = TEXT("MyWorld@PersistentLevel[0]:/PlayerCharacter[0]");
AActor* FoundActor = UWorldNavigator::FindUActorByPath(ActorPath);
if (FoundActor)
{
    UE_LOG(LogTemp, Log, TEXT("Found actor: %s"), *FoundActor->GetName());
}
```

**Blueprint Usage:**
- Node: "Find UActor By Path"
- Category: World Navigator
- Input: Path (String)
- Output: Actor Reference

#### `FindActorComponentByPath(FString& Path)`

Finds an actor component using its path string.

**Parameters:**
- `Path`: String path identifying the component

**Returns:**
- `UActorComponent*`: Pointer to the found component, or nullptr if not found

**C++ Example:**
```cpp
FString ComponentPath = TEXT("MyWorld@PersistentLevel[0]:/PlayerCharacter[0]->HealthComponent[0]");
UActorComponent* FoundComponent = UWorldNavigator::FindActorComponentByPath(ComponentPath);
if (FoundComponent)
{
    UE_LOG(LogTemp, Log, TEXT("Found component: %s"), *FoundComponent->GetName());
}
```

#### `FindSceneComponentByPath(FString& Path)`

Finds a scene component using its path string, supporting hierarchical component structures.

**Parameters:**
- `Path`: String path identifying the scene component

**Returns:**
- `USceneComponent*`: Pointer to the found scene component, or nullptr if not found

**C++ Example:**
```cpp
FString SceneComponentPath = TEXT("MyWorld@PersistentLevel[0]:/PlayerCharacter[0]->RootComponent[0]/MeshComponent[0]");
USceneComponent* FoundSceneComponent = UWorldNavigator::FindSceneComponentByPath(SceneComponentPath);
if (FoundSceneComponent)
{
    FVector Location = FoundSceneComponent->GetComponentLocation();
    UE_LOG(LogTemp, Log, TEXT("Scene component location: %s"), *Location.ToString());
}
```

### Generating World Paths

#### `GetWorldPath(AActor* Actor)`

Generates a world path string for an actor.

**Parameters:**
- `Actor`: Pointer to the actor

**Returns:**
- `FString`: Generated path string

**C++ Example:**
```cpp
AActor* MyActor = GetWorld()->SpawnActor<AActor>();
FString ActorPath = UWorldNavigator::GetWorldPath(MyActor);
UE_LOG(LogTemp, Log, TEXT("Actor path: %s"), *ActorPath);
// Output: "MyWorld@PersistentLevel[0]:/Actor[0]"
```

#### `GetWorldPath(UActorComponent* Component)`

Generates a world path string for an actor component.

**Parameters:**
- `Component`: Pointer to the component

**Returns:**
- `FString`: Generated path string

**C++ Example:**
```cpp
UActorComponent* MyComponent = MyActor->GetComponentByClass<UStaticMeshComponent>();
FString ComponentPath = UWorldNavigator::GetWorldPath(MyComponent);
UE_LOG(LogTemp, Log, TEXT("Component path: %s"), *ComponentPath);
// Output: "MyWorld@PersistentLevel[0]:/MyActor[0]->StaticMeshComponent[0]"
```

#### `GetWorldPath(USceneComponent* Component)`

Generates a world path string for a scene component, including its full hierarchy.

**Parameters:**
- `Component`: Pointer to the scene component

**Returns:**
- `FString`: Generated path string with full component hierarchy

**C++ Example:**
```cpp
USceneComponent* ChildComponent = MyActor->GetRootComponent()->GetAttachChildren()[0];
FString SceneComponentPath = UWorldNavigator::GetWorldPath(ChildComponent);
UE_LOG(LogTemp, Log, TEXT("Scene component path: %s"), *SceneComponentPath);
// Output: "MyWorld@PersistentLevel[0]:/MyActor[0]->RootComponent[0]/ChildComponent[0]"
```

## Usage Scenarios

### 1. Object Serialization and Deserialization

```cpp
// Serialize an actor reference
AActor* ImportantActor = GetSomeActor();
FString SerializedPath = UWorldNavigator::GetWorldPath(ImportantActor);

// Save to file or send over network...

// Deserialize the actor reference
FString LoadedPath = LoadFromFile();
AActor* RestoredActor = UWorldNavigator::FindUActorByPath(LoadedPath);
```

### 2. Debug and Logging

```cpp
void LogActorHierarchy(AActor* Actor)
{
    FString ActorPath = UWorldNavigator::GetWorldPath(Actor);
    UE_LOG(LogTemp, Log, TEXT("Processing actor at: %s"), *ActorPath);
    
    // Log all components
    for (UActorComponent* Component : Actor->GetComponents())
    {
        FString ComponentPath = UWorldNavigator::GetWorldPath(Component);
        UE_LOG(LogTemp, Log, TEXT("  Component: %s"), *ComponentPath);
    }
}
```

### 3. Level Streaming and Persistence

```cpp
// Store references to objects before level unload
TArray<FString> ImportantObjectPaths;
for (AActor* Actor : ImportantActors)
{
    ImportantObjectPaths.Add(UWorldNavigator::GetWorldPath(Actor));
}

// After level reload, restore references
TArray<AActor*> RestoredActors;
for (const FString& Path : ImportantObjectPaths)
{
    AActor* RestoredActor = UWorldNavigator::FindUActorByPath(Path);
    if (RestoredActor)
    {
        RestoredActors.Add(RestoredActor);
    }
}
```

### 4. Network Synchronization

```cpp
// Send object reference over network
UFUNCTION(Server, Reliable)
void ServerUpdateObject(const FString& ObjectPath, const FTransform& NewTransform)
{
    AActor* TargetActor = UWorldNavigator::FindUActorByPath(ObjectPath);
    if (TargetActor)
    {
        TargetActor->SetActorTransform(NewTransform);
    }
}
```

## Error Handling

The functions return special error strings for invalid states:

- `"[invalid world]"`: World is null or invalid
- `"[invalid level]"`: Level is null or invalid  
- `"[invalid actor]"`: Actor is null or invalid
- `"[invalid component]"`: Component is null or invalid
- `"[null Component]"`: Scene component is null
- `"[null World]"`: World is null for scene component
- `"[null Level]"`: Level is null for scene component
- `"[null Actor]"`: Actor is null for scene component

Always check for these error strings and null return values:

```cpp
FString ActorPath = TEXT("SomeWorld@SomeLevel[0]:/SomeActor[0]");
AActor* FoundActor = UWorldNavigator::FindUActorByPath(ActorPath);

if (FoundActor == nullptr)
{
    UE_LOG(LogTemp, Error, TEXT("Could not find actor at path: %s"), *ActorPath);
    return;
}

// Safe to use FoundActor
```

## Performance Considerations

- **Caching**: Consider caching frequently accessed paths rather than regenerating them
- **Batch Operations**: When finding multiple objects, batch the operations to minimize lookups
- **Level Loading**: Be aware that objects may not be available until their levels are fully loaded
- **World Context**: Ensure the world context is valid when using these functions

## Integration with Blueprint

All UWorldNavigator functions are Blueprint-callable. In Blueprint graphs:

1. **Finding Objects**: Use "Find UActor By Path", "Find Actor Component By Path", or "Find Scene Component By Path" nodes
2. **Getting Paths**: Use "Get World Path" nodes with appropriate input types
3. **String Handling**: Use string manipulation nodes to work with path strings
4. **Error Checking**: Use "Is Valid" nodes to check if returned objects are valid

## Best Practices

1. **Always validate** returned objects before using them
2. **Handle invalid paths** gracefully with appropriate error logging
3. **Consider object lifetime** - paths become invalid when objects are destroyed
4. **Use appropriate path type** - actor paths for actors, component paths for components
5. **Document path formats** when passing paths between systems
6. **Test with duplicate names** to ensure indexing works correctly