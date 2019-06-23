#include "CoreMinimal.h"
#include "PropertyType.generated.h"

USTRUCT(BlueprintType)
struct FPropertyData
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadWrite)
		FString property_name;

	UPROPERTY(BlueprintReadWrite)
		FString column_name;

	UPROPERTY(BlueprintReadWrite)
		FString entity_id;

	UPROPERTY(BlueprintReadWrite)
		FString type;

	UPROPERTY(BlueprintReadWrite)
		FString data_type;

	UPROPERTY(BlueprintReadWrite)
		FString visible;

	UPROPERTY(BlueprintReadWrite)
		FString readonly;

	UPROPERTY(BlueprintReadWrite)
		FString title;
	UPROPERTY(BlueprintReadWrite)
		FString ref_entity;
	UPROPERTY(BlueprintReadWrite)
		FString ref_key;
	UPROPERTY(BlueprintReadWrite)
		FString _order;
	UPROPERTY(BlueprintReadWrite)
		FString constraint_name;
	UPROPERTY(BlueprintReadWrite)
		FString is_nullable;
	UPROPERTY(BlueprintReadWrite)
		FString default;
};

USTRUCT(BlueprintType)
struct FAbrisDataProperty
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadWrite)
		TArray<FPropertyData> data;
};

USTRUCT(BlueprintType)
struct FRequestProperty
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadWrite)
		FString jsonrpc;
	UPROPERTY(BlueprintReadWrite)
		FAbrisDataProperty result;
};