#include "Entity.h"
#include "JsonUtilities/Public/JsonObjectConverter.h"

UEntity* UEntity::GetEntity()
{
	auto obj = NewObject<UEntity>();

	obj->HttpRequest = CreateRequest(AbriseUrlMode::DEMO);
	obj->HttpRequest->SetContentAsString("jsonrpc=2.0&method=getTableDataPredicate&params=%5B%7B%22entityName%22%3A%22entity_edit%22%2C+%22schemaName%22%3A%22meta%22%7D%5D");

	obj->HttpRequest->OnProcessRequestComplete().BindUObject(obj, &UEntity::OnGetEntityCompleted);
	obj->HttpRequest->ProcessRequest();

	return obj;
}

UEntity* UEntity::GetProperty(FEntityData entity)
{
	auto obj = NewObject<UEntity>();

	obj->HttpRequest = CreateRequest(AbriseUrlMode::DEMO);
	obj->HttpRequest->SetContentAsString("jsonrpc=2.0&method=getTableDataPredicate&params=%5B%7B%22entityName%22%3A%22property%22%2C%22schemaName%22%3A%22meta%22%2C%22predicate%22%3A%7B%22strict%22%3Atrue%2C%22operands%22%3A%5B%7B%22levelup%22%3Afalse%2C%22operand%22%3A%7B%22field%22%3A%22entity_id%22%2C%22op%22%3A%22EQ%22%2C%22value%22%3A%22" + entity.entity_id +"%22%2C%22search_in_key%22%3Atrue%7D%7D%5D%7D%7D%5D");

	obj->HttpRequest->OnProcessRequestComplete().BindUObject(obj, &UEntity::OnGetPropertyCompleted);
	obj->HttpRequest->ProcessRequest();

	return obj;
}

void UEntity::OnGetEntityCompleted(FHttpRequestPtr Req, FHttpResponsePtr Resp, bool success)
{
	if (!success)
		return;

	UE_LOG(LogTemp, Log, TEXT("%s"), *Resp->GetContentAsString());

	FRequest t;

	FJsonObjectConverter::JsonObjectStringToUStruct(Resp->GetContentAsString(), &t, 0, 0);

	for (int i = 0; i < t.result.data.Num(); i++)
		UE_LOG(LogTemp, Log, TEXT("%s"), *t.result.data[i].title)

		EntityDelegat.Broadcast(t);
}

void UEntity::OnGetPropertyCompleted(FHttpRequestPtr Req, FHttpResponsePtr Resp, bool success)
{
	if (!success)
		return;

	UE_LOG(LogTemp, Log, TEXT("%s"), *Resp->GetContentAsString());

	FRequestProperty t;

	FJsonObjectConverter::JsonObjectStringToUStruct(Resp->GetContentAsString(), &t, 0, 0);

	for (int i = 0; i < t.result.data.Num(); i++)
		UE_LOG(LogTemp, Log, TEXT("%s"), *t.result.data[i].title)

	PropertyDelegat.Broadcast(t);
}
