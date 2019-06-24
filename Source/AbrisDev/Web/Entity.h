// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RequestClass.h"
#include "PropertyType.h"
#include "Entity.generated.h"


USTRUCT(BlueprintType)
struct FEntityData
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

USTRUCT(BlueprintType)
struct FAbrisData
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadWrite)
		TArray<FEntityData> data;
};

USTRUCT(BlueprintType)
struct FRequest
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadWrite)
		FString jsonrpc;
	UPROPERTY(BlueprintReadWrite)
		FAbrisData result;
};

USTRUCT(BlueprintType)
struct FFilledEntityMap
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadWrite)
		TMap<FString, FString> keys;
};

USTRUCT(BlueprintType)
struct FFilledEntity
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadWrite)
		TArray<FFilledEntityMap> date;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEntityDelegat, FRequest, Req);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPropertyDelegat, FRequestProperty, Req);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFilledEntityDelegat, FFilledEntity, Req);


UCLASS()
class ABRISDEV_API UEntity : public UObject, public RequestClass
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static UEntity* GetEntity();

	UFUNCTION(BlueprintCallable)
	static UEntity* GetProperty(FString entity_id);

	UFUNCTION(BlueprintCallable)
		UEntity* GetFilledEntity(FString table_name);

	UPROPERTY(BlueprintAssignable)
	FEntityDelegat EntityDelegat;

	UPROPERTY(BlueprintAssignable)
	FPropertyDelegat PropertyDelegat;

	UPROPERTY(BlueprintAssignable)
		FFilledEntityDelegat CurrentEntityDelegat;

private:

	void OnGetEntityCompleted(FHttpRequestPtr Req, FHttpResponsePtr Resp, bool success);
	void OnGetCurrentEntityCompleted(FHttpRequestPtr Req, FHttpResponsePtr Resp, bool success);
	void OnGetPropertyCompleted(FHttpRequestPtr Req, FHttpResponsePtr Resp, bool success);

	void ReadJson(const FString& jsonData);
};
