// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Containers/Ticker.h"
#include "WidgetAnimationPlayCallbackProxyV2.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWidgetAnimationV2Result);

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

	// Called when the animation reaches its end naturally
	UPROPERTY(BlueprintAssignable)
	FWidgetAnimationV2Result Finished;

	// Called when the animation is stopped before reaching its end
	UPROPERTY(BlueprintAssignable)
	FWidgetAnimationV2Result Interrupted;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", DisplayName = "Play Animation with Finished event V2", ShortToolTip = "Play Animation and trigger event on Finished, reversing in place if playing",  ToolTip="Play Animation on widget, reversing in place if animation is playing, and trigger Finish event when the animation is done."), Category = "User Interface|Animation")
	static PLAYWIDGETANIMATION_API UWidgetAnimationPlayCallbackProxyV2* CreatePlayAnimationProxyObject(FWidgetAnimationHandle& Result, class UUserWidget* Widget, class UWidgetAnimation* InAnimation, EUMGSequencePlayModeV2::Type PlayMode = EUMGSequencePlayModeV2::Forward, float PlaybackSpeed = 1.0f);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", DisplayName = "Play Animation with Finished event V2 Duo", ShortToolTip = "Play Animation and trigger event on Finished, reversing in place if playing",  ToolTip="Play Animation on widget, reversing in place if animation is playing, and trigger Finish event when the animation is done."), Category = "User Interface|Animation")
	static PLAYWIDGETANIMATION_API UWidgetAnimationPlayCallbackProxyV2* CreatePlayAnimationDuoProxyObject(FWidgetAnimationHandle& Result, class UUserWidget* Widget, class UWidgetAnimation* InForwardAnimation, class UWidgetAnimation* InReverseAnimation, EUMGSequencePlayModeV2::Type PlayMode = EUMGSequencePlayModeV2::Forward, float PlaybackSpeed = 1.0f);
		
	static PLAYWIDGETANIMATION_API UWidgetAnimationPlayCallbackProxyV2* CreatePlayAnimationProxyObjectDeferred();
	static PLAYWIDGETANIMATION_API void PlayAnimationProxyObjectDeferred(UWidgetAnimationPlayCallbackProxyV2* Proxy, FWidgetAnimationHandle& Result, class UUserWidget* Widget, class UWidgetAnimation* InAnimation, EUMGSequencePlayModeV2::Type PlayMode = EUMGSequencePlayModeV2::Forward, float PlaybackSpeed = 1.0f);
	static PLAYWIDGETANIMATION_API void PlayAnimationDuoProxyObjectDeferred(UWidgetAnimationPlayCallbackProxyV2* Proxy, FWidgetAnimationHandle& Result, class UUserWidget* Widget, class UWidgetAnimation* InForwardAnimation, class UWidgetAnimation* InReverseAnimation, EUMGSequencePlayModeV2::Type PlayMode = EUMGSequencePlayModeV2::Forward, float PlaybackSpeed = 1.0f);
	
private:
	void ExecutePlayAnimation(class UUserWidget* Widget, class UWidgetAnimation* InAnimation, EUMGSequencePlayModeV2::Type PlayMode, float PlaybackSpeed);
	void ExecutePlayDuoAnimation(class UUserWidget* Widget, class UWidgetAnimation* InForwardAnimation, class UWidgetAnimation* InReverseAnimation, EUMGSequencePlayModeV2::Type PlayMode, float PlaybackSpeed);
	void OnSequenceFinished(FWidgetAnimationState& State);
	bool OnAnimationFinished(float DeltaTime);

	void BeginProgressTracking(class UUserWidget* Widget, class UWidgetAnimation* Anim, bool bForward);
	bool SampleProgress(float DeltaTime);
	void StopProgressTracking();

	FWidgetAnimationHandle WidgetAnimationHandle;
	FDelegateHandle OnFinishedHandle;

	TWeakObjectPtr<class UUserWidget> TrackedWidget;
	TWeakObjectPtr<class UWidgetAnimation> TrackedAnimation;
	FTSTicker::FDelegateHandle ProgressTickerHandle;
	float ProgressBoundaryTime = 0.f;
	float LastSampleTime = 0.f;
	bool bTrackForward = true;

	// Whether the tracked animation reached its end boundary before finishing (natural completion vs external Stop)
	bool bReachedEnd = false;
};
