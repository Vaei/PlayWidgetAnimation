# PlayWidgetAnimation
This removes "Start at Time" from Play Animation with Finished event, and computes it automatically for the purpose of reversing an animation part way through

If you don't need a finish event or the duo node, you can just use the engine's `UUserWidget::PlayAnimationForward` and `UUserWidget::PlayAnimationReverse` nodes.

## Example Usage

![example usage](https://github.com/Vaei/PlayWidgetAnimation/blob/examples/Example1.png)

## Duo Node Usage

This node can play different animations that have the same play lengths - you must check this yourself - to reverse in place with.

![example usage](https://github.com/Vaei/PlayWidgetAnimation/blob/examples/ExampleDuo0.png)

## UE5 Default Comparison

This is how it used to look using only engine functionality, although this is an incomplete example, you also have to factor in if it's currently playing or not as well so it only gets messier

![engine equivalent](https://github.com/Vaei/PlayWidgetAnimation/blob/examples/Example0.png)
