#pragma once

#include "Item.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include <variant>
#include <utility>

class Player
{
private:
	sf::Texture player_texture;
	int currentState = 0;
	int screen_coords[2] = {0,0}; // [0] = screen_width, [1] = screen_height
	using Items = std::variant<Rod, Bait, Fish>;
	float balance = 0.0f;
	int coords[2] = { 0,0 }; // [0] = x, [1] = y
public:
	std::vector<Items> inventory;

    Player(int screen_width,int screen_height);
	Player(int screen_width, int screen_height, float balance, int grip, int reel, int head,int bait,const std::vector<std::pair<int,float>>& fishes);
	Player() = default;
    sf::Sprite Load();
	std::vector<int> getCoordinates();
	void setCoordinates(int x, int y);
	bool checkIfFullInventory();
	int getState();
	void setState(int state);
	void addBalance(float amount);
	float getBalance();
};
namespace Data
{
	class Player {
	public:
		enum States {
			UP,						// 0
			RIGHT,					// 1
			DOWN,					// 2
			LEFT,					// 3
			RIGHT_UP,				// 4
			RIGHT_DOWN,				// 5
			LEFT_DOWN,				// 6
			LEFT_UP,				// 7

			THROW_ANIMATION_1_UP,			// 8
			THROW_ANIMATION_1_RIGHT,		// 9
			THROW_ANIMATION_1_DOWN,		// 10
			THROW_ANIMATION_1_LEFT,		// 11
			THROW_ANIMATION_1_RIGHT_UP,		// 12
			THROW_ANIMATION_1_RIGHT_DOWN,	// 13
			THROW_ANIMATION_1_LEFT_DOWN,	// 14
			THROW_ANIMATION_1_LEFT_UP,		// 15

			THROW_ANIMATION_2_UP,			// 16
			THROW_ANIMATION_2_RIGHT,		// 17
			THROW_ANIMATION_2_DOWN,		// 18
			THROW_ANIMATION_2_LEFT,		// 19
			THROW_ANIMATION_2_RIGHT_UP,		// 20
			THROW_ANIMATION_2_RIGHT_DOWN,	// 21
			THROW_ANIMATION_2_LEFT_DOWN,	// 22
			THROW_ANIMATION_2_LEFT_UP,		// 23

			THROW_ANIMATION_3_UP,			// 24
			THROW_ANIMATION_3_RIGHT,		// 25
			THROW_ANIMATION_3_DOWN,		// 26
			THROW_ANIMATION_3_LEFT,		// 27
			THROW_ANIMATION_3_RIGHT_UP,		// 28
			THROW_ANIMATION_3_RIGHT_DOWN,	// 29
			THROW_ANIMATION_3_LEFT_DOWN,	// 30
			THROW_ANIMATION_3_LEFT_UP,		// 31

			REEL_ANIMATION_1_UP,			// 32
			REEL_ANIMATION_1_RIGHT,		// 33
			REEL_ANIMATION_1_DOWN,		// 34
			REEL_ANIMATION_1_LEFT,		// 35
			REEL_ANIMATION_1_RIGHT_UP,		// 36
			REEL_ANIMATION_1_RIGHT_DOWN,	// 37
			REEL_ANIMATION_1_LEFT_DOWN,	// 38
			REEL_ANIMATION_1_LEFT_UP,		// 39

			REEL_ANIMATION_2_UP,			// 40
			REEL_ANIMATION_2_RIGHT,		// 41
			REEL_ANIMATION_2_DOWN,		// 42
			REEL_ANIMATION_2_LEFT,		// 43
			REEL_ANIMATION_2_RIGHT_UP,		// 44
			REEL_ANIMATION_2_RIGHT_DOWN,	// 45
			REEL_ANIMATION_2_LEFT_DOWN,	// 46
			REEL_ANIMATION_2_LEFT_UP,		// 47

			REEL_ANIMATION_3_UP,			// 48
			REEL_ANIMATION_3_RIGHT,		// 49
			REEL_ANIMATION_3_DOWN,		// 50
			REEL_ANIMATION_3_LEFT,		// 51
			REEL_ANIMATION_3_RIGHT_UP,		// 52
			REEL_ANIMATION_3_RIGHT_DOWN,	// 53
			REEL_ANIMATION_3_LEFT_DOWN,	// 54
			REEL_ANIMATION_3_LEFT_UP		// 55
		};
	};

	
}
