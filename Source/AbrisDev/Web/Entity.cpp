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

UEntity* UEntity::GetFilledEntity(FString table_name)
{
	HttpRequest = CreateRequest(AbriseUrlMode::DEMO);
	HttpRequest->SetContentAsString("jsonrpc=2.0&method=getTableDataPredicate&params=%5B%7B%22entityName%22%3A%22"+ FGenericPlatformHttp::UrlEncode(table_name) +"%22%2C+%22schemaName%22%3A%22public%22%7D%5D");

	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UEntity::OnGetCurrentEntityCompleted);
	HttpRequest->ProcessRequest();

	return this;
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
