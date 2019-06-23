// Fill out your copyright notice in the Description page of Project Settings.


#include "RequestClass.h"

RequestClass::RequestClass()
{
}

TSharedPtr<IHttpRequest> RequestClass::CreateRequest(AbriseUrlMode mode)
{
	auto request = FHttpModule::Get().CreateRequest();

	request->SetHeader("Content-Type", "application/x-www-form-urlencoded");
	request->SetVerb("POST");
	switch (mode)
	{
	case AbriseUrlMode::MAIN:
	{
		request->SetURL("http://abris.site/Server/request.php");
		break;
	}
	case AbriseUrlMode::DEMO:
	{
		request->SetURL("http://demo.abris.site/Server/request.php");
		break;
	}
	}

	return request;
}

