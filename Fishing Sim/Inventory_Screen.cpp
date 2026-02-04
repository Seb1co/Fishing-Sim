#include "Inventory_Screen.h"
#include <iostream>
#include "Item.h"

InventoryScreen::InventoryScreen(std::vector<std::variant<Rod,Bait,Fish>> inventory) {
	if (!this->inventory_texture.loadFromFile("assets/images/inventory_screen.png"))
	{
		std::cout << "Could not load inventory screen texture";
	}
	else { // it succeds 
		sf::Sprite inventory_screen_sprite(this->inventory_texture);
		inventory_screen_sprite.setPosition(sf::Vector2f(450, 250));
		this->inventory_items.push_back(inventory_screen_sprite);
		int contor = 0;
		//Where the fish will be located 179 216
		int start_x = inventory_screen_sprite.getGlobalBounds().position.x + 179; // starting position for the fish sprites on the x axis
		int start_y = inventory_screen_sprite.getGlobalBounds().position.y + 216; // starting position for the fish sprites on the y axis
		for (auto& item : inventory) {
			int column_number = contor % 10;
			int row_number = contor / 10;
			if (item.index() == 2) {// fish
				sf::Sprite fish_sprite(Data::Fish::fishTextures[std::get<Fish>(item).getSpecies()]);
				fish_sprite.setPosition(sf::Vector2f(column_number * 64 + start_x, row_number * 64 + start_y));
				this->inventory_items.push_back(fish_sprite);
			}
			else if (item.index() == 0) { // rod
				sf::Sprite head_sprite(Data::Rod::headTextures[std::get<Rod>(item).getHead()]);
				head_sprite.setPosition(sf::Vector2f(column_number * head_sprite.getTexture().getSize().x + start_x, row_number * head_sprite.getTexture().getSize().y + start_y));
				this->inventory_items.push_back(head_sprite);
				sf::Sprite grip_sprite(Data::Rod::gripTextures[std::get<Rod>(item).getGrip()]);
				grip_sprite.setPosition(sf::Vector2f(column_number * grip_sprite.getTexture().getSize().x + start_x, row_number * grip_sprite.getTexture().getSize().y + start_y + 38));
				this->inventory_items.push_back(grip_sprite);
				sf::Sprite reel_sprite(Data::Rod::reelTextures[std::get<Rod>(item).getReel()]);
				reel_sprite.setPosition(sf::Vector2f(column_number * reel_sprite.getTexture().getSize().x + start_x + grip_sprite.getTexture().getSize().x, row_number * reel_sprite.getTexture().getSize().y + start_y + head_sprite.getTexture().getSize().y));
				this->inventory_items.push_back(reel_sprite);
			}
			else { // bait
				sf::Sprite bait_sprite(Data::Bait::baitTextures[std::get<Bait>(item).getType()]);
				bait_sprite.setPosition(sf::Vector2f(column_number * 64 + start_x,start_y + (row_number * 64))); // 64 and not bait_sprite.getTexture().getSize().x or y because all of the bait textures
																																				   //have different sizes wich are not equal to 64,so it could colide with other items in the hotbar
				this->inventory_items.push_back(bait_sprite);
			}
			contor++;
		}
	}
}
std::vector<sf::Sprite> InventoryScreen::show() {
	return inventory_items;
}