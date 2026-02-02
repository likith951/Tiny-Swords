#pragma once
#include "raylib.h"
#include "Header.h"
#include<algorithm>
#include<iostream>
#include "Enemy.h"

int WaveNo = 0;

bool Check_Collision(Player *player,std::vector<Enemy>&enemies)
{
	bool iscolliding = false;
	for (Enemy &enemy : enemies)
	{
		if (CheckCollisionRecs(player->nxtFrameBox(), enemy.getHitBox()))
		{
			iscolliding=true;
		}
	}
	return iscolliding;
		 
}

std::vector<Enemy*> Check_Hit(Player* player,std::vector<Enemy> &enemies)
{
	std::vector<Enemy*> EnemyCanGetHit;
	for (Enemy &enemy : enemies)
	{
		if(CheckCollisionRecs(player->getHurtBox(), enemy.getHitBox()) && player->Pstate == State::Attacking) EnemyCanGetHit.push_back(&enemy);
	}
	return  EnemyCanGetHit;
}
 void Check_Enemy_Collosion(std::vector<Enemy>& enemies, Player* player)
{
	 std::vector<Enemy*> EnemyColliding;
	 for (Enemy& enemy : enemies)
	 {
		 if (CheckCollisionRecs(enemy.nxtFrameBox(), player->getHitBox())) enemy.iscolliding = true;
		 else enemy.iscolliding = false;

	 }
	
}
std::vector<Enemy*> Check_Enemy_Hit(std::vector<Enemy>& enemies, Player* player)
{
	std::vector<Enemy*> EnemyCanHit;
	for (Enemy& enemy : enemies)
	{
		if (CheckCollisionRecs(enemy.getHurtBox(), player->getHitBox()) && enemy.Estate == State::Attacking) EnemyCanHit.push_back(&enemy);
	}
	return EnemyCanHit;
}

void E2Ecollision(std::vector<Enemy>& enemies)
{
	for (Enemy& e : enemies)
		e.iscollidingE = false;

	for (int i = 0; i < enemies.size(); i++)
	{
		for (int j = i + 1; j < enemies.size(); j++)

			if (CheckCollisionRecs(enemies[i].getHitBox(), enemies[j].nxtFrameBox()) || CheckCollisionRecs(enemies[j].getHitBox(), enemies[i].nxtFrameBox()))
			{

				float distI = Vector2Length(Vector2Subtract(enemies[i].playerPos, enemies[i].pos));
				float distJ = Vector2Length(Vector2Subtract(enemies[j].playerPos, enemies[j].pos));
				std::cout << i << j;
				if (distI >= distJ)
				{
					enemies[i].iscollidingE = true;
					enemies[i].lastCollodied = GetTime();
					
				}

				else
				{
					enemies[j].iscollidingE = true;
					enemies[j].lastCollodied= GetTime();


				}

			}

	}
}




/*

	for (Enemy& e : enemies)
		e.iscollidingE = false;
	for (int i =0;i<enemies.size();i++)
	{
		for (int j= i+1;j<enemies.size();j++)

			if (CheckCollisionRecs(enemies[i].getHitBox(), enemies[j].nxtFrameBox())||CheckCollisionRecs(enemies[j].getHitBox(), enemies[i].nxtFrameBox()))
			{

				float distI = Vector2Length(Vector2Subtract(enemies[i].playerPos,enemies[i].pos));
				float distJ = Vector2Length(Vector2Subtract(enemies[j].playerPos, enemies[j].pos));

				if (distI > distJ)
				{
					enemies[i].iscollidingE = true;
				}

				else
				{
					enemies[j].iscollidingE = true;
				}

			}

	}*/


