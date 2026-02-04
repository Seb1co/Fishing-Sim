#pragma once

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Shop.h"
#include "Event.h"
#include <vector>
#include <utility>


int closestFive(int value);
class Game
{
private:
	 bool show_inventory = false;
	 bool show_shop		 = false;

	 Shop shop;

	 sf::RenderWindow& main_window;
	 int Seed = 0;

	 Player player;

	 std::vector<std::vector<int>> GameMapSkeleton;
	 sf::Texture tileMapAtlas;

	 std::unique_ptr<Event> event = nullptr;
	 float wait_time = 0.f;
	 float event_time = 0.f;
	 sf::Clock event_clock;

	 void GenerateMap();
	 void Loop();
	 void Listen();
	 void Render();
	 void Save();
	 void Animantion(float duration);
public:
	Game(sf::RenderWindow& main_wind);
	Game(sf::RenderWindow& main_wind, int seed, int player_x, int player_y, float balance, int grip, int reel, int head, int bait, const std::vector<std::pair<int, float>>& fishes);
	/// <summary>
	/// Starts the game loop
	/// </summary>
	void Start();
	/// <summary>
	/// Shows the game map
	/// </summary>
	void showMap();
};