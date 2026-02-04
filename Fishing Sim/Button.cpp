#include "Button.h"

Button::Button(int button_x, int button_y, float price,short item,short type_of_item) : price(price), coordinates{ button_x,button_y },item(item),type(type_of_item) {
}

bool Button::isInBounds(int mouse_x, int mouse_y) {
	if (mouse_x >= this->coordinates[0] && mouse_x <= this->coordinates[0] + 64) {
		if (mouse_y >= this->coordinates[1] && mouse_y <= this->coordinates[1] + 64) {
			return true;
		}
	}
	return false;
}

bool Button::canBuy(int player_balance) {
	return player_balance >= this->price;
}

Type_Button::Type_Button(int button_x, int button_y) : coordinates{button_x,button_y} {
}

bool Type_Button::isInBounds(int mouse_x, int mouse_y) {
	return (mouse_x >= this->coordinates[0] && mouse_x <= this->coordinates[0] + 64) && (mouse_y >= this->coordinates[1] && mouse_y <= this->coordinates[1] + 64);
}