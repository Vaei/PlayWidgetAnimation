// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "WidgetAnimationPlayCallbackProxyV2.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWidgetAnimationResult);

/** Describes playback modes for UMG sequences. */
UENUM(BlueprintType)
namespace EUMGSequencePlayModeV2
{
	enum Type
	{
		/** Animation plays and loops from the beginning to the end. */
		Forward,
		/** Animation plays and loops from the end to the beginning. */
		Reverse,
	};
}

UCLASS(MinimalAPI)
class UWidgetAnimationPlayCallbackProxyV2 : public UObject
{
	GENERATED_UCLASS_BODY()

	// Called when animation has been completed
	UPROPERTY(BlueprintAssignable)
	FWidgetAnimationResult Finished;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", DisplayName = "Play Animation with Finished event V2", ShortToolTip = "Play Animation and trigger event on Finished, reversing in place if playing",  ToolTip="Play Animation on widget, reversing in place if animation is playing, and trigger Finish event when the animation is done."), Category = "User Interface|Animation")
	static UWidgetAnimationPlayCallbackProxyV2* CreatePlayAnimationProxyObject(class UUMGSequencePlayer*& Result, class UUserWidget* Widget, class UWidgetAnimation* InAnimation, EUMGSequencePlayModeV2::Type PlayMode = EUMGSequencePlayModeV2::Forward, float PlaybackSpeed = 1.0f);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", DisplayName = "Play Animation with Finished event V2 Duo", ShortToolTip = "Play Animation and trigger event on Finished, reversing in place if playing",  ToolTip="Play Animation on widget, reversing in place if animation is playing, and trigger Finish event when the animation is done."), Category = "User Interface|Animation")
	static UWidgetAnimationPlayCallbackProxyV2* CreatePlayAnimationDuoProxyObject(class UUMGSequencePlayer*& Result, class UUserWidget* Widget, class UWidgetAnimation* InForwardAnimation, class UWidgetAnimation* InReverseAnimation, EUMGSequencePlayModeV2::Type PlayMode = EUMGSequencePlayModeV2::Forward, float PlaybackSpeed = 1.0f);
	
private:
	class UUMGSequencePlayer* ExecutePlayAnimation(class UUserWidget* Widget, class UWidgetAnimation* InAnimation, EUMGSequencePlayModeV2::Type PlayMode, float PlaybackSpeed);
	class UUMGSequencePlayer* ExecutePlayDuoAnimation(class UUserWidget* Widget, class UWidgetAnimation* InForwardAnimation, class UWidgetAnimation* InReverseAnimation, EUMGSequencePlayModeV2::Type PlayMode, float PlaybackSpeed);
	void OnSequenceFinished(class UUMGSequencePlayer& Player);
	bool OnAnimationFinished(float DeltaTime);

	FDelegateHandle OnFinishedHandle;
};
