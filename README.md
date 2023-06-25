# PlayWidgetAnimation
This removes "Start at Time" from Play Animation with Finished event, and computes it automatically for the purpose of reversing an animation part way through

## Do I need this?
If you don't need a Finished event or the duo node, you can just use the engine's `UUserWidget::PlayAnimationForward` and `UUserWidget::PlayAnimationReverse` nodes.

## Example Usage

![example usage](https://github.com/Vaei/PlayWidgetAnimation/blob/examples/Example1.png)

## Duo Node Usage

This node can play different animations that have the same play lengths - you must check this yourself - to reverse in place with.

![example usage](https://github.com/Vaei/PlayWidgetAnimation/blob/examples/ExampleDuo0.png)

## UE5 Default Comparison

This is how it used to look using only engine functionality, although this is an incomplete example, you also have to factor in if it's currently playing or not as well so it only gets messier

![engine equivalent](https://github.com/Vaei/PlayWidgetAnimation/blob/examples/Example0.png)

## Changelog
### 1.0.0.3
Fixed loading phase

### 1.0.0.2
Fixed bug with duo node not stopping previous anim

### 1.0.0.1
Added duo node

### 1.0.0.0
Initial Release