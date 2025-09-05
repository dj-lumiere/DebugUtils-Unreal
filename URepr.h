#pragma once
#include "CoreMinimal.h"
#include "Containers/Array.h"
#include "Containers/Map.h"
#include "Containers/Set.h"

template <typename T>
FString Repr(const T& Value);

template <typename T, typename U>
FString Repr(const TPair<T, U>& Pair)
{
	return FString::Printf(TEXT("(%s, %s)"), *Repr(Pair.Key), *Repr(Pair.Value));
}

template <>
inline FString Repr(const int8& Value)
{
	return FString::Printf(TEXT("%i_i8"), Value);
}

template <>
inline FString Repr(const int16& Value)
{
	return FString::Printf(TEXT("%i_i16"), Value);
}

template <>
inline FString Repr(const int32& Value)
{
	return FString::Printf(TEXT("%i_i32"), Value);
}

template <>
inline FString Repr(const int64& Value)
{
	return FString::Printf(TEXT("%lld_i64"), Value);
}

template <>
inline FString Repr(const uint8& Value)
{
	return FString::Printf(TEXT("%u_u8"), Value);
}

template <>
inline FString Repr(const uint16& Value)
{
	return FString::Printf(TEXT("%u_u16"), Value);
}

template <>
inline FString Repr(const uint32& Value)
{
	return FString::Printf(TEXT("%u_u32"), Value);
}

template <>
inline FString Repr(const uint64& Value)
{
	return FString::Printf(TEXT("%llu_u64"), Value);
}

template <>
inline FString Repr(const float& Value)
{
	return FString::Printf(TEXT("%f_f32"), Value);
}

template <>
inline FString Repr(const double& Value)
{
	return FString::Printf(TEXT("%f_f64"), Value);
}

template <>
inline FString Repr(const bool& Value)
{
	return Value ? TEXT("true") : TEXT("false");
}

template <>
inline FString Repr(const FString& Value)
{
	return FString::Printf(TEXT("\"%s\""), *Value);
}

template <>
inline FString Repr(const FName& Value)
{
	return Value.ToString();
}

template <>
inline FString Repr(const FText& Value)
{
	return Value.ToString();
}

template <typename T>
FString Repr(const TArray<T>& Array)
{
	FString Result = TEXT("[");
	for (int32 i = 0; i < Array.Num(); ++i)
	{
		Result += Repr(Array[i]);
		if (i < Array.Num() - 1)
		{
			Result += TEXT(", ");
		}
	}
	Result += TEXT("]");
	return Result;
}

template <typename T>
FString Repr(const TSet<T>& Set)
{
	FString Result = TEXT("{");
	int32 Index = 0;
	for (const T& Element : Set)
	{
		Result += Repr(Element);
		if (Index < Set.Num() - 1)
		{
			Result += TEXT(", ");
		}
		++Index;
	}
	Result += TEXT("}");
	return Result;
}

template <typename K, typename V>
FString Repr(const TMap<K, V>& Map)
{
	FString Result = TEXT("{");
	int32 Index = 0;
	for (const auto& Pair : Map)
	{
		Result += FString::Printf(TEXT("%s: %s"), *Repr(Pair.Key), *Repr(Pair.Value));
		if (Index < Map.Num() - 1)
		{
			Result += TEXT(", ");
		}
		++Index;
	}
	Result += TEXT("}");
	return Result;
}

template <typename T>
FString Repr(const TOptional<T>& Optional)
{
	if (Optional.IsSet())
	{
		return FString::Printf(TEXT("Some(%s)"), *Repr(Optional.GetValue()));
	}
	return TEXT("None");
}

template <typename... Types>
FString Repr(const TTuple<Types...>& Tuple);

template <typename Tuple, size_t... Is>
FString ReprTupleImpl(const Tuple& Tup, std::index_sequence<Is...>)
{
	TArray<FString> Elements = {Repr(Tup.template Get<Is>())...};
	FString Result;
	for (int32 i = 0; i < Elements.Num(); ++i)
	{
		Result += Elements[i];
		if (i < Elements.Num() - 1)
		{
			Result += TEXT(", ");
		}
	}
	return Result;
}

template <typename... Types>
FString Repr(const TTuple<Types...>& Tuple)
{
	return TEXT("(") + ReprTupleImpl(Tuple, std::index_sequence_for<Types...>{}) + TEXT(")");
}

#define UE_REPR(x) UE_LOG(LogTemp, Display, TEXT("%s = %s"), TEXT(#x), *Repr(x))
#define UE_REPR_WARNING(x) UE_LOG(LogTemp, Warning, TEXT("%s = %s"), TEXT(#x), *Repr(x))
#define UE_REPR_ERROR(x) UE_LOG(LogTemp, Error, TEXT("%s = %s"), TEXT(#x), *Repr(x))
