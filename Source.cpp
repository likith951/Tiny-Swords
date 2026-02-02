#include "Header.h"
#include"Enemy.h"
#include"GameManager.h"
#include<string>
#include <vector>
#include<random>

enum class GameState {
	Start,
	Game,
	End
};

std::vector<Enemy> enemies;
void EnemyRenderBeginPlay()
{
	for (Enemy& enemy : enemies)
	{
		enemy.IdleTex = LoadTexture("C:\\Projects\\C++\\Raylib Projects\\TinySword\\Enemy\\Warrior_Idle.png");
		enemy.AttackTex = LoadTexture("C:\\Projects\\C++\\Raylib Projects\\TinySword\\Enemy\\Warrior_Attack2.png");
		enemy.RunTex = LoadTexture("C:\\Projects\\C++\\Raylib Projects\\TinySword\\Enemy\\Warrior_Run.png");

	}
}
void spawnEnemies(Texture2D enemyTex,Player &player)
{

	for (int i = enemies.size()-1; i >= 0; i--)
	{
		if (!enemies[i].ALIVE)
		{
			enemies.erase(enemies.begin() + i);
		}
		
	}
	if (enemies.empty())
	{
		WaveNo++;
		for (int i = 0; i < 2*WaveNo; i++)
		{

			float randX=rand()%GetScreenWidth();
			float randY=rand()%GetScreenHeight();
			enemies.emplace_back(enemyTex, Vector2{ randX,randY }, 100.0f);

		}
		if (WaveNo % 5 == 0)
		{
			player.Health = 100;
		}
		EnemyRenderBeginPlay();
	}


}

void EnemyRender(Player &player)
{
	//collision check bwt player
	for (Enemy& enemy : enemies)
	{
		enemy.Draw();
		enemy.Update();
		enemy.playerPos = player.pos;
	}
	std::vector<Enemy*> takingdmg = Check_Hit(&player, enemies);
	std::vector<Enemy*> canattack = Check_Enemy_Hit(enemies, &player);
	for (Enemy* enemy : takingdmg)
	{
		enemy->takeDamage(50.f);
	}
	Check_Enemy_Collosion(enemies,&player);
	for (Enemy* enemy : canattack)
	{
		if (player.canGetHit)
		{
			player.takeHit(5.0f);
		}
	}
	E2Ecollision(enemies);
}
int main() {
	
	InitWindow(1950, 1080, "Raylib Window");
	Texture2D enemyTex = LoadTexture("C:\\Projects\\C++\\Raylib Projects\\TinySword\\Enemy\\Warrior_Idle.png");
	SetTargetFPS(120);
	Texture2D tex= LoadTexture("C:\\Projects\\C++\\Raylib Projects\\TinySword\\Warrior_Idle.png");
	Player player(tex,{100,100}, 300.0f);
	player.IdleTex = LoadTexture("C:\\Projects\\C++\\Raylib Projects\\TinySword\\Warrior_Idle.png");
	player.AttackTex = LoadTexture("C:\\Projects\\C++\\Raylib Projects\\TinySword\\Warrior_Attack2.png");
	player.RunTex = LoadTexture("C:\\Projects\\C++\\Raylib Projects\\TinySword\\Warrior_Run.png");
	player.GuardTex = LoadTexture("C:\\Projects\\C++\\Raylib Projects\\TinySword\\Warrior_Guard.png");

	//Enemy
	srand(time(NULL));
	
	

	
	int currentFrame = 0;
	int currentframe = 0;
	int x = 0;
	int y = 0;
	GameState GameState;
	GameState = GameState::Start;
	while (!WindowShouldClose()) {
		switch (GameState)
		{
		case GameState::Start:
		{
			BeginDrawing();
			ClearBackground(RAYWHITE);
			DrawText("Press Enter To Start [Enter]", GetScreenWidth() / 2, GetScreenHeight() / 2, 50, LIGHTGRAY);
			if (IsKeyDown(KEY_ENTER))
			{
				GameState = GameState::Game;
			}

			EndDrawing();
			break;
		}
		
		case GameState::Game:
		{
			int Fps = GetFPS();
			currentFrame++;
			currentframe++;
			spawnEnemies(tex, player);


			EnemyRender(player);
			std::string fpsString = std::to_string(Fps);
			const char* fpsCStr = fpsString.c_str();
			BeginDrawing();
			ClearBackground(BEIGE);
			DrawText(fpsCStr, 0, 0, 20, LIGHTGRAY);
			player.isColliding = Check_Collision(&player, enemies);
			player.Update();
			player.Draw(currentFrame, x);
			DrawText(std::to_string(player.Health).c_str(), 0, 110, 20, RED);
			DrawText(std::to_string(WaveNo).c_str(), GetScreenWidth() / 2, 0, 50, LIGHTGRAY);
			if (!player.ALIVE)
			{
				GameState=GameState::End;
			}
			if (player.Health <= 30)
			{
				DrawText("Heal will be granted after every 5 rounds", 0, GetScreenHeight()-30, 20, RED);
			}

			EndDrawing();
			break;
		}
		case GameState::End:
		{
			BeginDrawing();
			DrawText("Game", GetScreenWidth() / 2, GetScreenHeight() / 2, 100, LIGHTGRAY);
			DrawText("Over", GetScreenWidth() / 2+30, GetScreenHeight() / 2+100, 100, LIGHTGRAY);

			EndDrawing();
			break;
		}
			
		default:
			break;
		}

	}
	CloseWindow();
	return 0;
}




//DBUG THIS LATER
/*
	
*/