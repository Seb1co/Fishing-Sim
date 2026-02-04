#include "Shop.h"
#include "Item.h"
#include <SFML/Graphics.hpp>
#include <vector>

int Shop::getTypeOfButton(Buttons button) {
	return button.index();
}

void Shop::changeSelectedButton(int pressed_button) {

	if(this->currentSelectedButton == -1){
		this->currentSelectedButton = pressed_button;
		std::get<Type_Button>(this->buttons[this->currentSelectedButton]).isSelected = true;
	}
	else {
		std::get<Type_Button>(this->buttons[this->currentSelectedButton]).isSelected = false;
		this->currentSelectedButton = pressed_button;
		if (!(pressed_button == -1)) {
			std::get<Type_Button>(this->buttons[this->currentSelectedButton]).isSelected = true;
		}
	}
	if (this->buttons.size() > 2) this->buttons.resize(2);
}

bool Shop::isInBounds(int playerX, int playerY, int gamemap_size_x, int gamemap_size_y) {
	auto min = [](int a, int b) {return a < b ? a : b; };
	auto max = [](int a, int b) {return a > b ? a : b; };
	int player_x = playerX;
	int shop_x = gamemap_size_x * 31 / 2 - 200;
	int player_y = playerY;
	int shop_y = gamemap_size_y * 31 / 2 - 200;
	if ((max(player_x, shop_x) == min(player_x, shop_x + 400)) && (max(player_y, shop_y) == min(player_y, shop_y + 400))) return true;
	return false;
}

void Shop::renderShop(sf::RenderWindow& window,std::vector<std::variant<Rod,Bait,Fish>>& inventory) {
	sf::Sprite shop_sprite(Data::Shop::shopTexture);
	shop_sprite.setPosition(sf::Vector2f(window.getSize().x / 2 - shop_sprite.getTexture().getSize().x / 2, 0));
	window.draw(shop_sprite);
	int start_x = shop_sprite.getPosition().x + 260;
	int start_y = shop_sprite.getPosition().y + 280;
	int spacing_x = 200;
	for (int i = 0; i < 2; i++) {
		sf::Sprite button_sprite(Data::Shop::buttons_textures[i]);
		button_sprite.setPosition(sf::Vector2f(start_x + (i * 64) + (i * spacing_x), start_y));
		if (this->buttons.size() <= 1) this->buttons.push_back(Type_Button(button_sprite.getPosition().x, button_sprite.getPosition().y));
		else { 
			float scale;
			if (std::get<Type_Button>(this->buttons[i]).isSelected) {
					scale = 1.5f;
				int purchasable_start_x = start_x;
				int purchasable_start_y = start_y + 150;
				int purchasable_spacing_x = 32;
				int purchasable_spacing_y = 64;
				if (i == 0) {
					for (int j = std::get<Rod>(inventory[i]).getGrip() + 1, divide_factor = j; j < Data::Rod::gripData.size(); j++) {
						sf::Sprite grip_sprite(Data::Rod::gripTextures[j]);
						grip_sprite.setPosition(sf::Vector2f(purchasable_start_x + ((j - divide_factor) * 64) + ((j - divide_factor) * purchasable_spacing_x), purchasable_start_y));
						
						Button grip_button(grip_sprite.getPosition().x, grip_sprite.getPosition().y, Data::Rod::LoadGripData(j).price,2,j);
						this->buttons.push_back(grip_button);

						window.draw(grip_sprite);
					}
					purchasable_start_y += purchasable_spacing_y;
					for (int j = std::get<Rod>(inventory[i]).getReel() + 1,divide_factor = j; j < Data::Rod::reelData.size(); j++) {
						sf::Sprite reel_sprite(Data::Rod::reelTextures[j]);
						reel_sprite.setPosition(sf::Vector2f(purchasable_start_x + ((j - divide_factor) * 64) + ((j - divide_factor) * purchasable_spacing_x), purchasable_start_y));
						
						Button reel_button(reel_sprite.getPosition().x, reel_sprite.getPosition().y, Data::Rod::LoadReelData(j).price,1,j);
						this->buttons.push_back(reel_button);
						
						window.draw(reel_sprite);
					}
					purchasable_start_y += purchasable_spacing_y;
					for (int j = std::get<Rod>(inventory[i]).getHead() + 1,divide_factor = j; j < Data::Rod::headData.size(); j++) {
						sf::Sprite head_sprite(Data::Rod::headTextures[j]);
						head_sprite.setPosition(sf::Vector2f(purchasable_start_x + ((j - divide_factor) * 64) + ((j - divide_factor) * purchasable_spacing_x), purchasable_start_y));
						
						Button head_button(head_sprite.getPosition().x, head_sprite.getPosition().y, Data::Rod::LoadHeadData(j).price,0,j);
						this->buttons.push_back(head_button);
						
						window.draw(head_sprite);
					}
				}
				else if (i == 1) {
					for (int j = std::get<Bait>(inventory[i]).getType() + 1,divide_factor = j; j < Data::Bait::baitdata.size(); j++) {
						sf::Sprite bait_sprite(Data::Bait::baitTextures[j]);
						bait_sprite.setPosition(sf::Vector2f(purchasable_start_x + ((j - divide_factor) * 64) + ((j - divide_factor) * purchasable_spacing_x), purchasable_start_y));
						
						Button bait_button(bait_sprite.getPosition().x, bait_sprite.getPosition().y, Data::Bait::LoadBaitData(j).price,3,j);
						this->buttons.push_back(bait_button);
						
						window.draw(bait_sprite);
					}
				}
			}
			else { scale = 1.f; }
			button_sprite.setScale(sf::Vector2f(scale, scale));
		}
		window.draw(button_sprite);

	}
	window.display();
}