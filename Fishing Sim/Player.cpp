#include <SFML/Graphics.hpp>
#include <vector>
#include "Item.h"
#include "Player.h"
#include <utility>

Player::Player(int screen_width, int screen_height) : screen_coords{screen_width,screen_height}
{
    if (!this->player_texture.loadFromFile("assets/images/player.png"))
    {
    }
    this->inventory.push_back(Rod(0,0,0));
    this->inventory.push_back(Bait(0));
}

Player::Player(int screen_width, int screen_height, float balance, int grip, int reel, int head,int bait, const std::vector<std::pair<int,float>>& fishes)
    : screen_coords{screen_width,screen_height}, balance(balance)
{
    if (!this->player_texture.loadFromFile("assets/images/player.png"))
    {
    }
    this->inventory.push_back(Rod(head,reel,grip));
    this->inventory.push_back(Bait(bait));
    for (auto& p : fishes) {
        // p.first -> species/id, p.second -> weight
        this->inventory.push_back(Fish(p.first, p.second));
    }
}

sf::Sprite Player::Load()
{
    int start_point[2] = { 3, 3 };
    int lengths[2] = {23,53};
    sf::Sprite player_sprite(player_texture);
    sf::IntRect player_rect;
    //33            63
    if (currentState <= Data::Player::LEFT_UP) {
        player_rect = sf::IntRect({ 3,3 }, { lengths[0],lengths[1] });
    }
    else if (currentState <= Data::Player::THROW_ANIMATION_1_LEFT_UP) {
        player_rect = sf::IntRect({ 31,3 }, { lengths[0],lengths[1] }); 
    }
    else if (currentState <= Data::Player::THROW_ANIMATION_2_LEFT_UP) {
        player_rect = sf::IntRect({ 59,3 }, { lengths[0],lengths[1] }); 
    }
    else if (currentState <= Data::Player::THROW_ANIMATION_3_LEFT_UP) {
        player_rect = sf::IntRect({ 87,3 }, { lengths[0],lengths[1] }); 
    }
    else if (currentState <= Data::Player::REEL_ANIMATION_1_LEFT_UP) {
        player_rect = sf::IntRect({ 31,3 }, { lengths[0],lengths[1] }); 
    }
    else if (currentState <= Data::Player::REEL_ANIMATION_2_LEFT_UP) {
        player_rect = sf::IntRect({ 59,3 }, { lengths[0],lengths[1] });
    }
    else if (currentState <= Data::Player::REEL_ANIMATION_3_LEFT_UP) {
        player_rect = sf::IntRect({ 87,3 }, { lengths[0],lengths[1] });
    }
    if (currentState % 8 <= 3) {   // getting the rotation
        player_sprite.rotate(sf::Angle(sf::degrees(90 * currentState)));
    }
    else {      // also getting the rotation , who would have thought?
        player_sprite.rotate(sf::Angle(sf::degrees(45 + (90 * (currentState - 4 )))));
    }
    player_texture.loadFromFile("assets/images/player.png", false, player_rect);
    player_sprite.setPosition(sf::Vector2f(screen_coords[0] / 2, screen_coords[1] / 2));
    return player_sprite;
}

void Player::addBalance(float amount) {
    this->balance += amount;
}

float Player::getBalance() {
    return this->balance;
}

int Player::getState() {
    return this->currentState;
}
void Player::setState(int state) {
    this->currentState = state;
}

    /// Returns the current coordinates of the player
std::vector<int> Player::getCoordinates() {
    return {this->coords[0],this->coords[1]};
}

    /// Sets the coordinates for the player
void Player::setCoordinates(int x, int y) {
    this->coords[0] = x;
    this->coords[1] = y;
}
    /// Checks if the player's inventory is full
bool Player::checkIfFullInventory()
{
    if (inventory.size() == 50) {
        return true;
    }
    return false;
}

