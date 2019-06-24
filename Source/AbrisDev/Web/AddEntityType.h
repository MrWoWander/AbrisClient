#include "CoreMinimal.h"
#include "AddEntityType.generated.h"

USTRUCT(BlueprintType)
struct FAddEntity
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
		FString entity_id;
	UPROPERTY(BlueprintReadWrite)
		FString schema_name;
	UPROPERTY(BlueprintReadWrite)
		FString table_name;
	UPROPERTY(BlueprintReadWrite)
		FString title;
	UPROPERTY(BlueprintReadWrite)
		FString primarykey;
	UPROPERTY(BlueprintReadWrite)
		FString table_type;
	UPROPERTY(BlueprintReadWrite)
		FString base_entity_id;
};