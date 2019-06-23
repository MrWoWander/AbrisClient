#include "Authorize.h"
#include "JsonUtilities/Public/JsonObjectConverter.h"

UAuthorize* UAuthorize::Authorize(FString email, FString password)
{
	auto obj = NewObject<UAuthorize>();

	obj->HttpRequest = CreateRequest(AbriseUrlMode::MAIN);

	FString ContentString = "jsonrpc=2.0&method=authenticate&params=%5B%7B%22usename%22%3A%22" + FGenericPlatformHttp::UrlEncode(email) + "%22%2C%22passwd%22%3A%22"+ FGenericPlatformHttp::UrlEncode(password) + "%22%7D%5D";
	obj->password = password;
	obj->HttpRequest->SetContentAsString(ContentString);


	obj->HttpRequest->OnProcessRequestComplete().BindUObject(obj, &UAuthorize::OnAuthorizeCompleted);
	obj->HttpRequest->ProcessRequest();

	return obj;
}

void UAuthorize::OnAuthorizeCompleted(FHttpRequestPtr Req, FHttpResponsePtr Resp, bool success)
{
	if (!success)
		return;

	UE_LOG(LogTemp, Log, TEXT("%s"), *Resp->GetContentAsString());

	GetDemoMode();
}

void UAuthorize::OnDemoCompleted(FHttpRequestPtr Req, FHttpResponsePtr Resp, bool success)
{
	if (!success)
		return;

	UE_LOG(LogTemp, Log, TEXT("YEEEE!"));

	UE_LOG(LogTemp, Log, TEXT("%s"), *Resp->GetContentAsString());

	FRequest1 t;

	FJsonObjectConverter::JsonObjectStringToUStruct(Resp->GetContentAsString(), &t, 0, 0);


	FString d = t.result.data[0].info;
	demoID = d.Mid(d.Find("?"), d.Find("\"" , ESearchCase::IgnoreCase, ESearchDir::FromStart, d.Find("?")) - d.Find("?"));

	GetCookDemo();
}

void UAuthorize::OnDemoCookCompleted(FHttpRequestPtr Req, FHttpResponsePtr Resp, bool success)
{
	if (!success)
		return;

	for (auto s : Resp->GetAllHeaders())
	UE_LOG(LogTemp, Log, TEXT("%s"), *s);

	AuthorizeDemo();
}

void UAuthorize::OnAuthorizeDemoCompleted(FHttpRequestPtr Req, FHttpResponsePtr Resp, bool success)
{
	if (!success)
		return;

	UE_LOG(LogTemp, Log, TEXT("%s"), *Resp->GetContentAsString());

	AuthorizeDelegat.Broadcast();
}

void UAuthorize::GetDemoMode()
{

	HttpRequest = CreateRequest(AbriseUrlMode::MAIN);
	HttpRequest->SetContentAsString("jsonrpc=2.0&method=getTableDataPredicate&params=%5B%7B%22entityName%22%3A%22my_db%22%2C+%22schemaName%22%3A%22public%22%7D%5D");

	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UAuthorize::OnDemoCompleted);
	HttpRequest->ProcessRequest();
}

void UAuthorize::GetCookDemo()
{
	HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("GET");

	HttpRequest->SetURL("http://demo.abris.site" / demoID);

	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UAuthorize::OnDemoCookCompleted);
	HttpRequest->ProcessRequest();
}

void UAuthorize::AuthorizeDemo()
{

	HttpRequest = CreateRequest(AbriseUrlMode::DEMO);

	FString ContentString = "jsonrpc=2.0&method=authenticate&params=%5B%7B%22usename%22%3A%22admin%22%2C%22passwd%22%3A%22" + FGenericPlatformHttp::UrlEncode(password) + "%22%7D%5D";

	HttpRequest->SetContentAsString(ContentString);

	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UAuthorize::OnAuthorizeDemoCompleted);
	HttpRequest->ProcessRequest();
}
