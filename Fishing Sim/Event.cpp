#include "Event.h"
#include <random>
#include <SFML/Graphics.hpp>

void Event::rollEvent() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> roll(1, Data::Event::RARE_CHANCE + Data::Event::EPIC_CHANCE + Data::Event::LEGENDARY_CHANCE + Data::Event::MYTHICAL_CHANCE);
	int chance = roll(gen);
	int weighted_chance = 0;
	for (int i = 0; i < 4; i++) { // 4 - the number of rarities
		weighted_chance += (i == 0 ? Data::Event::RARE_CHANCE : (i == 1 ? Data::Event::EPIC_CHANCE : (i == 2 ? Data::Event::LEGENDARY_CHANCE : Data::Event::MYTHICAL_CHANCE)));
		if (chance <= weighted_chance) {
			this->type = i;
			break;
		}
	}
}

Event::Event(int gamemap_size_x,int gamemap_size_y) {
	this->rollEvent();
	int min_x = 930;
	int min_y = 558;
	int max_x = (gamemap_size_x * 31) - 993;
	int max_y = (gamemap_size_y * 31) - 652;
	int min_length = 320;
	int max_length = 640;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> coord_x(min_x,max_x - min_length - 2);
	std::uniform_int_distribution<> coord_y(min_y, max_y - min_length - 2);
	std::uniform_int_distribution<> length_gen(min_length, max_length);
	this->coordinates[0] = coord_x(gen) - (gamemap_size_x * 31 / 2);
	this->coordinates[1] = coord_y(gen) - (gamemap_size_y * 31 / 2);
	int possible_length = length_gen(gen);
	this->lengths[0] = max_x - this->coordinates[0] < possible_length ? max_x - this->coordinates[0] : possible_length;
	possible_length = length_gen(gen);
	this->lengths[1] = max_y - this->coordinates[1] < possible_length ? max_y - this->coordinates[1] : possible_length;
}

bool Event::shouldRender(int playerX,int playerY,int gamemap_size_x,int gamemap_size_y) {
	int playerX_virtual_coordinates = playerX - (gamemap_size_x * 31 / 2);
	int playerY_virtual_coordinates = playerY - (gamemap_size_y * 31 / 2);
	if ((playerX_virtual_coordinates - 960 <= this->coordinates[0] || playerX_virtual_coordinates - 960 <= this->coordinates[0] + this->lengths[0]) && (playerX_virtual_coordinates + 960 >= this->coordinates[0] || playerX_virtual_coordinates + 960 >= this->coordinates[0] + this->lengths[0])) {
		if ((playerY_virtual_coordinates - 600 <= this->coordinates[1] || playerY_virtual_coordinates - 600 <= this->coordinates[1] + this->lengths[1]) && (playerY_virtual_coordinates + 600 >= this->coordinates[1] || playerY_virtual_coordinates + 600 >= this->coordinates[1] + this->lengths[1])) {
			return true;
		}
	}
	return false;
}

sf::RectangleShape Event::renderEvent(int playerX,int playerY,int gamemap_size_x, int gamemap_size_y) {
	sf::RectangleShape event_shape;
	int playerX_virtual_coordinates = playerX - (gamemap_size_x * 31 / 2);
	int playerY_virtual_coordinates = playerY - (gamemap_size_y * 31 / 2);
	event_shape.setPosition(sf::Vector2f(960 - (playerX_virtual_coordinates - this->coordinates[0]), 600 - (playerY_virtual_coordinates - this->coordinates[1])));
	event_shape.setSize(sf::Vector2f(this->lengths[0], this->lengths[1]));
	event_shape.setFillColor(sf::Color::Transparent);
	event_shape.setOutlineThickness(5.f);
	event_shape.setOutlineColor(this->getColorByType());
	return event_shape;
}

bool Event::isInsideEvent(int playerX, int playerY, int gamemap_size_x, int gamemap_size_y) {
	int playerX_virtual_coordinates = playerX - (gamemap_size_x * 31 / 2);
	int playerY_virtual_coordinates = playerY - (gamemap_size_y * 31 / 2);
	if (playerX_virtual_coordinates >= this->coordinates[0] && playerX_virtual_coordinates <= this->coordinates[0] + this->lengths[0]) {
		if (playerY_virtual_coordinates >= this->coordinates[1] && playerY_virtual_coordinates <= this->coordinates[1] + this->lengths[1]) {
			return true;
		}
	}
	return false;
}

int Event::getLuckModifierByType() {
	return	this->type == 0 ? int(Data::Event::RARE_LUCK_MODIFIER) :
			this->type == 1 ? Data::Event::EPIC_LUCK_MODIFIER :
			this->type == 2 ? Data::Event::LEGENDARY_LUCK_MODIFIER :
							  Data::Event::MYTHICAL_LUCK_MODIFIER;
}

sf::Color Event::getColorByType() {
	return	this->type == 0 ? sf::Color::Blue :
			this->type == 1 ? sf::Color::Magenta :
			this->type == 2 ? sf::Color::Yellow :
							  sf::Color::Red;
}
std::string Event::getNameByType() {
	return	this->type == 0 ? "Rare Event" :
			this->type == 1 ? "Epic Event" :
			this->type == 2 ? "Legendary Event" :
							  "Mythical Event";
}
