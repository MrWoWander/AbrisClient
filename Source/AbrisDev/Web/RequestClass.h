#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "RequestClass.generated.h"

UENUM(BlueprintType)
enum class AbriseUrlMode : uint8
{
	MAIN,
	DEMO
};

class ABRISDEV_API RequestClass
{
public:

	RequestClass();

protected:

	static TSharedPtr<IHttpRequest> CreateRequest(AbriseUrlMode mode);

	TSharedPtr<IHttpRequest> HttpRequest;
};
