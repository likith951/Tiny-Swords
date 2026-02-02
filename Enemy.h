#pragma once
#include "Header.h"
#include "raylib.h"
#include <raymath.h>

class Enemy {
	private:
	Texture2D texture;
	void UpdateState();
	
	
	float speed;
	int frameWidth = 8;
	int Hdir = 1;
	int currentFrame = 0;
public:
	Texture2D IdleTex;
	Texture2D RunTex;
	Texture2D AttackTex;
	State Estate;
	Vector2 direction = { 0,0 };
	Vector2 pos;
	Vector2 playerPos;
	bool ALIVE = true;
	int Health = 100;
	int x=0;
	bool iscolliding = false;//wth player
	bool iscollidingE = false;//wth enemy
	float Ecooldown = 0.25f;
	float lastCollodied = 0;
	float attackCooldown = 1.5f; // Cooldown time in seconds
	float lastAttackTime = 0.0f; // Time of the last attack
	bool canAttack = false; //Flag to check if the enemy can attack based on cooldown

	bool canGetHit = true; // Flag to check if the enemy can take damage
	float invincibilityDuration = 1.0f; // Duration of invincibility in seconds
	float lastHitTime = 0.0f; // Time of the last hit taken

	Enemy(Texture2D Tex, Vector2 pos, float speed);
	void Update();
	void Draw();
	Rectangle getAnimatedFrame(int frameRate, int winfps);
	void takeDamage(float damageDelt);
	Rectangle getHitBox() {
		return { pos.x + ((texture.width / frameWidth) / 3), pos.y + texture.height / 2, (float(texture.width) / float(frameWidth)) / 3, float(texture.height) / 5 };
	}
	Rectangle nxtFrameBox()
	{
		int dirY = (direction.y > 0) ? 1 : -1;
		dirY = (direction.x > 0) ? -dirY : dirY;
		if (direction.x == 0 && direction.y != 0)
		{
			return { pos.x + ((texture.width / frameWidth) / 3), pos.y + (-Hdir * 10 * speed * GetFrameTime()) + texture.height / 2, (float(texture.width) / float(frameWidth)) / 3, float(texture.height) / 5 };
		}
		if (direction.y == 0 && direction.x != 0)
		{
			return{ pos.x + (Hdir * 10 * speed * GetFrameTime()) + ((texture.width / frameWidth) / 3), pos.y + texture.height / 2, (float(texture.width) / float(frameWidth)) / 2.5f, float(texture.height) / 5 };
		}

		return { pos.x + (Hdir * 10 * speed * GetFrameTime()) + ((texture.width / frameWidth) / 3), pos.y + (-Hdir * dirY * 10 * speed * GetFrameTime()) + texture.height / 2, (float(texture.width) / float(frameWidth)) / 2.5f, float(texture.height) / 5 };
	}
	Rectangle getHurtBox()
	{
		Rectangle rightrec = { pos.x + ((texture.width / frameWidth) / 1.5f),pos.y + float(texture.height / 4), float(texture.height) / 5 , (float(texture.width) / float(frameWidth) / 2) };
		Rectangle leftrec = { pos.x + ((texture.width / frameWidth) / 7),pos.y + float(texture.height / 4), float(texture.height) / 5 , (float(texture.width) / float(frameWidth) / 2) };


		if (direction.x > 0)
		{
			return rightrec;
		}
		else if (direction.x < 0)
		{
			return leftrec;
		}
		else
		{
			if (Hdir == 1)
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

Enemy::Enemy(Texture2D Tex, Vector2 pos, float speed)
{
	this->texture = Tex;
	this->pos = pos;
	this->speed = speed;
}
Rectangle Enemy::getAnimatedFrame(int frameRate, int winfps)
{
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
			if (Estate == State::Attacking)
			{
				this->lastAttackTime = GetTime();
				Estate = State::Idle;
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
void Enemy::Draw()
{
	if (ALIVE)
	{
		DrawTextureRec(texture, getAnimatedFrame(8, 120), pos, WHITE);

	}
	
}

void Enemy::UpdateState()
{
	switch (Estate)
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
	case State::Dying:
		ALIVE = false;
	default:
		break;
	}
}
void Enemy::takeDamage(float damageDelt)
{
	if (canGetHit)
	{
		canGetHit = false;
		lastHitTime = GetTime();
		Health -= damageDelt;
		if (Health <= 0)
		{
			canGetHit = false;
			Estate = State::Dying;
		}
	}
	
	
}
void Enemy::Update()
{
	//Chasing Code to be written

	if (ALIVE)
	{
		currentFrame++;
		UpdateState();
		direction = Vector2Subtract(playerPos, pos);
		direction = Vector2Normalize(direction);
		if (lastAttackTime + attackCooldown <= GetTime()) canAttack = true;
		if (playerPos != pos && !iscolliding&&!iscollidingE&&(lastCollodied+Ecooldown<=GetTime()))
		{
			pos = Vector2Add(pos, Vector2Scale(direction, speed * GetFrameTime()));
			Estate = State::Running;
		}
		else if(iscolliding && canAttack)
		{
			Estate = State::Attacking;
		}
		else if (Estate!=State::Attacking)
		{
			Estate = State::Idle;
		}
		if (direction.y > 0)
			Hdir = +1;
		else if (direction.y < 0)
			Hdir = -1;
		if(direction.x>0)
			Hdir = 1;
		else if(direction.x<0)
			Hdir =- 1;
		
		canGetHit = (lastHitTime + invincibilityDuration <= GetTime()) ? true : false;
	}
	
	
}