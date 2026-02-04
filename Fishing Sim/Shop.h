#pragma once

#include <vector>
#include "Player.h"
#include "Button.h"
#include <variant>
#include <SFML/Graphics.hpp>

class Shop {
private:
	using Buttons = std::variant<Type_Button, Button>;
	int currentSelectedButton = -1;
public:
	Shop() = default;
	std::vector<Buttons> buttons;
	int getTypeOfButton(Buttons buton);
	void changeSelectedButton(int pressed_button);
	void renderShop(sf::RenderWindow& window,std::vector<std::variant<Rod,Bait,Fish>>& inventory);
	bool isInBounds(int playerX, int playerY, int gamemap_size_x, int gamemap_size_y);
};

namespace Data {
	class Shop {
	public:
		inline static sf::Texture shopTexture = sf::Texture("assets/images/shop_gui.png");
		inline static sf::Texture buttons_textures[2] = { sf::Texture("assets/images/rod/shop_rod.png"),
														  sf::Texture("assets/images/bait/meat.png")};
	};
}
