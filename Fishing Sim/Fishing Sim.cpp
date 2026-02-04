#include <fstream>
#include "Game.h"
#include <string>
#include <sstream>
#include <vector>
#include <random>
#include <utility> 

#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow main_window(sf::VideoMode({ 1920,1200 }), "SFML window", sf::State::Fullscreen, sf::ContextSettings());
	main_window.setFramerateLimit(60);
    std::ifstream file("savefile.txt");
    if (file) {
        // Variables to store data
        int seed = 0, playerX = 0, playerY = 0;
        double balance = 0.0;
        int grip = 0, reel = 0, head = 0;
        int bait = 0;
        std::vector<std::pair<int, float>> fishes; // (fishID, weight)

        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string label;
            if (!(ss >> label)) continue; // skip empty lines

            if (label == "Seed:") {
                ss >> seed;
            }
            else if (label == "Player") {
                std::string coord;
                ss >> coord; // X: or Y:
                if (coord == "X:") ss >> playerX;
                else if (coord == "Y:") ss >> playerY;
            }
            else if (label == "Balance:") {
                ss >> balance;
            }
            else if (label == "Rod:") {
                ss >> grip >> reel >> head;
            }
            else if (label == "Bait:") {
                ss >> bait;
            }
            else if (label == "Fishes:") {
                // Read rest of the line after the label
                std::string rest;
                std::getline(ss, rest);
                // trim leading spaces
                size_t start = rest.find_first_not_of(" \t");
                if (start == std::string::npos) continue; // nothing after Fishes:
                rest = rest.substr(start);

                // split by comma
                size_t pos = 0;
                while (pos < rest.size()) {
                    size_t comma = rest.find(',', pos);
                    std::string token;
                    if (comma == std::string::npos) {
                        token = rest.substr(pos);
                        pos = rest.size();
                    } else {
                        token = rest.substr(pos, comma - pos);
                        pos = comma + 1;
                    }
                    // trim token
                    size_t a = token.find_first_not_of(" \t");
                    if (a == std::string::npos) continue;
                    size_t b = token.find_last_not_of(" \t");
                    token = token.substr(a, b - a + 1);

                    std::stringstream fs(token);
                    int id;
                    double weight;
                    if (fs >> id >> weight) {
                        fishes.emplace_back(id, static_cast<float>(weight));
                    }
                    else {
                        // try splitting by space
                        std::istringstream is(token);
                        if (is >> id) {
                            if (is >> weight) {
                                fishes.emplace_back(id, static_cast<float>(weight));
                            }
                        }
                    }
                }
            }
        }
        file.close();
        if (seed > 999999 || seed < 100000) {
            std::random_device rd;
			std::mt19937* gen = new std::mt19937(rd());
			std::uniform_int_distribution<> seed_dist(100000, 999999); // define the range
			seed = seed_dist(*gen);
			delete gen;
        }
		if (grip < 0 || grip > 4) grip = 0;
		if (reel < 0 || reel > 4) reel = 0;
		if (head < 0 || head > 4) head = 0;
		if (bait < 0 || bait > 4) bait = 0;     // Validate rod and bait types
		for (auto& fish : fishes) if (fish.first < 0 || fish.first > 4) fish.first = 0;    // Validate fish types
		// Start the game with loaded data
		Game* game = new Game(main_window, seed, playerX, playerY, balance, grip, reel, head, bait, fishes);
        game->Start();
		delete game;
    }
    else {
        Game* game = new Game(main_window);
        game->Start();
        delete game;
    }
}