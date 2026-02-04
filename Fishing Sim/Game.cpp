#include "Game.h"
#include "Player.h"
#include "Item.h"
#include "Event.h"
#include "Shop.h"
#include "Button.h"
#include "Inventory_Screen.h"
#include "PerlinNoise.h" 		// by me

#include <vector>
#include <thread>
#include <string>
#include <fstream>
#include <cmath>
#include <random> 			// C++

#include <SFML/Graphics.hpp> 	// SFML


Game::Game(sf::RenderWindow& main_window) : main_window(main_window) {
	this->player = Player(this->main_window.getSize().x, this->main_window.getSize().y);
};

Game::Game(sf::RenderWindow& main_window, int seed, int player_x, int player_y, float balance, int grip, int reel, int head, int bait,const std::vector<std::pair<int, float>>& fishes)
    : main_window(main_window)
{
    this->player = Player((int)this->main_window.getSize().x, (int)this->main_window.getSize().y,balance,grip,reel,head,bait,fishes);
	this->player.setCoordinates(player_x, player_y);
    this->Seed = seed;
}

	/// rounding to the closest multiple of five
int closestFive(int value) {
	if (value >= 0) return (int)(value / 5.f + 0.5f) * 5;
	else if (value < 0) return (int)(value / 5.f - 0.5f) * 5;
}

	/// Creating the map for the game
	void Game::GenerateMap() {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> seed(100000, 999999); // define the range
		this->Seed = this->Seed == 0 ? seed(gen) : this->Seed;
		PerlinNoise* pn = new PerlinNoise(this->Seed);
		this->tileMapAtlas = sf::Texture("assets/images/tiles_sheet.png");
		this->GameMapSkeleton = pn->generateMatrix(256,256,8);
		
		if (this->player.getCoordinates()[0] == 0 && this->player.getCoordinates()[1] == 0)
		this->player.setCoordinates(this->GameMapSkeleton[0].size() * 31 / 2, this->GameMapSkeleton.size() * 31 / 2); // set player in the middle of the map
		
		delete pn;
	}

	//Player Animantion
	void Game::Animantion(float duration) {
		int initial_state = this->player.getState();
		int delay = 50; // milliseconds
		std::clock_t start_time = std::clock();
			player.setState(Data::Player::THROW_ANIMATION_1_UP + initial_state);
			Game::Render();
			std::this_thread::sleep_for(std::chrono::milliseconds(delay));
			player.setState(Data::Player::THROW_ANIMATION_2_UP + initial_state);
			Game::Render();
			std::this_thread::sleep_for(std::chrono::milliseconds(delay));
			player.setState(Data::Player::THROW_ANIMATION_3_UP + initial_state);
			Game::Render();
			std::clock_t current_time = std::clock();
			while (float(current_time - start_time) < duration) {
				player.setState(Data::Player::REEL_ANIMATION_1_UP + initial_state);
				Game::Render();
				std::this_thread::sleep_for(std::chrono::milliseconds(delay));
				player.setState(Data::Player::REEL_ANIMATION_2_UP + initial_state);
				Game::Render();
				std::this_thread::sleep_for(std::chrono::milliseconds(delay));
				player.setState(Data::Player::REEL_ANIMATION_3_UP + initial_state);
				Game::Render();
				current_time = std::clock();
			}
		player.setState(initial_state);
		Game::Render();
	}

	///Rendering the rectangle that should be rendered,depending on the position of the player
	void Game::Render() {
		if (!this->show_inventory && !this->show_shop) { // || shop_shown

			int current_location[2];
			// 62x39 tiles visible on the screen
						//tiles to pixels			player_location
			current_location[0] = this->GameMapSkeleton[0].size() - (floor((this->GameMapSkeleton[0].size() * 31) - this->player.getCoordinates()[0]) / 31);
			current_location[1] = this->GameMapSkeleton.size() - (floor((this->GameMapSkeleton.size() * 31) - this->player.getCoordinates()[1]) / 31);

			//std::cout << "Current Location: " << current_location[0] << "  " << current_location[1] << std::endl; //should be 128,128 at the start
			int start_coords[2] = { 18,13 };
			int lengths[2] = { 31,31 };
			int tile_offset = 1;
			this->main_window.clear(sf::Color::Black);
			//18 tile uri inaintea current_location[1]	si				21 dupa
			for (int y = current_location[1] - 18, contory = 0; y <= current_location[1] + 21; y++, contory++) {
				//				30 tile uri inaintea current_location[0]	si			32 dupa
				for (int x = current_location[0] - 30, contorx = 0; x <= current_location[0] + 32; x++, contorx++) {
					//  <0     <0,05  <0,1  <0,15  <0,2   <0,25  <0,3  <0,35 <0,4  <0,45  <0,5   <0,55  <0,6  <0,65
					int v = this->GameMapSkeleton[y][x]; // the value of the current file
					v = closestFive(v); // rounding the value for easier debugging
					sf::IntRect rect; //the rectangle from the texture atlas (the tile wich will be shown)
					// Water
					for (int contor = 0, value = 5, grass_value = 200, max_water_value = 65; value * contor < grass_value + 10; contor++) {
						//value = the distance between the values that change the tile
						//the bigger the value,the less frequent the changes resulting in bigger areas of the same tile

						//grass_value = the value that represents the value where grass appears
						//max_water_value = the maximum value for water tiles

						//Make sure that contor * value will be equal to max_water_value at some point
						if (v >= grass_value) { // For Grass
							contor = grass_value / value;
							int multiply_value = v % grass_value;
							rect = sf::IntRect({ start_coords[0] + (lengths[0] * multiply_value) + (2 * ((contor * value % grass_value) / value)) , start_coords[1] * 5 + 6 }, { lengths[0],lengths[1] });
							break;
						}
						if (v <= contor * value && contor * value <= max_water_value) { //For Water
							rect = sf::IntRect({ start_coords[0] + (lengths[0] * contor) + (contor * 2), start_coords[1] }, { lengths[0],lengths[1] });
							break;
						}
						else if (contor == 13) { //For max water value idk why it wouldn't work for the previous if
							rect = sf::IntRect({ start_coords[0] + (lengths[0] * contor) + (contor * 2), start_coords[1] }, { lengths[0],lengths[1] });
							break;
						}
					}
					sf::Sprite sprite(this->tileMapAtlas, rect);
					//std::cout << "Current tile: " << x << "  " << y << std::endl;
					sprite.setPosition(sf::Vector2f(contorx * lengths[0] - (this->player.getCoordinates()[0] - (current_location[0] * lengths[0])), contory * lengths[1] - (this->player.getCoordinates()[1] - (current_location[1] * lengths[0]))/* + (this->player.getCoordinates()[1] - (current_location[1] * 31 - 31))*/));
					//std::cout << "Verificare: " << contorx * 31 - (this->player.getCoordinates()[0] - (current_location[0] * 31)) << "  " << contory * 31 - (this->player.getCoordinates()[1] - (current_location[1] * 31)) << std::endl;
					this->main_window.draw(sprite);
				}
			}
				//Drawing the player in the center of the screen
			this->main_window.draw(this->player.Load());
			
			const sf::Font font("assets/fonts/arial.ttf");
			if (this->shop.isInBounds(this->player.getCoordinates()[0], this->player.getCoordinates()[1], this->GameMapSkeleton[0].size(), this->GameMapSkeleton.size())) {
				std::string shop_string = "Press E to open shop or to sell items";
				sf::Text shopText(font, shop_string, 50);
				shopText.setPosition(sf::Vector2f(this->main_window.getSize().x / 2 - 200, 20));
				this->main_window.draw(shopText);
			}
			else if (this->event != nullptr) {
					std::string event_string = this->event->getNameByType() + " at location X: " + std::to_string(this->event->coordinates[0]) + " Y: " + std::to_string(this->event->coordinates[1]);
					sf::Text eventText(font,event_string,50);
					eventText.setPosition(sf::Vector2f(this->main_window.getSize().x / 2 - 300, 20));
					eventText.setFillColor(this->event->getColorByType());
					this->main_window.draw(eventText);
					if (this->event->shouldRender(this->player.getCoordinates()[0], this->player.getCoordinates()[1],this->GameMapSkeleton[0].size(),this->GameMapSkeleton.size())) {
						this->main_window.draw(this->event->renderEvent(this->player.getCoordinates()[0], this->player.getCoordinates()[1], this->GameMapSkeleton[0].size(), this->GameMapSkeleton.size()));
					}
				}

			std::string coord_string = "X: " + std::to_string(int(this->player.getCoordinates()[0] - (this->GameMapSkeleton[0].size() * 31 / 2))) + " Y: " + std::to_string(int(this->player.getCoordinates()[1] - (this->GameMapSkeleton.size() * 31 / 2)));
			sf::Text coord_text(font, coord_string, 30);
			coord_text.setPosition(sf::Vector2f(10, 10));
			this->main_window.draw(coord_text);

			std::string balance_string = "Balance: " + std::to_string(int(this->player.getBalance())) + " $";
			sf::Text balance_text(font, balance_string, 30);
			balance_text.setPosition(sf::Vector2f(this->main_window.getSize().x - 250, 10));
			this->main_window.draw(balance_text);

			sf::Texture hotbar_texture("assets/images/hotbar.png");
			sf::Sprite hotbar_sprite(hotbar_texture);
			hotbar_sprite.setPosition(sf::Vector2f(this->main_window.getSize().x / 2 - hotbar_texture.getSize().x / 2, this->main_window.getSize().y - hotbar_texture.getSize().y - 10));
			this->main_window.draw(hotbar_sprite);

			int start_x = hotbar_sprite.getPosition().x + 163; // 163 72
			int start_y = hotbar_sprite.getPosition().y + 72;
			for (int i = 0, iterations = this->player.inventory.size() <= 10 ? this->player.inventory.size() : 10; i < iterations; i++) {
				int column_number = i % 10;
				int row_number = i / 10;
				if (i == 0) {
					sf::Sprite head_sprite(Data::Rod::headTextures[std::get<Rod>(this->player.inventory[i]).getHead()]);
					head_sprite.setPosition(sf::Vector2f(column_number * head_sprite.getTexture().getSize().x + start_x, row_number * head_sprite.getTexture().getSize().y + start_y));
					this->main_window.draw(head_sprite);

					sf::Sprite grip_sprite(Data::Rod::gripTextures[std::get<Rod>(this->player.inventory[i]).getGrip()]);
					grip_sprite.setPosition(sf::Vector2f(column_number * grip_sprite.getTexture().getSize().x + start_x, row_number * grip_sprite.getTexture().getSize().y + start_y + 38));
					this->main_window.draw(grip_sprite);

					sf::Sprite reel_sprite(Data::Rod::reelTextures[std::get<Rod>(this->player.inventory[i]).getReel()]);
					reel_sprite.setPosition(sf::Vector2f(column_number * reel_sprite.getTexture().getSize().x + start_x + grip_sprite.getTexture().getSize().x, row_number * reel_sprite.getTexture().getSize().y + start_y + head_sprite.getTexture().getSize().y));
					this->main_window.draw(reel_sprite);
				}
				else if (i == 1) {
					sf::Sprite bait_sprite(Data::Bait::baitTextures[std::get<Bait>(this->player.inventory[i]).getType()]);
					bait_sprite.setPosition(sf::Vector2f(column_number * 64 + start_x, start_y + (row_number * 64))); // 64 and not bait_sprite.getTexture().getSize().x because all of the bait textures have different sizes wich are not equal to 64,so it could colide with other items in the hotbar
					this->main_window.draw(bait_sprite);
				}
				else {
					sf::Sprite fish_sprite(Data::Fish::fishTextures[std::get<Fish>(this->player.inventory[i]).getSpecies()]);
					fish_sprite.setPosition(sf::Vector2f(column_number * 64 + start_x, row_number * 64 + start_y));
					this->main_window.draw(fish_sprite);
				}
			}


			//Displaying everything on the screen
			this->main_window.display();
		}
		else if (this->show_shop) {
			
			const sf::Font font("assets/fonts/arial.ttf");
			
			std::string balance_string = "Balance: " + std::to_string(int(this->player.getBalance())) + " $";
			sf::Text balance_text(font, balance_string, 30);
			balance_text.setPosition(sf::Vector2f(this->main_window.getSize().x - 250, 10));
			this->main_window.draw(balance_text);
		}
	}

	void Game::Listen() {

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
			if (show_shop)		show_shop		= !show_shop;
			if (show_inventory) show_inventory	= !show_inventory;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E) && !show_inventory) {
			//open the shop

			// false -> intra -> true	-- prima apasare (a)
			//true -> nu intra -> false -- a doua apasare
			if (!show_shop) {
				if (this->shop.isInBounds(this->player.getCoordinates()[0], this->player.getCoordinates()[1], this->GameMapSkeleton[0].size(), this->GameMapSkeleton.size())){
					for (auto& items : this->player.inventory) {
						if (items.index() == 2) {
							this->player.addBalance(std::get<Fish>(items).getPrice());
						}
					}
					show_shop = true;
					Game::Render();
					show_shop = false; // to show the balance before opening the shop
					this->player.inventory.resize(2);
					this->shop.renderShop(this->main_window,this->player.inventory);
				}
				else {
					show_shop = true; // to prevent opening the shop when not in range(it will be false after if will end)
				}
			}
			show_shop = !show_shop;
			if (!this->show_shop) {
				if (this->shop.buttons.size() > 2) {
					this->shop.buttons.resize(2);
					this->shop.changeSelectedButton(-1);
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(200)); // to prevent multiple openings/closings of the shop
		}





		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::I) && !show_shop) {
			if(!show_inventory && !show_shop){
				InventoryScreen* inventory_screen = new InventoryScreen(this->player.inventory);
				for (auto& sprite : inventory_screen->show()) {
					this->main_window.draw(sprite);
				}
				this->main_window.display();
				delete inventory_screen;
			}
			show_inventory = !show_inventory;
			std::this_thread::sleep_for(std::chrono::milliseconds(200)); // to prevent multiple openings/closings of the inventory
		}




		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			if (show_shop) {
				sf::Vector2i mouse_pos = sf::Mouse::getPosition(this->main_window);
				for (int i = 0; i < this->shop.buttons.size(); i++) {
					//std::cout << "Button Pos: " << std::get<Type_Button>(this->shop.buttons[i]).getCoordinates()[0] << "  " << std::get<Type_Button>(this->shop.buttons[i]).getCoordinates()[1];
					if (i <= 1 && std::get<Type_Button>(this->shop.buttons[i]).isInBounds(mouse_pos.x, mouse_pos.y)) {
						this->shop.changeSelectedButton(i);
						this->shop.renderShop(this->main_window, this->player.inventory);
						break;
					}
					else if (i >= 2 && std::get<Button>(this->shop.buttons[i]).isInBounds(mouse_pos.x, mouse_pos.y)) {
						Button current_button = std::get<Button>(this->shop.buttons[i]);
						if (current_button.canBuy(this->player.getBalance())) {
							this->player.addBalance(-current_button.getPrice());
							switch(current_button.getItem()) {
								case 0:
									std::get<Rod>(this->player.inventory[0]).setHead(current_button.getType()); break;
								case 1:
									std::get<Rod>(this->player.inventory[0]).setReel(current_button.getType()); break;
								case 2:
									std::get<Rod>(this->player.inventory[0]).setGrip(current_button.getType()); break;
								case 3:
									std::get<Bait>(this->player.inventory[1]).setType(current_button.getType()); break;
								default:
									break;

							}
							
							for (int j = i; j < this->shop.buttons.size() - 1; j++) {
								this->shop.buttons[j] = this->shop.buttons[j + 1];
							}
							this->shop.buttons.pop_back();
							i--;
							this->shop.renderShop(this->main_window, this->player.inventory);
							break;
						}
					}
				}
			}
			if (!this->player.checkIfFullInventory() && !show_shop) {
				int duration = (Data::Rod::LoadReelData(std::get<Rod>(this->player.inventory[0]).getReel()).time - Data::Bait::LoadBaitData(std::get<Bait>(this->player.inventory[1]).getType()).wait_time) * 1000;
				this->Animantion(duration >= 0 ? duration : 0);
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_real_distribution<> weight(1, Data::Rod::LoadHeadData(std::get<Rod>(this->player.inventory[0]).getHead()).maxkg);
				//chance calculation
				float weight_chance = 0.0f;
				for (int i = 0; i < Data::Fish::fishdata.size(); i++) {
					weight_chance += Data::Fish::LoadFishData(i).chance;
				} 		// calculating the summed up chances for all of the fishes
				int fact = 1;
				while ((weight_chance * fact) != int(weight_chance * fact)) {
					fact *= 10;
				}
				std::uniform_int_distribution<> chance(1, int(weight_chance * fact));
				int best_fish = 0; 	// this will be the fish that will be added into the inventory
				// represents the best fish that you will get out of the next 2 for loops
				for (int rolls = 0; rolls < (Data::Rod::LoadGripData(std::get<Rod>(this->player.inventory[0]).getGrip()).chance + Data::Bait::LoadBaitData(std::get<Bait>(this->player.inventory[1]).getType()).chance) + (this->event ? (this->event->isInsideEvent(this->player.getCoordinates()[0],this->player.getCoordinates()[1],this->GameMapSkeleton[0].size(),this->GameMapSkeleton.size()) ? this->event->getLuckModifierByType() : 0) : 0); rolls++) {
					int fish_chance = chance(gen); // the randomly generated number
					weight_chance = 0; // reseting so I can add the chances up you'll see what I'm talking abt
					for (int i = 0; i < Data::Fish::fishdata.size(); i++) {
						// 1  5   10   20   50       / 86
						//30
						weight_chance += Data::Fish::LoadFishData(i).chance * fact; // adding to the sum
						if (weight_chance >= fish_chance) { 		// if the randomly generated number is less than the current 
							//sum of weights it means that  it is the fish that we caught
							// here is a clearer explanation:
								// let's say that we have 3 fishes with chances 10, 20, 50
								// the max chance will be 80
								// and the weight map will look like this:
								// 1..10...30........80
								// now if the randomly generated number is 25 it means that we caught the second fish
							if (best_fish < i) {
								best_fish = i;
							}
							break;
						}
					}
				}
				this->player.inventory.push_back(Fish(best_fish, weight(gen)));
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(200)); // to prevent multiple fish catching
		}



		if (!this->show_inventory && !this->show_shop) {
			bool directions[4] = { false };




			

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
				directions[3] = true;
				std::vector<int> coordinates = this->player.getCoordinates();
				if (coordinates[0] - 5 < 930)
					this->player.setCoordinates(930, coordinates[1]);
				else
					this->player.setCoordinates(coordinates[0] - 5, coordinates[1]);
				//				this->player.setState(Data::Player::LEFT);

			}




			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
				directions[1] = true;
				std::vector<int> coordinates = this->player.getCoordinates();
				if (coordinates[0] + 5 > (this->GameMapSkeleton[0].size() * 31) - 993)
					this->player.setCoordinates((this->GameMapSkeleton[0].size() * 31) - 993, coordinates[1]);
				else
					this->player.setCoordinates(coordinates[0] + 5, coordinates[1]);
				//				this->player.setState(Data::Player::RIGHT);

			}



			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
				directions[0] = true;
				std::vector<int> coordinates = this->player.getCoordinates();
				if (coordinates[1] - 5 < 558)
					this->player.setCoordinates(coordinates[0], 558);
				else
					this->player.setCoordinates(coordinates[0], coordinates[1] - 5);
				//				this->player.setState(Data::Player::UP);

			}



			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
				directions[2] = true;
				std::vector<int> coordinates = this->player.getCoordinates();
				if (coordinates[1] + 5 > (this->GameMapSkeleton.size() * 31) - 652)
					this->player.setCoordinates(coordinates[0], (this->GameMapSkeleton.size() * 31) - 652);
				else
					this->player.setCoordinates(coordinates[0], coordinates[1] + 5);
				//				this->player.setState(Data::Player::DOWN);

			}



			if (directions[0] && directions[1]) this->player.setState(Data::Player::RIGHT_UP);
			else if (directions[0] && directions[3]) this->player.setState(Data::Player::LEFT_UP);
			else if (directions[2] && directions[1]) this->player.setState(Data::Player::RIGHT_DOWN);
			else if (directions[2] && directions[3]) this->player.setState(Data::Player::LEFT_DOWN);
			else if (directions[0]) this->player.setState(Data::Player::UP);
			else if (directions[1]) this->player.setState(Data::Player::RIGHT);
			else if (directions[2]) this->player.setState(Data::Player::DOWN);
			else if (directions[3]) this->player.setState(Data::Player::LEFT);
		}
	}
	/// <summary>
	/// The Game Loop
	/// </summary>
	void Game::Loop() {
		while (this->main_window.isOpen()) {
			while (const std::optional event = this->main_window.pollEvent())
			{
				// Close window: exit
				if (event->is<sf::Event::Closed>()) {
					Game::Save();
					this->main_window.close();
				}
			}
			if (!this->event && this->event_clock.getElapsedTime().asMilliseconds() >= wait_time) {
				this->event = std::make_unique<Event>(this->GameMapSkeleton[0].size(),this->GameMapSkeleton.size());
				wait_time = 0;
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<> event_time_dist(Data::Event::MIN_TIME, Data::Event::MAX_TIME); // define the range
				event_time = event_time_dist(gen);
				this->event_clock.restart();
			}
			else if (this->event && this->event_clock.getElapsedTime().asMilliseconds() >= event_time) {
				this->event.reset();
				event_time = 0;
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<> wait_time_dist(Data::Event::MIN_WAIT_TIME, Data::Event::MAX_WAIT_TIME); // define the range
				wait_time = wait_time_dist(gen);
				this->event_clock.restart();
			}
			//Game Logic
			Game::Listen();
			Game::Render();
		}
	}
	/// <summary>
	/// Loads the Map,Player and the other assets of the game
	/// </summary>
	void Game::Start() {
		///Generating and rendering the map to the screen
		Game::GenerateMap();
		Game::Render();
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> wait_time(Data::Event::MIN_WAIT_TIME, 60000); // 0 sec -> 60 sec
		this->wait_time = wait_time(gen);
		this->event_clock.restart();
		///Starting the game loop
		Game::Loop();
	}
	/// <summary>
	/// Stops the game
	/// </summary>
	void Game::Save() {
		std::string save_information;
		save_information.append("Seed: " + std::to_string(this->Seed) + '\n');
		save_information.append("Player X: " + std::to_string(this->player.getCoordinates()[0]) + '\n');
		save_information.append("Player Y: " + std::to_string(this->player.getCoordinates()[1]) + '\n');
		save_information.append("Balance: " + std::to_string(this->player.getBalance()) + '\n');
		save_information.append("Rod: " + std::to_string(std::get<Rod>(this->player.inventory[0]).getGrip()) + " " + 
										  std::to_string(std::get<Rod>(this->player.inventory[0]).getReel()) + " " + 
										  std::to_string(std::get<Rod>(this->player.inventory[0]).getHead()) + '\n');
		save_information.append("Bait: " + std::to_string(std::get<Bait>(this->player.inventory[1]).getType()) + '\n');
		save_information.append("Fishes: ");
		for (auto& fish : this->player.inventory) {
			if (fish.index() == 2) {
				save_information.append(std::to_string(std::get<Fish>(fish).getSpecies()) + " " + std::to_string(std::get<Fish>(fish).getWeight()) + ",");
			}
		}
		
		std::ofstream save_file("savefile.txt");
		save_file << save_information;
		save_file.close();

	}

	/// <summary>
	/// Shows the map(use after calling the Start() function)
	/// </summary>
	void Game::showMap() {
		std::ofstream map_file("mapfile.txt");
		for (int i = 0; i < GameMapSkeleton.size(); i ++) {
			for (int j = 0; j < GameMapSkeleton[i].size(); j++) {
				map_file << GameMapSkeleton[i][j] << " ";
			}
			map_file << '\n';
		}
	}

