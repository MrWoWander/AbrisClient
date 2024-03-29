#include "Entity.h"
#include "JsonUtilities/Public/JsonObjectConverter.h"
#include "Runtime/Json/Public/Json.h"

UEntity* UEntity::GetEntity()
{
	auto obj = NewObject<UEntity>();

	obj->HttpRequest = CreateRequest(AbriseUrlMode::DEMO);
	obj->HttpRequest->SetContentAsString("jsonrpc=2.0&method=getTableDataPredicate&params=%5B%7B%22entityName%22%3A%22entity_edit%22%2C+%22schemaName%22%3A%22meta%22%7D%5D");

	obj->HttpRequest->OnProcessRequestComplete().BindUObject(obj, &UEntity::OnGetEntityCompleted);
	obj->HttpRequest->ProcessRequest();

	return obj;
}

UEntity* UEntity::GetProperty(FString entity_id)
{
	auto obj = NewObject<UEntity>();

	obj->HttpRequest = CreateRequest(AbriseUrlMode::DEMO);
	obj->HttpRequest->SetContentAsString("jsonrpc=2.0&method=getTableDataPredicate&params=%5B%7B%22entityName%22%3A%22property%22%2C%22schemaName%22%3A%22meta%22%2C%22predicate%22%3A%7B%22strict%22%3Atrue%2C%22operands%22%3A%5B%7B%22levelup%22%3Afalse%2C%22operand%22%3A%7B%22field%22%3A%22entity_id%22%2C%22op%22%3A%22EQ%22%2C%22value%22%3A%22" + entity_id +"%22%2C%22search_in_key%22%3Atrue%7D%7D%5D%7D%7D%5D");

	obj->HttpRequest->OnProcessRequestComplete().BindUObject(obj, &UEntity::OnGetPropertyCompleted);
	obj->HttpRequest->ProcessRequest();

	return obj;
}

UEntity* UEntity::DelEntity(FString value)
{
	HttpRequest = CreateRequest(AbriseUrlMode::DEMO);
	HttpRequest->SetContentAsString("jsonrpc=2.0&method=deleteEntitiesByKey&params=%5B%7B%22entityName%22%3A%22entity%22%2C%22schemaName%22%3A%22meta%22%2C%22key%22%3A%22entity_id%22%2C%22value%22%3A%22" + value + "%22%7D%5D");

	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UEntity::OnDelEntityCompleted);
	HttpRequest->ProcessRequest();

	return this;
}

UEntity* UEntity::GetFilledEntity(FString table_name)
{
	HttpRequest = CreateRequest(AbriseUrlMode::DEMO);
	HttpRequest->SetContentAsString("jsonrpc=2.0&method=getTableDataPredicate&params=%5B%7B%22entityName%22%3A%22"+ FGenericPlatformHttp::UrlEncode(table_name) +"%22%2C+%22schemaName%22%3A%22public%22%7D%5D");

	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UEntity::OnGetCurrentEntityCompleted);
	HttpRequest->ProcessRequest();

	return this;
}

UEntity* UEntity::AddEntity(FAddEntity entity)
{
	auto obj = NewObject<UEntity>();
	obj->HttpRequest = CreateRequest(AbriseUrlMode::DEMO);
	obj->HttpRequest->SetContentAsString("jsonrpc=2.0&method=addEntities&params=%5B%7B%22entityName%22%3A%22entity%22%2C%22schemaName%22%3A%22meta%22%2C%22fields%22%3A%5B%7B%22entity_id%22%3A%22" + FGenericPlatformHttp::UrlEncode(entity.entity_id) + "%22%2C%22schema_name%22%3A%22" + FGenericPlatformHttp::UrlEncode(entity.schema_name) + "%22%2C%22table_name%22%3A%22" + FGenericPlatformHttp::UrlEncode(entity.table_name) + "%22%2C%22title%22%3A%22" + FGenericPlatformHttp::UrlEncode(entity.title) + "%22%2C%22primarykey%22%3A%22" + FGenericPlatformHttp::UrlEncode(entity.primarykey) + "%22%2C%22table_type%22%3A%22" + FGenericPlatformHttp::UrlEncode(entity.table_type) + "%22%2C%22view_definition%22%3A%22%22%2C%22base_entity_id%22%3A%22" + FGenericPlatformHttp::UrlEncode(entity.base_entity_id) + "%22%7D%5D%2C%22files%22%3A%5B%5D%2C%22key%22%3A%22entity_id%22%2C%22types%22%3Anull%7D%5D");

	obj->HttpRequest->OnProcessRequestComplete().BindUObject(obj, &UEntity::OnAddEntityCompleted);
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

void UEntity::OnGetCurrentEntityCompleted(FHttpRequestPtr Req, FHttpResponsePtr Resp, bool success)
{
	if (!success)
		return;

	UE_LOG(LogTemp, Log, TEXT("%s"), *Resp->GetContentAsString());

	ReadJson(Resp->GetContentAsString());
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

void UEntity::OnAddEntityCompleted(FHttpRequestPtr Req, FHttpResponsePtr Resp, bool success)
{
	if (!success)
		return;

	UE_LOG(LogTemp, Log, TEXT("%s"), *Resp->GetContentAsString());

	const TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(Resp->GetContentAsString());
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
	
		if (JsonObject->GetArrayField("error").Num() == 0)
		{
			AddEntityDelegat.Broadcast();
		} else
		{
			AddEntityFailDelegat.Broadcast();
		}
	}

}

void UEntity::OnDelEntityCompleted(FHttpRequestPtr Req, FHttpResponsePtr Resp, bool success)
{
	if (!success)
		return;

	UE_LOG(LogTemp, Log, TEXT("%s"), *Resp->GetContentAsString());

	DellEntityDelegat.Broadcast();
}

void UEntity::ReadJson(const FString& jsonData)
{
	const TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(jsonData);
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		TArray<FFilledEntityMap> jsonEntity;
		JsonObject = JsonObject->GetObjectField("result");
		for (auto json : JsonObject->GetArrayField("data"))
		{
			TMap<FString, FString> mapEntity;

			for(auto j : json->AsObject()->Values)
			{
				
				UE_LOG(LogTemp, Log, TEXT("Key: %s, Value: %s"), *j.Key, *j.Value->AsString())

				FString replaceHTMLStyle = j.Value->AsString();

				if (replaceHTMLStyle.Find(TEXT("<p>")) >= 0) {
					replaceHTMLStyle.ReplaceInline(TEXT("<p>"), TEXT(""));
					replaceHTMLStyle.ReplaceInline(TEXT("</p>"), TEXT("\n"));
					replaceHTMLStyle.ReplaceInline(TEXT("<br>"), TEXT(""));
					UE_LOG(LogTemp, Log, TEXT("HTML: %s"), *replaceHTMLStyle);
				}

				mapEntity.Add(j.Key, replaceHTMLStyle);
			}
			FFilledEntityMap m;
			m.keys = mapEntity;

			jsonEntity.Add(m);
		}

		FFilledEntity d;
		d.date = jsonEntity;

		CurrentEntityDelegat.Broadcast(d);
	}
}
