#pragma once
#include <vector>
#include <variant>
#include "Item.h"
#include <SFML/Graphics.hpp>

class InventoryScreen
{
private:
	sf::Texture					inventory_texture; // the background texture
	std::vector<sf::Sprite>		inventory_items;   // All the sprites that will be shown in the inventory
public:
	InventoryScreen(std::vector<std::variant<Rod,Bait,Fish>> inventory);
	InventoryScreen() = default;
	std::vector<sf::Sprite> show();
};

