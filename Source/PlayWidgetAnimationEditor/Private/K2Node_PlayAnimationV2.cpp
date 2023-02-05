// Copyright Epic Games, Inc. All Rights Reserved.

#include "K2Node_PlayAnimationV2.h"

#include "WidgetAnimationPlayCallbackProxyV2.h"

#define LOCTEXT_NAMESPACE "UMG"

UK2Node_PlayAnimationV2::UK2Node_PlayAnimationV2(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UWidgetAnimationPlayCallbackProxyV2, CreatePlayAnimationProxyObject);
	ProxyFactoryClass = UWidgetAnimationPlayCallbackProxyV2::StaticClass();
	ProxyClass = UWidgetAnimationPlayCallbackProxyV2::StaticClass();
}

UK2Node_PlayAnimationV2Duo::UK2Node_PlayAnimationV2Duo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UWidgetAnimationPlayCallbackProxyV2, CreatePlayAnimationDuoProxyObject);
	ProxyFactoryClass = UWidgetAnimationPlayCallbackProxyV2::StaticClass();
	ProxyClass = UWidgetAnimationPlayCallbackProxyV2::StaticClass();
}

#undef LOCTEXT_NAMESPACE
