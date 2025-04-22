Pong Game
This is a simple Pong-style game made in C++, built on top of the BUas Template, which is in turn built on an SDK framework.
The game is structured around a Unity-like engine, where each object (like paddles, the ball, and power-ups) is a GameObject with components such as position, collider, trigger, scripts, and renderers.
Everything is handled using a component-based architecture with a custom event system and simple physics.


How to Play
Try to hit the ball with your paddle and score against your opponent. Don't let the ball pass your paddle!

Power-Ups
Power-ups float on the field and activate when the ball gets shot through them. Their effects are:
	Blue – Speeds up the player who hit he power up.
	Yellow – Makes the paddle of the player who hit the power-up larger.
	Green - Instantly gives a point to the player who hit the power-up.
	Red - removes a point from the player who didn't hit the power-up.



Controls
Single Player (Right Paddle)
	Mouse: Paddle follows your mouse vertically
	Keyboard: W/S or Up/Down arrows

Multiplayer
	Left Player: W/S
	Right Player: Up/Down

Press ESC to quit the game at any time.