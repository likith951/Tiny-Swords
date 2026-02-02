#pragma once
#include "raylib.h"
#include <raymath.h>
#include<vector>

enum class State {
	Idle,
	Running,
	Attacking,
	Dying 
};

class Player {
private:
	Texture2D texture;
	void UpdateState();
	Vector2 direction = { 0,0 };
	float speed;
	int frameWidth = 8;
	int Hdir = 1;
	int Vdir = 1;
	
	
public:
	Vector2 pos;
	Texture2D IdleTex;
	Texture2D RunTex;
	Texture2D AttackTex;
	Texture2D GuardTex;
	State Pstate;
	int Health = 100;
	int ALIVE = true;
	bool isColliding = false;	
	int x=0;
	float attackCooldown = 0.75f; // Cooldown time in seconds
	float lastAttackTime = 0.0f; // Time of the last attack
	bool canAttack = false; //Flag to check if the player can attack based on cooldown
	bool canGetHit = false;
	float invincibility = 1.0f;
	float lastHitTime;
	Player(Texture2D Tex, Vector2 pos, float speed);
	void Update();
	void Draw(int& currentFrame, int& x);
	Rectangle getAnimatedFrame(int frameRate, int winfps, int& currentFrame);
	void takeHit(float damageDelt);

	Rectangle getHitBox() {
		return { pos.x + ((texture.width / frameWidth) / 3), pos.y + texture.height / 2, (float(texture.width) /float(frameWidth))/3, float(texture.height)/5 };
	}
	Rectangle nxtFrameBox()
	{
		int dirY = (direction.y>0)? 1 : -1;
		dirY = (direction.x > 0) ? -dirY : dirY;
		if(direction.x==0&&direction.y!=0)
		{
			return { pos.x + ((texture.width / frameWidth) / 3), pos.y + (-Hdir * 10 * speed*GetFrameTime()) + texture.height / 2, (float(texture.width) / float(frameWidth)) / 3, float(texture.height) / 5 };
		}
		if (direction.y == 0 && direction.x != 0)
		{
			return{ pos.x + (Hdir * 10 * speed * GetFrameTime()) + ((texture.width / frameWidth) / 3), pos.y  + texture.height / 2, (float(texture.width) / float(frameWidth)) / 4, float(texture.height) / 5 };
		}
		
		return { pos.x+(Hdir*10*speed*GetFrameTime()) + ((texture.width / frameWidth) / 3), pos.y + (-Hdir * dirY * 10 * speed * GetFrameTime()) + texture.height / 2, (float(texture.width) / float(frameWidth)) / 4, float(texture.height) / 5 };
	}
	Rectangle getHurtBox()
	{
		Rectangle rightrec= { pos.x + ((texture.width / frameWidth) / 1.5f),pos.y + float(texture.height / 4), float(texture.height) / 5 , (float(texture.width) / float(frameWidth) / 2) };
		Rectangle leftrec= { pos.x + ((texture.width / frameWidth) / 7),pos.y + float(texture.height / 4), float(texture.height) / 5 , (float(texture.width) / float(frameWidth) / 2) };
		Rectangle bottomrec = { pos.x + ((texture.width / frameWidth) / 4),pos.y + float(texture.height / 1.5), (float(texture.width) / float(frameWidth) / 2) , float(texture.height) / 5 };
		
		if (direction.x>0)
		{
			return rightrec;
		}
		else if(direction.x<0)
		{
			return leftrec;
		}
		else if(direction.y>0)
		{
			return bottomrec;
		}
		else
		{
			if(Hdir==1)
			{
				return rightrec;
			}
			else
			{
				return leftrec;
			}
		}

	}
};




Player::Player(Texture2D Tex, Vector2 pos, float speed) {

	texture = Tex;
	this->Pstate = State::Idle;
	this->pos = pos;
	this->speed = speed;
}
Rectangle Player::getAnimatedFrame(int frameRate, int winfps, int& currentFrame) {
	Rectangle frameRec;
	if (ALIVE)
	{
		if (currentFrame >= winfps / frameRate)
		{
			currentFrame = 0;
			x++;
		}
		if (x >= frameWidth)
		{
			if (Pstate == State::Attacking)
			{
				this->lastAttackTime = GetTime();
				Pstate = State::Idle;
			}
			x = 0;
		}


		frameRec.x = (x) * (texture.width / frameWidth);
		frameRec.y = 0;
		frameRec.width = Hdir * texture.width / frameWidth;
		frameRec.height = texture.height;
	}
	return frameRec;
}




void Player::Draw(int &currentFrame,int &x) {
	DrawTextureRec(texture, getAnimatedFrame( 10, 120, currentFrame), pos, WHITE);
}
void Player::takeHit(float damageDelt)
{
	if (canGetHit)
	{
		canGetHit = false;
		lastHitTime = GetTime();
		Health -= damageDelt;
		if (Health <=0)
		{
			ALIVE = false;
			Health = 0;
		}
	}

}	
void Player::UpdateState() {
	// Update player state based on input or other conditions
	switch (Pstate)
	{
	case State::Idle:
		texture = IdleTex;
		frameWidth = 8;
		direction = { 0,0 };
		break;
	case State::Running:
		texture = RunTex;
		frameWidth = 6;
		break;
	case State::Attacking:
		texture = AttackTex;
		canAttack = false;
		frameWidth = 4;
		break;
	default:
		break;
	}
}
void Player::Update() {
	// Update player state and position here
	if (ALIVE)
	{
		if (IsKeyDown(KEY_W))
		{
			Hdir = 1;
			direction.y = -1;
		}
		else if (IsKeyDown(KEY_S))
		{
			Hdir = -1;
			direction.y = 1;
		}
		else direction.y = 0;
		if (IsKeyDown(KEY_A))
		{
			Hdir = -1;
			direction.x = -1;
		}
		else if (IsKeyDown(KEY_D))
		{
			Hdir = 1;
			direction.x = 1;
		}
		else direction.x = 0;
		if (Pstate != State::Attacking && !isColliding)
		{
			direction = Vector2Normalize(direction);
			direction = Vector2Scale(direction, speed);
		}
		pos = Vector2Add(pos, Vector2Scale(direction, GetFrameTime()));

		if (lastAttackTime + attackCooldown <= GetTime()) canAttack = true;
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && Pstate != State::Attacking && canAttack)
		{
			x = 0;
			Pstate = State::Attacking;
		}
		if (!IsKeyDown(KEY_W) && !IsKeyDown(KEY_S) &&
			!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D) && Pstate != State::Attacking)
		{
			Pstate = State::Idle;
		}
		else if (Pstate != State::Attacking)
		{
			Pstate = State::Running;
		}
		canGetHit = (lastHitTime + invincibility <= GetTime()) ? true : false;
		UpdateState(); 
	}

}	


/*
	Game Stat:TO BE IMPLEMENTED
	Idle Animation:8 frames
	Running Animation:6 frames
	Attacking Animation:4 frames
	Guarding Animation:To be Added 

	Idle State: When no movement keys are pressed, the player is idle.
	Running State: When movement keys (WASD) are pressed, the player runs in the corresponding direction.
	Attacking State: When the left mouse button is clicked, the player performs an attack animation.

	Transitions:
	- From Idle to Running: Press any movement key.
	- From Running to Idle: Release all movement keys.
	- From any state to Attacking: Click the left mouse button.
	-From Attacking to Idle: After the attack animation completes, return to Idle state.(Since it is only one frame force changing to idle,should be fine)


*/