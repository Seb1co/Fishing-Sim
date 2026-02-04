#pragma once
#include <SFML/Graphics.hpp>
#include "Item.h"

class Button
{
private:
	int coordinates[2];
	float price;
	short item;				// 0 - head
							// 1 - reel
							// 2 - grip
							// 3 - bait
	short type;	// type of head/reel/grip/bait
public:
	Button(int button_x, int button_y, float price, short item, short type_of_item);
	Button() = default;
	bool isInBounds(int mouse_x,int mouse_y);
	bool canBuy(int player_balance);
	inline short getItem() { return this->item; }
	inline short getType() { return this->type; }
	inline float getPrice() { return this->price; }
	inline int* getCoordinates() { return coordinates; };
	void operator=(const Button& other) {
		this->coordinates[0] = other.coordinates[0];
		this->coordinates[1] = other.coordinates[1];
		this->price = other.price;
		this->item = other.item;
		this->type = other.type;
	}
	bool operator==(const Button& other) {
		return this->coordinates[0] == other.coordinates[0] &&
			this->coordinates[1] == other.coordinates[1] && 
			this->price == other.price &&
			this->item == other.item &&
			this->type == other.type;
	};
};

class Type_Button{
private:
	int coordinates[2];
public:
	Type_Button(int button_x, int button_y);
	Type_Button() = default;
	bool isSelected = false;
	bool isInBounds(int mouse_x,int mouse_y);
	inline int* getCoordinates() { return coordinates; };
	void operator=(const Type_Button& other) {
		this->coordinates[0] = other.coordinates[0];
		this->coordinates[1] = other.coordinates[1];
		this->isSelected = other.isSelected;
	}
	bool operator==(const Type_Button& other) {
		return this->coordinates[0] == other.coordinates[0] && this->coordinates[1] == other.coordinates[1] && this->isSelected == other.isSelected;
	};
};

