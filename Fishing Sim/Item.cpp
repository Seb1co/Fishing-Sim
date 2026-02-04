#include "Item.h"
#include <cmath>
#include <vector>
#include <cstring>
#include <iostream>



	//
	Data::Fish::FishData Data::Fish::LoadFishData(int species) {
		return Data::Fish::fishdata.at(species);
	}
	//
	Data::Bait::BaitData Data::Bait::LoadBaitData(int type) {
		return Data::Bait::baitdata.at(type);
	}
	//
	Data::Rod::HeadData Data::Rod::LoadHeadData(int material) {
		return Data::Rod::headData.at(material);
	}
	Data::Rod::ReelData Data::Rod::LoadReelData(int material) {
		return Data::Rod::reelData.at(material);
	}
	Data::Rod::GripData Data::Rod::LoadGripData(int material) {
		return Data::Rod::gripData.at(material);
	}
					/// ROD ///

/// Constructor

Rod::Rod(int grip, int reel, int head) : Head(head), Reel(reel), Grip(grip) { Id = 0; };

/// GET
int Rod::getGrip(){
	return this->Grip;
}
int Rod::getHead() {
	return this->Head;
}
int Rod::getReel() {
	return this->Reel;
}
int Rod::getId() {
	return this->Id;
}

/// SET
void Rod::setGrip(int grip) {
	this->Grip = grip;
}
void Rod::setHead(int head) {
	this->Head = head;
}
void Rod::setReel(int reel) {
	this->Reel = reel;
}

					/// BAIT ///
					
/// Constructor

Bait::Bait(int type) : Type(type) { Id = 1; };


/// GET

int Bait::getType() {
	return this->Type;
}
int Bait::getId() {
	return this->Id;
}

/// SET

void Bait::setType(int type) {
	this->Type = type;
}

					/// FISH ///
/// Constructor

Fish::Fish(int species, float weight) : Species(species), Weight(weight),Id(2) {
	this->Price = Data::Fish::LoadFishData(Species).basePrice * (sqrt(Weight) >= 1 ? sqrt(Weight) : 1);// can't be less than the base price
};

/// GET

int Fish::getSpecies() {
	return this->Species;
}
double Fish::getWeight() {
	return this->Weight;
}
float Fish::getPrice() {
	return this->Price;
}
int Fish::getId() {
	return this->Id;
}


/// SET

void Fish::setSpecies(int species) {
	this->Species = species;
}
void Fish::setWeight(float weight) {
	this->Weight = Weight;
}
