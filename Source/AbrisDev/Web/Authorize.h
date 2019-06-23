// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RequestClass.h"
#include "Authorize.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAuthorizeDelegat);

USTRUCT()
struct FDemoData
{
	GENERATED_BODY()

	UPROPERTY()
	FString info;
};

USTRUCT()
struct FAbrisData1
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FDemoData> data;
};

USTRUCT()
struct FRequest1
{
	GENERATED_BODY()

	UPROPERTY()
	FString jsonrpc;
	UPROPERTY()
		FAbrisData1 result;
};

UCLASS()
class ABRISDEV_API UAuthorize : public UObject, public RequestClass
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static UAuthorize* Authorize(FString email, FString password);

	UPROPERTY(BlueprintAssignable)
	FAuthorizeDelegat AuthorizeDelegat;

private:

	void OnAuthorizeCompleted(FHttpRequestPtr Req, FHttpResponsePtr Resp, bool success);
	void OnDemoCompleted(FHttpRequestPtr Req, FHttpResponsePtr Resp, bool success);
	void OnDemoCookCompleted(FHttpRequestPtr Req, FHttpResponsePtr Resp, bool success);
	void OnAuthorizeDemoCompleted(FHttpRequestPtr Req, FHttpResponsePtr Resp, bool success);

	void GetDemoMode();

	void GetCookDemo();

	void AuthorizeDemo();

	FString demoID;
	FString password;
};
