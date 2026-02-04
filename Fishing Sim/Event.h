#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Event
{
private:
	void rollEvent();
public:
	Event(int gamemap_size_x,int gamemamp_size_y);
	int type;
	int coordinates[2];
	int lengths[2];
	bool shouldRender(int playerX,int playerY,int gamemap_size_x,int gamemap_size_y);
	bool isInsideEvent(int playerX, int playerY, int gamemap_size_x, int gamemap_size_y);
	sf::RectangleShape renderEvent(int playerX, int playerY,int gamemap_size_x, int gamemap_size_y);
	sf::Color getColorByType();
	std::string getNameByType();
	int getLuckModifierByType();
};

namespace Data
{
	class Event {
	public:
		enum Time {
			MIN_TIME = 120000,		// 2 min
			MAX_TIME = 240000,		// 4 min
			MIN_WAIT_TIME = 0,		// instantly
			MAX_WAIT_TIME = 300000	// 5 min
		};
		enum Type {
			RARE,
			EPIC,
			LEGENDARY,
			MYTHICAL
		};
		enum Type_Chance {
			RARE_CHANCE = 100,		
			EPIC_CHANCE = 15,		
			LEGENDARY_CHANCE = 7,	
			MYTHICAL_CHANCE = 2		
		};
		enum Modifiers {
			RARE_LUCK_MODIFIER = 3,
			EPIC_LUCK_MODIFIER = 5,
			LEGENDARY_LUCK_MODIFIER = 8,
			MYTHICAL_LUCK_MODIFIER = 12
		};
	};
}

