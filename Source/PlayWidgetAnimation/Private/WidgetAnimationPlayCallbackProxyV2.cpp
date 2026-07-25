// Copyright Epic Games, Inc. All Rights Reserved.

#include "WidgetAnimationPlayCallbackProxyV2.h"

#include "Animation/UMGSequencePlayer.h"
#include "Animation/WidgetAnimation.h"
#include "Containers/Ticker.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(WidgetAnimationPlayCallbackProxyV2)

#define LOCTEXT_NAMESPACE "UMG"

UWidgetAnimationPlayCallbackProxyV2* UWidgetAnimationPlayCallbackProxyV2::CreatePlayAnimationProxyObject(FWidgetAnimationHandle& Result, class UUserWidget* Widget, UWidgetAnimation* InAnimation, EUMGSequencePlayModeV2::Type PlayMode, float PlaybackSpeed)
{
	UWidgetAnimationPlayCallbackProxyV2* Proxy = NewObject<UWidgetAnimationPlayCallbackProxyV2>();
	Proxy->SetFlags(RF_StrongRefOnFrame);
	Proxy->ExecutePlayAnimation(Widget, InAnimation, PlayMode, PlaybackSpeed);
	Result = Proxy->WidgetAnimationHandle;
	return Proxy;
}

UWidgetAnimationPlayCallbackProxyV2* UWidgetAnimationPlayCallbackProxyV2::CreatePlayAnimationProxyObjectDeferred()
{
	UWidgetAnimationPlayCallbackProxyV2* Proxy = NewObject<UWidgetAnimationPlayCallbackProxyV2>();
	Proxy->SetFlags(RF_StrongRefOnFrame);
	return Proxy;
}

void UWidgetAnimationPlayCallbackProxyV2::PlayAnimationProxyObjectDeferred(
	UWidgetAnimationPlayCallbackProxyV2* Proxy, FWidgetAnimationHandle& Result, class UUserWidget* Widget,
	class UWidgetAnimation* InAnimation, EUMGSequencePlayModeV2::Type PlayMode, float PlaybackSpeed)
{
	Proxy->ExecutePlayAnimation(Widget, InAnimation, PlayMode, PlaybackSpeed);
	Result = Proxy->WidgetAnimationHandle;
}

void UWidgetAnimationPlayCallbackProxyV2::PlayAnimationDuoProxyObjectDeferred(
	UWidgetAnimationPlayCallbackProxyV2* Proxy, FWidgetAnimationHandle& Result, class UUserWidget* Widget,
	class UWidgetAnimation* InForwardAnimation, class UWidgetAnimation* InReverseAnimation,
	EUMGSequencePlayModeV2::Type PlayMode, float PlaybackSpeed)
{
	Proxy->ExecutePlayDuoAnimation(Widget, InForwardAnimation, InReverseAnimation, PlayMode, PlaybackSpeed);
	Result = Proxy->WidgetAnimationHandle;
}

UWidgetAnimationPlayCallbackProxyV2* UWidgetAnimationPlayCallbackProxyV2::CreatePlayAnimationDuoProxyObject(
	FWidgetAnimationHandle& Result, UUserWidget* Widget, UWidgetAnimation* InForwardAnimation,
	UWidgetAnimation* InReverseAnimation, EUMGSequencePlayModeV2::Type PlayMode, float PlaybackSpeed)
{
	UWidgetAnimationPlayCallbackProxyV2* Proxy = NewObject<UWidgetAnimationPlayCallbackProxyV2>();
	Proxy->SetFlags(RF_StrongRefOnFrame);
	Proxy->ExecutePlayDuoAnimation(Widget, InForwardAnimation, InReverseAnimation, PlayMode, PlaybackSpeed);
	Result = Proxy->WidgetAnimationHandle;
	return Proxy;
}

UWidgetAnimationPlayCallbackProxyV2::UWidgetAnimationPlayCallbackProxyV2(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UWidgetAnimationPlayCallbackProxyV2::ExecutePlayAnimation(class UUserWidget* Widget, UWidgetAnimation* InAnimation, EUMGSequencePlayModeV2::Type PlayMode, float PlaybackSpeed)
{
	if (!Widget || !InAnimation)
	{
		return;
	}

	// This is the main change away from Epic's - will reverse the animation in place instead of from the end
	// Note that current time is always 0.f if no animation is playing even if it is remaining at the end!
	float StartAtTime = 0.f;
	if (Widget->IsAnimationPlaying(InAnimation))
	{
		const float CurrentTime = Widget->GetAnimationCurrentTime(InAnimation);

		StartAtTime = PlayMode == EUMGSequencePlayModeV2::Reverse ?
		InAnimation->GetEndTime() - CurrentTime : CurrentTime;
	}

	const bool bForward = PlayMode != EUMGSequencePlayModeV2::Reverse;
	WidgetAnimationHandle = Widget->PlayAnimation(InAnimation, StartAtTime, 1, bForward ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse, PlaybackSpeed);
	FWidgetAnimationState* State = WidgetAnimationHandle.GetAnimationState();
	if (State)
	{
		State->GetOnWidgetAnimationFinished().AddUObject(this, &UWidgetAnimationPlayCallbackProxyV2::OnSequenceFinished);
		BeginProgressTracking(Widget, InAnimation, bForward);
	}
}

void UWidgetAnimationPlayCallbackProxyV2::ExecutePlayDuoAnimation(UUserWidget* Widget,
	UWidgetAnimation* InForwardAnimation, UWidgetAnimation* InReverseAnimation, EUMGSequencePlayModeV2::Type PlayMode,
	float PlaybackSpeed)
{
	if (!Widget || !InForwardAnimation || !InReverseAnimation)
	{
		return;
	}

	const bool bForward = PlayMode != EUMGSequencePlayModeV2::Reverse;
	UWidgetAnimation* AnimToPlay = bForward ? InForwardAnimation : InReverseAnimation;
	const UWidgetAnimation* AnimToCheck = bForward ? InReverseAnimation : InForwardAnimation;

	// This is the main change away from Epic's - will reverse the animation in place instead of from the end
	// Note that current time is always 0.f if no animation is playing even if it is remaining at the end!
	float StartAtTime = 0.f;
	if (Widget->IsAnimationPlaying(AnimToCheck))
	{
		const float CurrentTime = Widget->GetAnimationCurrentTime(AnimToCheck);

		StartAtTime = PlayMode == EUMGSequencePlayModeV2::Reverse ?
		AnimToCheck->GetEndTime() - CurrentTime : CurrentTime;

		Widget->StopAnimation(AnimToCheck);
	}

	// Always play forward, because the animations aren't actually being reversed
	WidgetAnimationHandle = Widget->PlayAnimation(AnimToPlay, StartAtTime, 1, EUMGSequencePlayMode::Forward, PlaybackSpeed);
	FWidgetAnimationState* State = WidgetAnimationHandle.GetAnimationState();
	if (State)
	{
		State->GetOnWidgetAnimationFinished().AddUObject(this, &UWidgetAnimationPlayCallbackProxyV2::OnSequenceFinished);
		BeginProgressTracking(Widget, AnimToPlay, true);
	}
}

void UWidgetAnimationPlayCallbackProxyV2::OnSequenceFinished(FWidgetAnimationState& State)
{
	State.GetOnWidgetAnimationFinished().Remove(OnFinishedHandle);

	StopProgressTracking();

	// We delay the Finish broadcast to next frame.
	FTSTicker::FDelegateHandle TickerHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UWidgetAnimationPlayCallbackProxyV2::OnAnimationFinished));
}


bool UWidgetAnimationPlayCallbackProxyV2::OnAnimationFinished(float /*DeltaTime*/)
{
	if (bReachedEnd)
	{
		Finished.Broadcast();
	}
	else
	{
		Interrupted.Broadcast();
	}

	// Returning false, disable the ticker.
	return false;
}

void UWidgetAnimationPlayCallbackProxyV2::BeginProgressTracking(UUserWidget* Widget, UWidgetAnimation* Anim, bool bForward)
{
	if (!Widget || !Anim)
	{
		return;
	}

	TrackedWidget = Widget;
	TrackedAnimation = Anim;
	bTrackForward = bForward;
	ProgressBoundaryTime = bForward ? Anim->GetEndTime() : 0.f;
	LastSampleTime = Widget->GetAnimationCurrentTime(Anim);
	bReachedEnd = FMath::IsNearlyEqual(LastSampleTime, ProgressBoundaryTime);

	ProgressTickerHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UWidgetAnimationPlayCallbackProxyV2::SampleProgress));
}

bool UWidgetAnimationPlayCallbackProxyV2::SampleProgress(float /*DeltaTime*/)
{
	UUserWidget* Widget = TrackedWidget.Get();
	UWidgetAnimation* Anim = TrackedAnimation.Get();
	if (!Widget || !Anim || !Widget->IsAnimationPlaying(Anim))
	{
		return false;
	}

	const float CurrentTime = Widget->GetAnimationCurrentTime(Anim);
	const float Step = FMath::Abs(CurrentTime - LastSampleTime);
	const float Remaining = FMath::Abs(ProgressBoundaryTime - CurrentTime);
	if (Remaining <= Step * 1.5f + UE_KINDA_SMALL_NUMBER)
	{
		bReachedEnd = true;
	}
	LastSampleTime = CurrentTime;
	return true;
}

void UWidgetAnimationPlayCallbackProxyV2::StopProgressTracking()
{
	if (ProgressTickerHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(ProgressTickerHandle);
		ProgressTickerHandle.Reset();
	}
}
#undef LOCTEXT_NAMESPACE

