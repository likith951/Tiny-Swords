#include "Header.h"
#include"Enemy.h"
#include"GameManager.h"
#include <raylib.h>
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
		enemy.IdleTex = LoadTexture("Enemy/Warrior_Idle.png");
		enemy.AttackTex = LoadTexture("Enemy/Warrior_Attack2.png");
		enemy.RunTex = LoadTexture("Enemy/Warrior_Run.png");

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
		if (WaveNo % 3 == 0)
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
	int monitor = GetCurrentMonitor();
    SetWindowSize(GetMonitorWidth(monitor),GetMonitorHeight(monitor));
    ToggleFullscreen();
	Texture2D enemyTex = LoadTexture("Enemy/Warrior_Idle.png");
	SetTargetFPS(120);
	Texture2D tex= LoadTexture("player/Warrior_Idle.png");
	Player player(tex,{100,100}, 300.0f);
	player.IdleTex = LoadTexture("player/Warrior_Idle.png");
	player.AttackTex = LoadTexture("player/Warrior_Attack2.png");
	player.RunTex = LoadTexture("player/Warrior_Run.png");
	player.GuardTex = LoadTexture("player/Warrior_Guard.png");

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
			DrawText(fpsCStr, 0, 0, 30, GREEN);
			player.isColliding = Check_Collision(&player, enemies);
			player.Update();
			player.Draw(currentFrame, x);
			DrawText(std::to_string(player.Health).c_str(), GetScreenWidth()-50,0, 30, RED);
			DrawText(std::to_string(WaveNo).c_str(), GetScreenWidth() / 2, 0, 50, LIGHTGRAY);
			if (!player.ALIVE)
			{
				GameState=GameState::End;
			}
			if (player.Health <= 30)
			{
				DrawText("Warrior heals to full HP after every 3 rounds", 0, GetScreenWidth()-30, 20, RED);
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
